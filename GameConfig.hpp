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


#ifndef GAMECONFIG_HPP_INCLUDED
#define GAMECONFIG_HPP_INCLUDED

#include <string>

struct GameConfig{
    public:
        static bool saveXml(const std::string &filename);
        static bool loadXml(const std::string &filename);

        //  Logs
        static bool logging;

        //  Game speed
        static const int fps;
        //  Skip main menu

        static const bool bypassMainMenu;

        //          Server
        //  Server's ip (for client)
        static std::string serverIp;
        //  Server's port
        static int serverPort;
        //  The maxumin network connections (for server)
        static int maxConnections;
        //  The number of worker threads the server will use
        static int serverThreads;

        //      Resolution
        //  Start fullscreen
        static bool startFullscreen;
        //  Fullscreen resolution
        static int fScreenWidth;
        static int fScreenHeight;
        static int fScreenBpp;
        //  Windowed resolution
        static int wScreenWidth;
        static int wScreenHeight;
        static int wScreenBpp;

        //      Audio
        //  Enable sound
        static bool sound;
        static bool music;
        static int audioRate;
        static int aduioBuffSize;

        //  For test mode/debug
        static int wTestScreenWidth;
        static int wTestScreenHeight;
};

#endif // GAMECONFIG_HPP_INCLUDED
