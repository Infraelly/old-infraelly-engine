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


#include "GameArea.hpp"

#include <iostream>
#include <string>
#include <utility>

#include "MutexLocker.hpp"
#include "Connection.hpp"
#include "Character.hpp"
#include "INFPacket.hpp"
#include "ServerContext.hpp"

using namespace inp;


GameArea::GameArea(ServerContext *server) :
    access_(SDL_CreateMutex()),
    server_(server)
{
    syncTimer.start();
    drop =0;
};
GameArea::GameArea(ServerContext *server, const std::string& mapFilename) :
    access_(SDL_CreateMutex()),
    server_(server)
{
    map_.loadMap(mapFilename);
    syncTimer.start();
    drop =0;
};
GameArea::~GameArea(){
    SDL_LockMutex(access_);
    SDL_DestroyMutex(access_);
};


Character GameArea::getPlayer(const std::string& id)const{
    ScopedMutexLock(access_);
    Character rVal;
    if( players_.find(id) != players_.end() ){
        rVal = *players_.find(id)->second;
    }
    return rVal;
}

inp::Connection *GameArea::getConnection(const std::string& id)const{
    return connections_.getConnection(id);
}

void GameArea::addPlayer(const CharCon& playerCon){
    if( (playerCon.first == NULL) || (playerCon.second == NULL) ){ return; }
    inp::Connection *con = playerCon.first;
    Character *player = playerCon.second;
//--------------------------------------
// TODO: Look up character from db and load
//------------------------------------
    player->setName(con->getId());
    player->setX( 300 );
    player->setY( 300 );

    // send character details to player
    inp::INFPacket infoPack;
    infoPack = buildSyncPacket(playerCon.second, playerCon.first);
    con->send(infoPack);


    ScopedMutexLock(access_);
    if( players_.size() > 0 ){
        INFPacket joinPack;
        //  Get all players in the current room
        for( size_t i = 0; i < connections_.size(); ++i ){
           joinPack << DataTypeByte::USER_JOIN;
           joinPack << connections_.conAt(i)->getId();
           joinPack << buildSyncPacket( players_[connections_.conAt(i)->getId()],
                                        connections_.conAt(i) );
        }
        con->send(joinPack);

        //tell other players we just joined
        joinPack.clear();
        joinPack << DataTypeByte::USER_JOIN << con->getId();
        joinPack << buildSyncPacket( player, con );
        connections_.sendAll(joinPack);
    }

    // add to us containers
    // this is done at the end to reduce time mutex is locked for
    connections_.addConnection( con->getId(), con );
    players_[con->getId()] = player;
}

CharCon GameArea::removeCharCon(const std::string& id){
    CharCon rVal;
    rVal.first = connections_.getConnection(id);
    ScopedMutexLock(access_);
    if( (rVal.first != NULL) && (players_.find(id) != players_.end()) ){
        rVal.second = players_.find(id)->second;
    } else {
        rVal.second = NULL;
    }
    return rVal;
}


size_t GameArea::getNumberConnections()const{
    ScopedMutexLock(access_);
    return players_.size();
}


void GameArea::setMap(const AreaMap& newMap){
    ScopedMutexLock(access_);
    map_ = newMap;
}

std::string GameArea::getMapName()const{
    return map_.getMapName();
}


void GameArea::sendAll(const inp::INFPacket& pack){
    connections_.sendAll(pack);
}


