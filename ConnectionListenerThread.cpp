/*-----------------------------------------------------------------------------\
|                         ____                                                 |
|      __                / __ \                                /\     /\       |
|     /_/               / /  \/                               / /    / /       |
|     __    ____       / /_    ____     ____       ____      / /    / /        |
|    / /   / __ \     / ___\  / __ \   / __ \     / __ \    / /    / /   /\  /\|
|   / /   / /  \ \   / /     / /  \/  / /  \ \   / ____/   / /    / /   / / / /|
|  / /_  / /   / /  / /     / /       \ \__/ /_  \  \___  / /_   / /_   \ \/ / |
|  \__/  \/    \/   \/      \/         \______/   \____/  \__/   \__/    \  /  |
|                                                                        / /   |
| ______________________________________________________________________/ /    |
|/   ____________________________________________________________________/     |
|\__/                                                                          |
|                                                                              |
|                                                                              |
|   Infraelly MMORPG                                                           |
|   Copyright (C) 2007-2010 Tony Huynh aka insanepotato                        |
|                                                                              |
|   Visit: http://sourceforge.net/projects/infraelly/                          |
|                                                                              |
|   License: (LGPL)                                                            |
|   This is free software; you can redistribute it and/or                      |
|   modify it under the terms of the GNU Library General Public                |
|   License as published by the Free Software Foundation; either               |
|   version 2 of the License, or (at your option) any later version.           |
|                                                                              |
|   This is distributed in the hope that it will be useful,                    |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of             |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          |
|   Library General Public License for more details.                           |
|                                                                              |
|   You should have received a copy of the GNU Library General Public          |
|   License along with this library; if not, write to the Free                 |
|   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  |
|                                                                              |
|   Tony Huynh                                                                 |
|   tony.huynh1991@gmail.com                                                   |
|                                                                              |
L-----------------------------------------------------------------------------*/


#include "ConnectionListenerThread.hpp"

#include <memory>

#include "INFPacket.hpp"
#include "ServerContext.hpp"
#include "GameConfig.hpp"


ConnectionListenerThread::ConnectionListenerThread(ServerContext& server) :
    server_(server),
    socketsReady_(0)
{
    inp::INFPacket pack;
    pack << inp::DataTypeByte::THREAD_SEND_CONSOLE;

    if( listenConnection_.initServer(GameConfig::serverPort) == -1 ){
        pack << "Server init failed... Please see stderr.txt";
    } else {
        pack << "Listening connection opened";
    }
    sendOut(pack);
}

ConnectionListenerThread::~ConnectionListenerThread(){
    inp::INFPacket pack;
    pack << inp::DataTypeByte::THREAD_SEND_CONSOLE << "Listening connection closed";
    sendOut(pack);
}


bool ConnectionListenerThread::work(){
    // get a queue of players waiting to log in
    socketsReady_ = 1;
    while( socketsReady_ && (loginQueue_.size()!=maxQueueLength_) ){
        socketsReady_ = listenConnection_.checkSet( 50 );
        if( socketsReady_ == -1 ){
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_CheckSockets:" << SDLNet_GetError() << std::endl;
            perror("SDLNet_CheckSockets");
            exit(EXIT_FAILURE);
        } else if( socketsReady_ != 0 ){
            //  Connection request recieved.
            std::auto_ptr<inp::Connection> newCon = listenConnection_.accept();
            if( newCon.get() == NULL ){
                std::cerr << __FILE__ << __LINE__ << "Failed to accept" << std::endl;
                return true;
            }
            #ifdef DEBUG
            server_.sendConsole("New Connection");
            #endif
            loginQueue_.push(newCon.release());
        }
    }

    // process queue
    while( !loginQueue_.empty() ){
        //check version
        if( validateInpVer(loginQueue_.front()) ){
            // log player in
            server_.loginPlayer( loginQueue_.front() );
        } else {
            delete loginQueue_.front();
        }
        loginQueue_.pop();
    }


    return true;
}

            //  users DOESNOT EXISTS
        /*if( users.size() > GameConfig::maxConnections ){
            //server is full
            INFPacket packet;
            packet << DataTypeByte::SERVER_MSG << "Infraelly Server is full, please try again later.";
            newCon->send(packet);
            //auto_ptr will auto destroy newCon
        } else {*/


bool ConnectionListenerThread::validateInpVer(inp::Connection* newCon){
    #ifdef DEBUG
    server_.sendConsole("Validate version");
    #endif
    //make sure client is using a recent version of the INP
    //wait for user to send us their ver (1 secs)
    inp::INFPacket netVerPack;
    int rVal = newCon->recvWait(netVerPack, 1000);
    if( rVal == 0 ){
        //waited too long, tell user timeout
        netVerPack.clear();
        netVerPack << inp::DataTypeByte::CONNECT_DENY << "Connection Timeout.";
        newCon->send(netVerPack);
        newCon->disconnect();
        return false;
    } else
    if( rVal == -1 ){
        //an error occured, make sure dc ,just ignore
        newCon->disconnect();
        return false;
    }

    //something was recived
    inp::NetCode code;
    netVerPack >> code;
    if( code == inp::DataTypeByte::INP_VER ){
        //check ver
        std::string ver;
        netVerPack >> ver;
        if( ver != inp::inp_ver ){
            //old ver
            netVerPack.clear();
            netVerPack << inp::DataTypeByte::CONNECT_DENY << "Please update client.";
            newCon->send(netVerPack);
            newCon->disconnect();
            return false;
        }
    } else {
        //some junk was sent. dc player
        netVerPack.clear();
        netVerPack << inp::DataTypeByte::CONNECT_DENY << "Please update client.";
        newCon->send(netVerPack);
        newCon->disconnect();
        return false;
    }


    //correct version
    netVerPack.clear();
    netVerPack << inp::DataTypeByte::CONNECT_SUCCESS;
    newCon->send(netVerPack);
    return true;
}
