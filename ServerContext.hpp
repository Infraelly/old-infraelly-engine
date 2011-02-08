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


#ifndef SERVERCONTEXT_HPP_INCLUDED
#define SERVERCONTEXT_HPP_INCLUDED

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <SDL/SDL.h>

#include "RunContext.hpp"

#include "Console.hpp"
#include "Connection.hpp"
#include "Timer.hpp"
#include "GameArea.hpp"
#include "GameZoneThread.hpp"


#include "ConnectionListenerThread.hpp"


class ServerContext : RunContext {
    public:
        ServerContext();
        ~ServerContext();

        friend class ConnectionListenerThread;

        //  Sends some text to the console
        void sendConsole(const std::string& text);

    private:
        //To lock access to the world
        SDL_mutex *worldAccess_;
        typedef std::map<std::string, GameArea*> GameAreaContainer;
        typedef GameAreaContainer::iterator GameAreaContainerIterator;
        //  This is the world object, it contains all the GameAreas
        GameAreaContainer world_;

        //  Each thread manages a group of GameAreas
        std::vector<GameZoneThread*> threads_;

        //  Controlls access to the console(gui)
        SDL_mutex *consoleAccess_;
        std::auto_ptr<Console> console_;

        //  Simple server uptime timer
        SDL_mutex *timerAccess_;
        Timer serverUpTimer_;

        //  Thread listens for new connections
        ConnectionListenerThread listenerThread_;


        //------------------
        //  Helper functions
        //-------------------

        //  Does authenticating of a new incomming connection,
        //      and synconising stats
        int loginPlayer(inp::Connection *newCon);

        //  Command lin proccessor for the console
        void commandLine(std::string commandString);


        // convienience functions
        //  Sends to all players
        void sendAll(const inp::INFPacket& pack);
        // Sends to specific user
        //  returns false if action failed
        bool sendTo(const std::string& username, const inp::INFPacket& pack);
        //  Total players
        unsigned int totalOnline();


        //call back to allow processing of entered text (for console comands)
        class ComLineCallBack : public InputCallBack{
            public:
                ComLineCallBack(ServerContext *this_) : ob(this_){};
                ~ComLineCallBack(){};
                void operator()(const std::string& text);

            private:
                ServerContext *ob;
        };
        ComLineCallBack comLineCaller;


        void events(SDL_Event &event);
        void input(Uint8 *keys);
        void logic();
        void draw();
};



#endif // SERVERCONTEXT_HPP_INCLUDED
