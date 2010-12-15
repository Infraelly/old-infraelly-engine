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


#include "Connection.hpp"
#include "ConnectionImpl.hpp"

#include <iostream>
#include <SDL/SDL_net.h>
#include "globalFunc.hpp" //isSpace;

#include "INFPacket.hpp"

namespace inp{

    Connection::Connection() : pimpl_(new ConnectionImpl){
        ++pimpl_->refCount;
    }


    Connection::Connection(TCPsocket& newUserSocket) : pimpl_(new ConnectionImpl){
        ++pimpl_->refCount;
        pimpl_->userSocket = newUserSocket;
        makeSet(1);
        addSocketToSet();
    }


    Connection::Connection(SDLNet_SocketSet& newGroup, TCPsocket& newUserSocket) : pimpl_(new ConnectionImpl){
        ++pimpl_->refCount;
        pimpl_->group = newGroup;
        pimpl_->userSocket = newUserSocket;
        addSocketToSet();
    }

    Connection::Connection(const Connection& src){
        SDL_LockMutex(src.pimpl_->dataAccess);
            pimpl_ = src.pimpl_;
            ++pimpl_->refCount;
        SDL_UnlockMutex(src.pimpl_->dataAccess);
    }

    Connection& Connection::operator=(Connection& rhs){
        if( &rhs != this ){
            SDL_LockMutex(rhs.pimpl_->dataAccess);
                clean();
                pimpl_ = rhs.pimpl_;
                ++pimpl_->refCount;
            SDL_UnlockMutex(rhs.pimpl_->dataAccess);
        }
        return *this;
    }

    Connection::~Connection(){
        clean();
    }

    void Connection::clean(){
        SDL_LockMutex(pimpl_->dataAccess);
        --pimpl_->refCount;
        if( pimpl_->refCount == 0 ){
            if( pimpl_->group != NULL ){
                SDLNet_TCP_DelSocket(pimpl_->group, pimpl_->userSocket);
                if( pimpl_->createdSet ){
                    SDLNet_FreeSocketSet(pimpl_->group);
                }
                pimpl_->group = NULL;
                pimpl_->createdSet = 0;
            }
            if( pimpl_->userSocket != NULL ){
                SDLNet_TCP_Close( pimpl_->userSocket );
                pimpl_->userSocket = NULL;
            }
            pimpl_->peer = NULL;
            pimpl_->active = false;
            pimpl_->connectTime = 0;
            delete pimpl_;
        } else {
            SDL_UnlockMutex(pimpl_->dataAccess);
        }
        pimpl_ = NULL;
    }








    int Connection::init(IPaddress *ip){
        if( connect(ip) == -1 ){ return -1; }
        makeSet(1);
        addSocketToSet();
        return 1;
    }

    int Connection::init(const std::string& host, int port){
        if( connect(host, port) == -1 ){ return -1; }
        makeSet(1);
        addSocketToSet();
        return 1;
    }

    int Connection::initServer(int port){
        IPaddress ip;
        if( SDLNet_ResolveHost(&ip, NULL, port)== -1 ) {
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
            return -1;
        }
        if( connect(&ip) == -1 ){ return -1; }
        makeSet(1);
        addSocketToSet();
        return 1;
    }