// runs map (non-blocking)
int GameArea::logic(){
    if( drop ){
        int lol_im_a_break_point = 1231;
    }
    int dropped = 0;
    INFPacket syncPack;
    CharCon playerCon;
    std::vector<inp::Connection*> activeConList;
    ScopedMutexLock(access_);
    //  Get active connections in a list.
    if( connections_.checkSockets( 1000, activeConList ) ){
        // handle each connection in list
        for(size_t i = 0; i < activeConList.size(); ++i){
            playerCon.first = activeConList.at(i);                                  //connection
            playerCon.second = players_.find(activeConList.at(i)->getId())->second; //player
            if( handleConnection( playerCon ) == -1 ){
                drop = true;
                dropped++;
                std::string id = playerCon.first->getId();
                //remove player from playerlist
                players_.erase( players_.find(id) );
                delete playerCon.second;
                connections_.erase( id );
                //tell others user is gone
                inp::INFPacket leavePack;
                leavePack << inp::DataTypeByte::USER_LEFT << id;
                sendAll(leavePack);
            } else {
                syncPack << buildSyncPacket(playerCon.second, playerCon.first);
            }
        }
        sendAll(syncPack);
    }

    /*//  sync players
    if( syncTimer.getTime() > 2000 ){
        connections_.sendAll( buildFullSyncPacket() );
        syncTimer.clear();
    }*/

    return dropped;
}


inp::INFPacket GameArea::buildSyncPacket(Character* player, Connection* con){
    if( player==NULL || con==NULL ){ return inp::INFPacket(); }

    // character details
    inp::INFPacket infoPack;
    infoPack << DataTypeByte::USERNAME << con->getId();
    infoPack << DataTypeByte::CHAR_NAME << player->getName();
    infoPack << DataTypeByte::CHAR_X << player->getX();
    infoPack << DataTypeByte::CHAR_Y << player->getY();
    infoPack << DataTypeByte::CHAR_CLASS << player->getClass();
    infoPack << DataTypeByte::CHAR_GENDER << player->getGender();
    infoPack << DataTypeByte::CHAR_DIR << player->getDirection();
    infoPack << DataTypeByte::CHAR_STATE << player->getState();

    //send all stats
    for( unsigned int i = 0; i < Stats::size_; ++i ){
        if( Stats::validStat(i) ){
            Stats::StatObjects stat = static_cast<Stats::StatObjects>(i);
            // the stat
            infoPack << DataTypeByte::CHAR_STAT_TYPE << static_cast<Uint8>(i);
            // the stat's value
            infoPack << DataTypeByte::CHAR_STAT_VAL << player->stats.getValue(stat);
        } else {
            std::cerr << __FILE__ << " " << __LINE__ << ": " << " Invalid stat: " << i << std::endl;
        }
    }

    return infoPack;
}

inp::INFPacket GameArea::buildFullSyncPacket(){
    INFPacket syncPack;
    if( players_.size() > 0 ){
        //  Get all players in the current room
        for( size_t i = 0; i < connections_.size(); ++i ){
           syncPack << buildSyncPacket( players_[connections_.conAt(i)->getId()],
                                        connections_.conAt(i) );
        }
    }
    return syncPack;
}



int GameArea::handleConnection( CharCon& player ){
    inp::INFPacket packet;
    std::string recvText;
    if( player.first->isActive() ) {
        //  Check for network activity
        int numReady = player.first->checkSocket();
        if( numReady == -1 ) {
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDLNet_CheckSockets: " << SDLNet_GetError() << std::endl;
            perror("SDLNet_CheckSockets");
            player.first->disconnect();
            player.first->setActive(false);
            return -1;
        } else if( numReady > 0 ){
            //  Recieve data and store into "packet"
            if( player.first->recv( packet ) != -1 ) {
                if( handlePacket(player, packet) == -1 ){
                    player.first->setActive(false);
                    return -1;
                }
            } else {
                //disconect
                std::cerr << __FILE__ << " " << __LINE__ << ": " << "Disconnecting a player: "+player.first->getId();
                player.first->disconnect();
                player.first->setActive(false);
                return -1;
            }
        }
    }
    return 0;
}

