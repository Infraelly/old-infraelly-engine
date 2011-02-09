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


#ifndef GAMEAREA_HPP_INCLUDED
#define GAMEAREA_HPP_INCLUDED


/*******************************************************************************

                                    GameArea

                                Runs a single map.
                                Thread safe

*******************************************************************************/

#include <string>
#include <SDL.h>

#include "INFPacket.hpp"
#include "AreaMap.hpp"
#include "ConnectionGroup.hpp"
#include "Timer.hpp"

class Character;
class ServerContext;
typedef std::pair<inp::Connection*, Character*> CharCon;

class GameArea{
    public:
        GameArea(ServerContext *server);
        explicit GameArea(ServerContext *server, const std::string& mapFilename);
        ~GameArea();

        // Returns a temporary COPY of the player. (cant return pointer because
        //  player is not thread safe)
        Character getPlayer(const std::string& id)const;
        // returns a pointer to the connection coresponding to id
        //  null if does not exist
        inp::Connection *getConnection(const std::string& id)const;

        //  Adds player to map.
        void addPlayer(const CharCon& playerCon);
        //  Returns the pointer for a player and stops processing its data
        // null if nothing
        CharCon removeCharCon(const std::string& id);


        //  Returns the number of connections
        size_t getNumberConnections()const;


        //  Sets the map to be used
        void setMap(const AreaMap& newMap);
        //  Returns the name of the map
        std::string getMapName()const;


        //  Sends a packet to all connections
        void sendAll(const inp::INFPacket& pack);


        // runs map (non-blocking)
        // returns number of player's dropped
        int logic();



    private:
        // Protect non thread safe members
        SDL_mutex *access_;

        //  Server context handle
        ServerContext *server_;

        //map associated with the game region
        AreaMap map_;

        //store conections
        inp::ConnectionGroup connections_;

        //store character objects coresponding to connections
        typedef std::map< std::string, Character* > PlayerList;
        typedef PlayerList::iterator PlayerListIterator;
        PlayerList players_;


        //  Timer kept to perform tasks at a set time inteval
        Timer syncTimer;

        //      Helper Functions

        inp::INFPacket buildSyncPacket(Character* player, inp::Connection* con);
        inp::INFPacket buildFullSyncPacket();

        //returns -1 if connection shud be removed (disconnected)
        int handleConnection( CharCon& player );
        int handlePacket( CharCon& player, inp::INFPacket& packet );

        //  Stops coppying
        GameArea(const GameArea &g){};
        GameArea& operator=(GameArea& rhs);
        void operator=(GameArea g)const{};
};



#endif // ROOM_HPP_INCLUDED