    int Connection::connect(){
        SDL_LockMutex(pimpl_->dataAccess);
            //make sure is dissconected
            if( pimpl_->userSocket != NULL ){
                SDLNet_TCP_Close( pimpl_->userSocket );
                pimpl_->userSocket = NULL;
            }
            pimpl_->active = false;
            pimpl_->connectTime = 0;
            //  connect to peer ( if there is one )
            if( pimpl_->peer != NULL ){
                pimpl_->userSocket = SDLNet_TCP_Open(pimpl_->peer);
                if( pimpl_->userSocket == NULL ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
                    pimpl_->active = false;
                    pimpl_->connectTime = 0;
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return -1;
                }
                pimpl_->active = true;
                pimpl_->connectTime = SDL_GetTicks();
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return 1;
    }

    int Connection::connect(IPaddress *ip){
        SDL_LockMutex(pimpl_->dataAccess);
            //make sure is dissconected
            if( pimpl_->userSocket != NULL ){
                SDLNet_TCP_Close( pimpl_->userSocket );
                pimpl_->userSocket = NULL;
            }
            pimpl_->active = false;
            pimpl_->connectTime = 0;
            // connect to ip (if one is stored)
            if( ip != NULL ){
                pimpl_->userSocket = SDLNet_TCP_Open(ip);
                if( pimpl_->userSocket == NULL ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
                    pimpl_->active = false;
                    pimpl_->connectTime = 0;
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return -1;
                }
                pimpl_->peer = SDLNet_TCP_GetPeerAddress(pimpl_->userSocket);
                pimpl_->active = true;
                pimpl_->connectTime = SDL_GetTicks();
                SDL_UnlockMutex(pimpl_->dataAccess);
                return 1;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return -11;
    }

    int Connection::connect(std::string host, int port){
        SDL_LockMutex(pimpl_->dataAccess);
            //make sure is dissconected
            if( pimpl_->userSocket != NULL ){
                SDLNet_TCP_Close( pimpl_->userSocket );
                pimpl_->userSocket = NULL;
            }
            pimpl_->active = false;
            pimpl_->connectTime = 0;
            if( (port != 0) && (!isSpace(host)) ){
                IPaddress ip;
                if( SDLNet_ResolveHost(&ip, host.c_str(), port) == -1 ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
                    pimpl_->active = false;
                    pimpl_->connectTime = 0;
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return -1;
                }
                pimpl_->userSocket = SDLNet_TCP_Open(&ip);
                if( pimpl_->userSocket == NULL ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
                    pimpl_->active = false;
                    pimpl_->connectTime = 0;
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return -1;
                }
                pimpl_->peer = SDLNet_TCP_GetPeerAddress(pimpl_->userSocket);
                pimpl_->active = true;
                pimpl_->connectTime = SDL_GetTicks();
                SDL_UnlockMutex(pimpl_->dataAccess);
                return 1;
            } else {
                std::cerr << __FILE__ << " " << __LINE__ << ": " << "Connection::connect(string, int): Port can't be 0. Host can't be whitespace." << std::endl;
                pimpl_->active = false;
                pimpl_->connectTime = 0;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return 1;
    }

    void Connection::disconnect(){
        SDL_LockMutex(pimpl_->dataAccess);
            pimpl_->peer = NULL;
            pimpl_->active = false;
            pimpl_->connectTime = 0;
            if( pimpl_->userSocket != NULL ){
                SDLNet_TCP_Close( pimpl_->userSocket );
                pimpl_->userSocket = NULL;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
    }





    int Connection::checkSet(int timeout){
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->active ){
                if( pimpl_->group != NULL ){
                    int num = SDLNet_CheckSockets(pimpl_->group, timeout);
                    if( num != -1 ){
                        pimpl_->active = true;
                        SDL_UnlockMutex(pimpl_->dataAccess);
                        return num;
                    }
                }
                pimpl_->active = false;
                pimpl_->connectTime = 0;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return -1;
    }

    int Connection::checkSocket(){
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->active ){
                if( (pimpl_->group != NULL) && (pimpl_->userSocket != NULL) ){
                    if( SDLNet_CheckSockets(pimpl_->group, 0) != -1 ){
                        if( SDLNet_SocketReady(pimpl_->userSocket) ){
                            pimpl_->active = true;
                            SDL_UnlockMutex(pimpl_->dataAccess);
                            return 1;
                        }
                    }
                }
                pimpl_->active = false;
                pimpl_->connectTime = 0;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return -1;
    }



    std::auto_ptr<Connection> Connection::accept(){
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->userSocket == NULL){
                SDL_UnlockMutex(pimpl_->dataAccess);
                std::auto_ptr<Connection> r(NULL);
            }
            TCPsocket sock = SDLNet_TCP_Accept(pimpl_->userSocket);
            if( sock == NULL ){
                std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_TCP_Accept: " << SDLNet_GetError() << std::endl;
                SDL_UnlockMutex(pimpl_->dataAccess);
                std::auto_ptr<Connection> r(NULL);
            }
            std::auto_ptr<Connection> newConnection( new Connection(sock) );
        SDL_UnlockMutex(pimpl_->dataAccess);
        return newConnection;
    }










    int Connection::send(const inp::INFPacket& data){
        //  prepare packet for sending
        PacketList packList;
        splitForSend(data, packList);
        // if there is nothing to send, leave.
        if( packList.empty() ){ return 1; }

        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->userSocket != NULL ){
                //  Send packets in list
                for(size_t i = 0; i < packList.size(); ++i ){
                    if( !packList[i].empty() ){
                        if( SDLNet_TCP_Send( pimpl_->userSocket,
                                             &packList[i][0],
                                             packList[i].size()   ) <
                            packList[i].size() )
                        {
                            if( pimpl_->userSocket != NULL ){
                                SDLNet_TCP_Close( pimpl_->userSocket );
                                pimpl_->userSocket = NULL;
                            }
                            pimpl_->peer = NULL;
                            pimpl_->active = false;
                            pimpl_->connectTime = 0;
                            pimpl_->lastSent.clear();
                            SDL_UnlockMutex(pimpl_->dataAccess);
                            return -1;
                        } else {
                            pimpl_->active = true;

                        }
                    }
                }
                pimpl_->lastSent = packList;
            } else {
                pimpl_->active = false;
                pimpl_->connectTime = 0;
                SDL_UnlockMutex(pimpl_->dataAccess);
                return 1;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return 1;
    }




    //recieve a single byte
    int Connection::recvOne(Uint8* data, Uint32 ms,  bool lockMutex){
        if( lockMutex ){ SDL_LockMutex(pimpl_->dataAccess); }

        if( (pimpl_->group != NULL) && (pimpl_->userSocket != NULL) ){
            Uint32 startTime = SDL_GetTicks();
            int numReady;

            while( (SDL_GetTicks()-startTime) < ms ){
                numReady = SDLNet_CheckSockets(pimpl_->group, 1);
                if( numReady == -1 ){ break; }
                if( numReady == 1 ){
                    if( SDLNet_SocketReady(pimpl_->userSocket) ){
                        if( SDLNet_TCP_Recv(pimpl_->userSocket, data, 1) <= 0 ) {
                            if( pimpl_->userSocket != NULL ){
                                SDLNet_TCP_Close( pimpl_->userSocket );
                                pimpl_->userSocket = NULL;
                            }
                            pimpl_->peer = NULL;
                            pimpl_->active = false;
                            pimpl_->connectTime = 0;
                            std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_TCP_Recv(): "<< SDLNet_GetError() << std::endl;
                            if( lockMutex ){ SDL_UnlockMutex(pimpl_->dataAccess); }
                            return -1;
                        } else {
                            pimpl_->active = true;
                            if( lockMutex ){ SDL_UnlockMutex(pimpl_->dataAccess); }
                            return 1;
                        }
                    }
                }
            }
        }

        pimpl_->active = false;
        pimpl_->connectTime = 0;
        if( lockMutex ){ SDL_UnlockMutex(pimpl_->dataAccess); }
        return -1;
    }



    int Connection::recv(inp::INFPacket& dest){
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->userSocket != NULL ){
                //make sure thers something to recieved
                if( (pimpl_->group != NULL) && (pimpl_->userSocket != NULL) ){
                    if( SDLNet_CheckSockets(pimpl_->group, 0) != -1 ){
                        if( SDLNet_SocketReady(pimpl_->userSocket) ){
                            pimpl_->active = true;
                        }
                    }
                } else {
                    pimpl_->active = false;
                    pimpl_->connectTime = 0;
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return 0;
                }

                dest.clear();
                Uint8 temp = 0;
                Uint8 len_buf[2];
                Uint16 len = 0;
                bool recieving = true;

                //get header byte
                if( recvOne( &temp, 1000, false ) == -1 ){
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return -1;
                }

                //  check if the header byte is a start packet
                if( temp != ControlByte::START  ){
                    SDL_UnlockMutex(pimpl_->dataAccess);
                    return 0;
                }

                //begin getting body of data
                while( recieving ){
                    //  This should be 2 bytes stating length of segment
                    for(unsigned i = 0; i < 2; ++i){
                        if( recvOne( &len_buf[i], 1000, false ) == -1 ){
                            SDL_UnlockMutex(pimpl_->dataAccess);
                            return -1;
                        }
                    }
                    len  = SDLNet_Read16(len_buf);

                    //try to get "len" amount of bytes
                    for(unsigned i = 0; i < len; ++i){
                        if( recvOne( &temp, 1000, false ) == -1 ){
                            SDL_UnlockMutex(pimpl_->dataAccess);
                            return -1;
                        } else {
                            dest.data.push_back(temp);
                        }
                    }

                    //  This shud be a wait_more/end
                    if( recvOne( &temp, 1000, false ) == -1 ){
                        SDL_UnlockMutex(pimpl_->dataAccess);
                        return -1;
                    }
                    if( (temp == ControlByte::CONTINUE ) ||
                        (temp == ControlByte::WAIT_MORE )  )
                    {
                        continue;
                    } else
                    // Slab already started.
                    //   this is a malformed transmission
                    if( (temp == ControlByte::START) ||
                        (temp == ControlByte::END)  )  //stop if this is the end
                    {
                        recieving = false;
                    } else {
                        // some bull shit happened here =/
                        recieving = false;
                    }
                }
                pimpl_->lastRecv = dest;
            } else { // pimpl_->userSocket != NULL
                pimpl_->active = false;
                pimpl_->connectTime = 0;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return 1;
    }

    int Connection::recvWait(inp::INFPacket& dest, Uint32 ms){
        SDL_LockMutex(pimpl_->dataAccess);
            if( (pimpl_->group != NULL) && (pimpl_->userSocket != NULL) ){
                Uint32 startTime = SDL_GetTicks();
                int numReady;
                while( (SDL_GetTicks()-startTime) < ms ){
                    numReady = SDLNet_CheckSockets(pimpl_->group, 0);
                    if( numReady != -1 ){
                        if( SDLNet_SocketReady(pimpl_->userSocket) ){
                            pimpl_->active = true;
                            SDL_UnlockMutex(pimpl_->dataAccess);
                            return recv(dest);
                        }
                    } else {
                        pimpl_->active = false;
                        pimpl_->connectTime = 0;
                        break;
                    }
                }
            } else {
                pimpl_->active = false;
                pimpl_->connectTime = 0;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return -1;
    }









    SDLNet_SocketSet Connection::getSocketSet()const{
        SDL_LockMutex(pimpl_->dataAccess);
            SDLNet_SocketSet tmp = pimpl_->group;
        SDL_UnlockMutex(pimpl_->dataAccess);
        return tmp;
    }

    TCPsocket Connection::getSocket()const{
        SDL_LockMutex(pimpl_->dataAccess);
            TCPsocket tmp = pimpl_->userSocket;
        SDL_UnlockMutex(pimpl_->dataAccess);
        return tmp;
    }

    IPaddress Connection::getPeer()const{
        SDL_LockMutex(pimpl_->dataAccess);
            IPaddress tmp = *pimpl_->peer;
        SDL_UnlockMutex(pimpl_->dataAccess);
        return tmp;
    }

    bool Connection::isActive()const{
        SDL_LockMutex(pimpl_->dataAccess);
            bool tmp = pimpl_->active;
        SDL_UnlockMutex(pimpl_->dataAccess);
        return tmp;
    }





    Uint32 Connection::getConnectTime()const{
        SDL_LockMutex(pimpl_->dataAccess);
            Uint32 tmp = pimpl_->connectTime;
        SDL_UnlockMutex(pimpl_->dataAccess);
        return tmp;
    }

    Uint32 Connection::getTimeConnected()const{
        Uint32 rVal;
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->connectTime == 0 ){
                rVal = 0;
            } else {
                rVal = SDL_GetTicks() - pimpl_->connectTime;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
        return rVal;
    }






    void Connection::setSocketSet(const SDLNet_SocketSet& newGroup){
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->createdSet ){
                SDLNet_FreeSocketSet(pimpl_->group);
                pimpl_->group = NULL;
                pimpl_->createdSet = false;
            }
            pimpl_->group = newGroup;
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    void Connection::setSocket(const TCPsocket& newUserSocket){
        SDL_LockMutex(pimpl_->dataAccess);
            pimpl_->userSocket = newUserSocket;
            pimpl_->peer = SDLNet_TCP_GetPeerAddress(pimpl_->userSocket);
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    void Connection::setPeer(IPaddress* newPeer){
        SDL_LockMutex(pimpl_->dataAccess);
            pimpl_->peer = newPeer;
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    void Connection::setActive(bool value){
        SDL_LockMutex(pimpl_->dataAccess);
            pimpl_->active = value;
        SDL_UnlockMutex(pimpl_->dataAccess);
    }







    void Connection::makeSet(int max){
        SDL_LockMutex(pimpl_->dataAccess);
            if( pimpl_->group == NULL ){
                if( pimpl_->createdSet ){
                    SDLNet_FreeSocketSet(pimpl_->group);
                    pimpl_->group = NULL;
                }
                pimpl_->group = SDLNet_AllocSocketSet(max);
                if( pimpl_->group == NULL ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_AllocSocketSet(" << max << "): " << SDLNet_GetError() << std::endl;
                    exit(EXIT_FAILURE);
                }
                pimpl_->createdSet = 1;
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    void Connection::addSocketToSet(){
        SDL_LockMutex(pimpl_->dataAccess);
            if( (pimpl_->group != NULL) && (pimpl_->userSocket != NULL) ){
                SDLNet_TCP_AddSocket(pimpl_->group, pimpl_->userSocket);
            }
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    void Connection::cleanSet(){
        SDL_LockMutex(pimpl_->dataAccess);
            SDLNet_TCP_DelSocket(pimpl_->group, pimpl_->userSocket);
            SDLNet_FreeSocketSet(pimpl_->group);
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    void Connection::removeSocketFromSet(){
        SDL_LockMutex(pimpl_->dataAccess);
            SDLNet_TCP_DelSocket(pimpl_->group, pimpl_->userSocket);
        SDL_UnlockMutex(pimpl_->dataAccess);
    }





    void Connection::setId(const std::string& newId){
        SDL_LockMutex(pimpl_->dataAccess);
            pimpl_->id = newId;
        SDL_UnlockMutex(pimpl_->dataAccess);
    }

    std::string Connection::getId(){
        std::string rVal;
        SDL_LockMutex(pimpl_->dataAccess);
            rVal = pimpl_->id;
        SDL_UnlockMutex(pimpl_->dataAccess);
        return rVal;
    }





    void splitForSend(const INFPacket& src, PacketList& destList){
        //clear out queue
        destList.clear();

        //segments
        std::vector<Uint8> tmp;
        tmp.reserve(maxSendSize);

        size_t dataIndex = 0;
        Uint16 len = 0;
        Uint8 len_buff[2];
        unsigned int remaining = 0;
        while( dataIndex < src.data.size() ){
            //  Packet header
            if( dataIndex == 0 ){
                tmp.push_back(ControlByte::START.getVal() );
            } else {
                tmp.push_back(ControlByte::CONTINUE.getVal() );
            }

            //  data
            //minus 2 from maxSendSize for the START/END
            //minus 2 again, becuase 2 bytes are used to store the length
            //  of this segment

            //how much of the input data is left to be packed
            remaining = src.data.size() - dataIndex;
            //the length of the next segment
            if( remaining > (maxSendSize-4) ){
                len = (maxSendSize-4);
            } else {
                len = remaining;
            }
            //insert the length of this segment
            SDLNet_Write16(len, len_buff);
            tmp.push_back( len_buff[0] );
            tmp.push_back( len_buff[1] );

            //write the actual data
            for(int i = 0; i < len; ++i){
                tmp.push_back( src.data[dataIndex] );
                ++dataIndex;
            }

            //  Packet footer
            if( dataIndex == src.getLength() ){
                tmp.push_back(ControlByte::END.getVal() );
            } else {
                tmp.push_back(ControlByte::WAIT_MORE.getVal() );
            }

            //  Put the packet on the list
            destList.push_back(tmp);
            tmp.clear();
        }
    }

}
