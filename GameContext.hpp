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


#ifndef GAMECONTEXT_HPP_INCLUDED
#define GAMECONTEXT_HPP_INCLUDED

#include <map>
#include <SDL/SDL.h>

#include "RunContext.hpp"

#include "GameGui.hpp"
#include "InfraellyMixer.hpp"
#include "Connection.hpp"
#include "Character.hpp"
#include "Timer.hpp"



class GameContext : RunContext {
    public:
        GameContext(inp::Connection *serverCon);
        ~GameContext();


    private:
        void events(SDL_Event &event);
        void input(Uint8 *keys);
        void logic();
        void draw();

        //  States used by network handler
        enum { DC, ERR, GOOD };

        //  Handles incomming network =]
        int handleIncomingNetwork();



        //call back to allow processing of entered text
        class ChatSendCallBack : public InputCallBack {
            public:
                ChatSendCallBack(GameContext *this_) : ob(this_){};
                ~ChatSendCallBack(){};
                void operator()(const std::string& text);

            private:
                GameContext *ob;
        };
        ChatSendCallBack chatSenderCallBack;
        //sends a sting (chat to server)
        void postToServer(const std::string& data);

        //processes slash commands
        void comLine(const std::string& commandString);

        //UI
        GameGui *gui;

        // connection to server
        inp::Connection *serverConnection;

        //ping timer
        Timer pingTimer;

        //  This will hold all the remote players and NPCS
        std::map<std::string, Character> remotePlayers;

        //  The local Player
        Character player;
        //  Co-ord update timer
        Timer movingUpdateTimer;
        Timer staticUpdateTimer;
        //  time between xy updates when moving
        static const int movingUpdateDelta = 150;
        //time between xy updates when stationary
        static const int staticUpdateDelta = 200;


        //  server wants to know our ping
        bool pingRequested;
        Timer pingRequestTimer;
};


#endif // INTROCONTEXT_HPP_INCLUDED
