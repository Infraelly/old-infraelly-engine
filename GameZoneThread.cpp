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


#include "GameZoneThread.hpp"

#include <sstream>
#include "GameArea.hpp"
#include "INFPacket.hpp"
#include "Connection.hpp"
#include "Character.hpp"
#include "MutexLocker.hpp"


using namespace inp;

GameZoneThread::GameZoneThread(const std::vector<GameArea*>& gameAreaList) :
    dataAccess_(SDL_CreateMutex())
{
    gameAreas_ = gameAreaList;
}

GameZoneThread::~GameZoneThread(){
    SDL_LockMutex(dataAccess_);
    SDL_DestroyMutex(dataAccess_);
    dataAccess_ = NULL;
}


inp::Connection *GameZoneThread::getConnection(const std::string& id)const{
    MutexLocker lock(dataAccess_);
    inp::Connection *con;
    for(size_t i = 0; i < gameAreas_.size(); ++i){
        con = gameAreas_.at(i)->getConnection(id);
        if( con != NULL ){ break; }
    }
    return con;
}


size_t GameZoneThread::getNumberConnections()const{
    MutexLocker lock(dataAccess_);
    size_t count = 0;
    for(size_t i = 0; i < gameAreas_.size(); ++i){
        count += gameAreas_.at(i)->getNumberConnections();
    }
    return count;
}


CharCon GameZoneThread::removePlayer(const std::string& id){
    MutexLocker lock(dataAccess_);
    CharCon playerCon;
    for(size_t i = 0; i < gameAreas_.size(); ++i){
        playerCon = gameAreas_.at(i)->removeCharCon(id);
        if( playerCon.first != NULL ){ break; }
    }
    return playerCon;
}

bool GameZoneThread::addPlayer(const CharCon& player, const std::string& map){
    MutexLocker lock(dataAccess_);
    for(size_t i = 0; i < gameAreas_.size(); ++i){
        if( gameAreas_.at(i)->getMapName() == map ){
            gameAreas_.at(i)->addPlayer(player);
            return true;
        }
    }
    return false;
}


void GameZoneThread::sendAll(const inp::INFPacket& packet){
    MutexLocker lock(dataAccess_);
    for(size_t i = 0; i < gameAreas_.size(); ++i){
        gameAreas_.at(i)->sendAll(packet);
    }
}


void GameZoneThread::handleUserData(inp::INFPacket &data){
}


bool GameZoneThread::work(){
    MutexLocker lock(dataAccess_);
    for(size_t i = 0; i < gameAreas_.size(); ++i){
        gameAreas_.at(i)->logic();
    }
    return true;
}
/*
void GameZoneThread::sendConsole(const std::string& text){
    INFPacket packet;
    packet << inp::DataTypeByte::THREAD_SEND_CONSOLE << text;
    sendOut(packet);
}*/
