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


#ifndef GAMEGUI_HPP
#define GAMEGUI_HPP

//  GameGui.hpp

#include <string>
#include "BaseGui.hpp"
#include "Character.hpp"
#include "Stats.hpp"
#include "StatBox.hpp"
#include "ChatBox.hpp"
#include "HealthBox.hpp"
#include "InventoryBox.hpp"


class GameGui : public BaseGui{
    public:
        GameGui();
        GameGui(SDL_Surface *screen);
        GameGui(SDL_Surface *screen, Stats *statsSource);
        GameGui(SDL_Surface *screen, Character *characterSource);
        ~GameGui();


        //  Repositions all widgets
        void resetPositions();


        //  Returns the StatBox
        StatBox *getStatBox()const;
        //  Returns the ChatBox
        ChatBox *getChatBox()const;
        //  Returns the HealthBox
        HealthBox *getHealthBox()const;
        //  Retruns the Inventory Box
        InventoryBox *getInventoryBox()const;



    private:
        void init();            //  Initialises object

        StatBox *statBox;       //  The StatBox
        ChatBox *chatBox;       //  The ChatBox
        HealthBox *healthBox;   //  The HealthBox
        InventoryBox *inventoryBox;
};




#endif