int GameArea::handlePacket( CharCon& playerCon, inp::INFPacket& packet ){
    using namespace inp;

    INFPacket outPacket;
    NetCode code;
    std::string recvText = "";
    std::string username = "";
    Sint32 recvVal = 0;
    //  Recursively read all data in packet
    while( !packet.readDone() ){
        packet >> code;

        //  If theres nothing left, or corupt packet
        //  just ignore it XD
        if( code == ControlByte::NOTHING ){ break; } else

        //  User is about to inform us soemthing about player with username following
        if( code == DataTypeByte::USERNAME ){
            packet >> recvText;
            if( recvText != "" ){
                username = recvText;
            }
        } else
        //  User us sending chat, relay to other users
        if( code == DataTypeByte::CHAT ){
            packet >> recvText;
            if( recvText != "" ){
                outPacket.clear();
                outPacket << DataTypeByte::USERNAME << username;
                outPacket << DataTypeByte::CHAT << recvText;
                connections_.sendAll(outPacket);
            }
        } else
        if( code == DataTypeByte::CHAR_X ){
            packet >> recvVal;
            if ( connections_.exists( username ) ){
                players_[username]->setX(recvVal);
            }
        } else
        //  user is telling us a user's y-pos
        if( code == DataTypeByte::CHAR_Y ){
            packet >> recvVal;
            if ( connections_.exists( username ) ){
                players_[username]->setY(recvVal);
            }
        } else
        //  user's state
        if( code == DataTypeByte::CHAR_DIR ){
            packet >> recvVal;
            if ( connections_.exists( username ) ){
                if( Character::validDirection(recvVal) ){
                    players_[username]->setDirection( static_cast<enum Directions>(recvVal) );
                }
            }
        } else
        //  user's state
        if( code == DataTypeByte::CHAR_STATE ){
            packet >> recvVal;
            if ( connections_.exists( username ) ){
                if( Character::validState(recvVal) ){
                    players_[username]->setState( static_cast<Character::CharacterStates>(recvVal) );
                }
            }
        } else
        /*/  User wants to move
        if( code == DataTypeByte::TRY_MOVE ){
            packet >> recvVal;
            if( Character::validDirection(recvVal) ){
                //sync position
                outPacket.clear();
                outPacket << DataTypeByte::USERNAME << playerCon.first->getId();
                outPacket << DataTypeByte::CHAR_X << player->getX();
                outPacket << DataTypeByte::CHAR_Y << player->getY();
                connections_.sendAll(outPacket);
            }
        } else
        //  User wants to stop
        if( code == DataTypeByte::TRY_STOP ){
            packet >> recvVal;
                //sync position
                outPacket.clear();
                outPacket << DataTypeByte::USERNAME << playerCon.first->getId();
                outPacket << DataTypeByte::CHAR_X << player->getX();
                outPacket << DataTypeByte::CHAR_Y << player->getY();
                connections_.sendAll(outPacket);
            }
        } else*/
        if( code == DataTypeByte::TRY_ATTACK ){
        } else
        if( code == DataTypeByte::TRY_DEFEND ){
        } else
        if( code == DataTypeByte::TRY_GAIN_ITEM ){
        } else
        if( code == DataTypeByte::TRY_DROP_ITEM ){
        } else
        if( code == DataTypeByte::TRY_USE_ITEM ){
        } else
        if( code == DataTypeByte::TRY_GAIN_SPELL ){
        } else
        if( code == DataTypeByte::TRY_CAST ){
        } else
        //  The client has pinged us. Echo the ping with a pong
        if( code == DataTypeByte::PING ){
            outPacket.clear();
            outPacket << DataTypeByte::PONG;
            playerCon.first->send(outPacket);
            break;
        } else
        //  The client has sent it's ping
        if( code == DataTypeByte::PING_TIME ){
            if( username != "" ){
                packet >> recvVal;
                if( recvVal != 0 ){
                    std::ostringstream pingStr;
                    pingStr << "Ping (" << username << "): " << recvVal;
                    server_->sendConsole( pingStr.str() );
                }
            }
            break;
        } else
        //  The client has requested our INP version
        if( code == DataTypeByte::REQ_INP_VER ){
            packet.clear();
            packet << DataTypeByte::INP_VER << inp::inp_ver;
            playerCon.first->send(packet);
            break;
        } else {
            server_->sendConsole(playerCon.first->getId() + " is sending unhandled data (h4x0rz)");
            return 0;
        }
    }//end while(packet.readDone());
    return 1;
}
