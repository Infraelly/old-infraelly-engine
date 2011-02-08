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


#include "ServerContext.hpp"

#include <sstream>
#include <utility>
#include <cmath>
#include <SDL/SDL_net.h>

#include "globalFunc.hpp"
#include "GameConfig.hpp"
#include "MutexLocker.hpp"
#include "Character.hpp"
#include "NetProtocol.hpp"

using namespace std;
using namespace inp;


void ServerContext::ComLineCallBack::operator()(const std::string& text){
    if( ob == NULL){
        cerr << __FILE__ << __LINE__ << "console call back, bad args" << endl;
        return;
    }
    ob->commandLine(text);
}


ServerContext::ServerContext() :
    worldAccess_( SDL_CreateMutex() ),
    consoleAccess_( SDL_CreateMutex() ),
    console_( new Console(screen) ),
    timerAccess_( SDL_CreateMutex() ),
    listenerThread_( *this ),
    comLineCaller(this)
{
    // Set window caption
    SDL_WM_SetCaption("Infraelly Nucleus (Server)", NULL);
    fpsManager.setMaxFps(10);

    //set call back for console
    console_->setInputCallBack(&comLineCaller);

    //  Deterine byte order and ip
    //  we should contact an internet site for this. (libcurl time?)
    {
        std::ostringstream host;
        IPaddress ip;
        SDLNet_ResolveHost( &ip, "localhost", GameConfig::serverPort );

        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            sendConsole("This computer uses: Big Endian a.k.a Network Byte Order (MSB)");
            host << ((ip.host >> 24)&0xFF) << "." << ((ip.host >> 16)&0xFF) << "." << ((ip.host >> 8)&0xFF) << "." << (ip.host&0xFF);
        #else
            sendConsole("This computer uses: Little Endian (LSB)");
            host << (ip.host&0xFF) << "." << ((ip.host >> 8)&0xFF) << "." << ((ip.host >> 16)&0xFF) << "." << ((ip.host >> 24)&0xFF);
        #endif

        sendConsole("Your IP is: "+host.str());
    }

    // Load all maps
    // for now just the null map atm (dummy map) =[
    GameArea *nullMap = new GameArea(this);
    world_["NULL"] = nullMap;

    //calculate how many maps will be allocated to each worker thread, roudned up
    int areasPerThread = ceil(world_.size()/(float)GameConfig::serverThreads);

    //  Create a thread to manage each area
    GameAreaContainerIterator startItr = world_.begin();
    for( int i = 0; (i<GameConfig::serverThreads)&&(startItr != world_.end()); ++i ){
        std::vector<GameArea*> gaList;
        for( int j = 0; (startItr != world_.end())&&(j<areasPerThread); ++j){
            gaList.push_back( startItr->second );
            ++startItr;
        }
        threads_.push_back( new GameZoneThread(gaList) );
    }

    //all Thread objects created, create the execution threads and start execution
    for( size_t i = 0; i < threads_.size(); ++i ){
        threads_.at(i)->create();
        threads_.at(i)->start();
    }

    //  Open connection
    //  Create listening proccess
    listenerThread_.create();
    listenerThread_.start();

    //start the server up time timer
    timerAccess_ = SDL_CreateMutex();
    serverUpTimer_.start();

    run();
}

ServerContext::~ServerContext(){
    sendConsole("Infraelly Server is shutting down");
    SDL_LockMutex(consoleAccess_);
    console_->logic();
    console_->draw();
    SDL_UnlockMutex(consoleAccess_);
    SDL_Flip(screen);

    //  Stop listening for connetcions
    listenerThread_.end();
    listenerThread_.waitFinish();


    inp::INFPacket packet;
    for( int t = 5; t != 0; --t ){
        packet.clear();
        packet << inp::DataTypeByte::SERVER_MSG << ("Server Shutdown in " + itos(t) + "...");
        for(size_t i = 0; i < threads_.size(); ++i){
            threads_.at(i)->sendAll(packet);
        }
        sendConsole("Server Shutdown in " + itos(t) + "...");
        SDL_LockMutex(consoleAccess_);
        console_->logic();
        console_->draw();
        SDL_UnlockMutex(consoleAccess_);
        SDL_Flip(screen);
        SDL_Delay(1000);
    }

    //Tell threads to stop
    for( size_t i = 0; i < threads_.size(); ++i ){
        threads_.at(i)->end();
    }
    // wait for threads to stop
    for( size_t i = 0; i < threads_.size(); ++i ){
        threads_.at(i)->waitFinish();
    }
    //  destroy threads
    for( size_t i = 0; i < threads_.size(); ++i ){
        delete threads_.at(i);
        threads_.at(i) = NULL;
    }

    //  Discconect all players
    //
    SDL_LockMutex(worldAccess_);
    for( std::map<std::string, GameArea*>::iterator itr = world_.begin();
         itr != world_.end();    ++itr  ){
        GameArea *temp = itr->second;
        delete temp;
        temp = NULL;
    }

    //destroy mutexes
    SDL_DestroyMutex(worldAccess_);

    SDL_LockMutex(consoleAccess_);
    SDL_DestroyMutex(consoleAccess_);

    SDL_LockMutex(timerAccess_);
    SDL_DestroyMutex(timerAccess_);
}


