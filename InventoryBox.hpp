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


#ifndef INVENTORYBOX_HPP_INCLUDED
#define INVENTORYBOX_HPP_INCLUDED


#include "InfraellyWindow.hpp"
#include "guichan.hpp"
#include "TSpriteButton.hpp"
#include <vector>

class TSpriteButton;
class Inventory;
class InfraellyScroller;

class InventoryBox : public InfraellyWindow{
    public:
        InventoryBox();
        ~InventoryBox();

        Inventory *getInventory()const;
        void setInventory(Inventory *invent);

        virtual void logic();

    private:
        Inventory *inventory_;

        gcn::Image* junkImage_;
        gcn::Image* dropImage_;
        gcn::Image* equipImage_;

        gcn::Container *itemContainer_;
        InfraellyScroller *scroller_;
        gcn::ImageButton *junkBtn_;
        gcn::ImageButton *dropBtn_;
        gcn::ImageButton *equipsBtn_;
        gcn::Label *weightLbl_;

        std::vector<TSpriteButton*> itemHolders_;

        int lastSize_;
};

#endif // INVENTORYBOX_HPP_INCLUDED
