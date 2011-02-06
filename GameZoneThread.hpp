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


#ifndef GAMEZONETHREAD_HPP_INCLUDED
#define GAMEZONETHREAD_HPP_INCLUDED


/*******************************************************************************

                            GameZoneThread

                      Runs a group of GameAreas (maps)
                                Thread safe

*******************************************************************************/


#include <SDL/SDL.h>
#include <vector>
#include <string>
#include "ThreadBase.hpp"

#include "GameArea.hpp"

class Character;
class GameArea;
namespace inp{
    class Connection;
    class INFPacket;
};


class GameZoneThread : public ThreadBase {
    public:
        GameZoneThread(const std::vector<GameArea*>& gameAreaList);
        ~GameZoneThread();

        inp::Connection *getConnection(const std::string& id)const;
        //  Returns the number of connections
        size_t getNumberConnections()const;

        //  removes a player from its gameArea and returns it
        // null if player not found
        CharCon removePlayer(const std::string& id);

        //  returns true if success
        //  returns false if mentioned game area not found
        bool addPlayer(const CharCon& player, const std::string& map);


        void sendAll(const inp::INFPacket& packet);


        virtual void handleUserData(inp::INFPacket &data);
        virtual bool work();


    private:
        GameZoneThread(const GameZoneThread &g){};
        void operator=(GameZoneThread g){};

        SDL_mutex *dataAccess_;

        std::vector<GameArea*> gameAreas_;


        //void sendConsole(const std::string& text);
};


#endif // GAMEZONETHREAD_HPP_INCLUDED