void ServerContext::events(SDL_Event &event){
    if( (event.type == SDL_VIDEORESIZE) || (event.key.keysym.sym == SDLK_F4)  ){
        console_->resetPositions();
    }
    console_->pushInput(event);
}

void ServerContext::input(Uint8 *keys){}

void ServerContext::logic(){
    //see if the threads have any msgs to say
    if( listenerThread_.outDataReady() ){
        INFPacket pack;
        listenerThread_.getOutData(pack);
        NetCode code;
        pack >> code;
        if( code == DataTypeByte::THREAD_SEND_CONSOLE ){
            string text;
            pack >> text;
            sendConsole(text);
        }
    }

    //see if the threads have any msgs to say
    for( size_t i = 0; i < threads_.size(); ++i ){
        if( threads_.at(i)->outDataReady() ){
            INFPacket pack;
            threads_.at(i)->getOutData(pack);
            NetCode code;
            pack >> code;
            if( code == DataTypeByte::THREAD_SEND_CONSOLE ){
                string text;
                pack >> text;
                sendConsole(text);
            }
        }
    }

    SDL_LockMutex(consoleAccess_);
    console_->logic();
    SDL_UnlockMutex(consoleAccess_);
}

void ServerContext::draw(){
    SDL_LockMutex(consoleAccess_);
    console_->draw();
    SDL_UnlockMutex(consoleAccess_);
}









//  Sends some text to the console
void ServerContext::sendConsole(const std::string& text){
    SDL_LockMutex(consoleAccess_);
    if( console_.get() != NULL ){
        console_->push_back(text);
    }
    SDL_UnlockMutex(consoleAccess_);
}

//  Sends to all players
void ServerContext::sendAll(const INFPacket& pack){
    MutexLocker lock(worldAccess_);
    map<string, GameArea*>::iterator itr;
    for( itr = world_.begin(); itr != world_.end(); ++itr ){
        itr->second->sendAll(pack);
    }
}

// Sends to specific user
//  returns false if action failed
bool ServerContext::sendTo(const string& username, const INFPacket& pack){
    MutexLocker lock(worldAccess_);
    std::map<std::string, GameArea*>::iterator itr;
    for( itr = world_.begin(); itr != world_.end(); ++itr ){
        inp::Connection *con = itr->second->getConnection(username);
        if( con != NULL ){
            return con->send(pack);
        }
    }
    return false;
}

//  Total players
unsigned int ServerContext::totalOnline(){
    unsigned int online = 0;
    map<string, GameArea*>::iterator itr;
    MutexLocker lock(worldAccess_);
    for( itr = world_.begin(); itr != world_.end(); ++itr ){
        online += itr->second->getNumberConnections();
    }
    return online;
}








int ServerContext::loginPlayer(Connection *con){
    #ifdef DEBUG
    sendConsole("Logging In");
    #endif

    INFPacket packet;
    NetCode code;
    std::string username = "";

    //***************************************
    //             verification
    //***************************************
    //  wait 4 seconds for a response
    if( con->recvWait(packet, 4000) != -1 ){
        std::string password;

        //get username
        packet >> code;
        if( code != DataTypeByte::USERNAME ){
            con->disconnect();
            delete con;
            con = NULL;
            return 0;
        }
        packet >> username;
        if( username == "" ){
            con->disconnect();
            delete con;
            con = NULL;
            return 0;
        }

        //////////////////////////////////////
        //  Do username lookup in database  //
        //////////////////////////////////////

        //get password
        packet >> code;
        if( code != DataTypeByte::PASS ){
            con->disconnect();
            delete con;
            con = NULL;
            return 0;
        }
        packet >> password;
        if( username == "" ){
            con->disconnect();
            delete con;
            con = NULL;
            return 0;
        }

        //////////////////////////////////////
        //  Do password lookup in database  //
        //////////////////////////////////////

        con->setId(username);
    } else {
        con->disconnect();
        delete con;
        con = NULL;
        return 0;
    }

    //  Connection is ready
    //  Send welcome
    packet.clear();
    packet << DataTypeByte::SERVER_MSG << "Welcome to the Infraelly Server";
    con->send(packet);
    sendConsole("User joined: "+username);

    //add self to room (null map)
    //add connection
    SDL_LockMutex(worldAccess_);
    world_["NULL"]->addPlayer( std::make_pair(con, new Character) );
    SDL_UnlockMutex(worldAccess_);

    return 1;
}


void ServerContext::commandLine(std::string commandString){
    //strip whitespace off front and back
    trimBack(commandString);
    trimFront(commandString);

    //  Check if text is a command
    if( (commandString.at(0) == '/') && (commandString.length() > 1 ) ){
        //store the pos of end of string
        string::size_type end = commandString.length();

        string::size_type i = 1;
        std::string temp = "";
        std::string command = "";
        std::vector<std::string> arguments;
        //extract the command
        while( i < end ){
            //check if next character is whitespace
            if( isspace(commandString.at(i)) ){
                //skip the whitespace
                while( isspace(commandString.at(++i)) ){}
                break;
            }
            command += toupper(commandString.at(i));
            ++i;
        }
        //extract the arguments
        while( i < end ){
            //check if next character is whitespace
            if( isspace(commandString.at(i)) ){
                arguments.push_back(temp);
                temp = "";
                //skip the whitespace
                while( isspace(commandString.at(++i)) );
                continue;
            }
            temp += commandString.at(i);
            ++i;

            //last argument
            if( (i == end) && (temp != "") ){
                arguments.push_back(temp);
            }
        }




        //  COMMAND IMPLEMETATION
        if( (command == "SAY") || (command == "S") ){
            if( arguments.empty() ){
                sendConsole("Not enough arguments: /s MSG");
            } else {
                string message(commandString, command.length()+2, commandString.length()-command.length()-2 );
                INFPacket packet;
                packet << DataTypeByte::SERVER_MSG << message;
                sendAll(packet);
            }
        } else
        if( (command == "WHISPER") || (command == "W") ){
            if( arguments.size() < 2 ){
                sendConsole("Not enough arguments: /w USER MSG");
            } else {
                int totalComLen = command.length()+arguments.at(0).size();
                string message(commandString, totalComLen+2, commandString.length()-totalComLen );
                INFPacket packet;
                packet << DataTypeByte::SERVER_MSG << message;
                if( !sendTo(arguments.at(0), packet) ){
                    sendConsole("Send failed.");
                }
             }
        } else
        if( (command == "KICK") || (command == "K") ){
            if( arguments.size() < 2 ){
                sendConsole("Not enough arguments: /k USER MSG");
            } else {
                int totalComLen = command.length()+arguments.at(0).size();
                string reason(commandString, totalComLen+2, commandString.length()-totalComLen );
                INFPacket packet;
                packet << DataTypeByte::KICK << reason;
                if( !sendTo(arguments.at(0), packet) ){
                    sendConsole("Send failed.");
                }
            }
        } else
        if( (command == "PING") || (command == "P") ){
            if( arguments.empty() ){
                sendConsole("Not enough arguments: /p USER");
            } else {
                INFPacket packet;
                packet << DataTypeByte::PING_REQ;
                if( !sendTo(arguments.at(0), packet) ){
                    sendConsole("Send failed.");
                }
             }
        } else
        if( command == "USERS" ){
            sendConsole( "Users online: " + itos(totalOnline()) );
        } else
        if( command == "USERSMAX" ){
            sendConsole("not implemented yet");
            //sendConsole( "Peak users online: " + users.sizePeak() );
        } else
        if( command == "UPTIME" ){
            SDL_LockMutex(timerAccess_);
            Uint32 time = serverUpTimer_.getTime();
            SDL_UnlockMutex(timerAccess_);
            if( time == 0 ){
                sendConsole( "Uptime: N/A" );
            } else {
                ostringstream uptime_str;
                uptime_str << "Uptime: ";
                if( time > 60000 ){
                    uptime_str << time/60000 << " minnutes ";
                    time %= 60000;
                }
                if( time > 1000 ){
                    uptime_str << time/1000 << " seconds ";
                    time %= 1000;
                }
                uptime_str << time << " ms.";

                sendConsole(uptime_str.str());
            }
        } else
        if( command == "CLEAR" ){
            SDL_LockMutex(consoleAccess_);
            console_->clear();
            SDL_UnlockMutex(consoleAccess_);
        } else
        if( (command == "HELP") || (command == "H") ){
           sendConsole(" ");
           sendConsole("------------------------------");
           sendConsole("Infraelly Server commands");
           sendConsole("------------------------------");
           sendConsole("Commands are not case sensetive");
           sendConsole("Brackets indicate short-hand command");
           sendConsole("eg. /(s)ay means: /say and /s do the same thing");
           sendConsole(" ");
           sendConsole("Send to all users: /(s)ay MSG");
           sendConsole("Send to one user: /(w)hisper USER MSG");
           sendConsole("Kick a user: /(k)ick USER REASON");
           sendConsole("Ping a user: /(p)ing USER");
           sendConsole("Number of players online: /users");
           sendConsole("Max number of players online since server start: /usersMax");
           sendConsole("Max number of players online since server start: /usersMax");
           sendConsole("Server uptime: /uptime");
           sendConsole("Commands list: /(h)elp");
           sendConsole("Clear console: /clear");
        } else
        if( (command == "WOOT") || (command == "W00T") ){
            sendConsole("w00t, you found an easter egg!");
        } else {
            sendConsole( "Invalid command. Try /help" );
        }
    }
}
