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


#ifndef MAPPERFILETOOLBAR_HPP
#define MAPPERFILETOOLBAR_HPP

//  MapperFileToolBar.hpp

#include <string>

#include "guichan.hpp"

#include "InfraellyWindow.hpp"

#include "Tileset.hpp"
#include "AreaMap.hpp"


class MapperFileToolBar : public InfraellyWindow{
    protected:
        AreaMap *world;

        gcn::ImageButton *newDocButton;
        gcn::ImageButton *saveDocButton;
        gcn::ImageButton *saveAsButton;
        gcn::ImageButton *openDocButton;
        gcn::ImageButton *reopenDocButton;
        gcn::ImageButton *closeDocButton;
        gcn::ImageButton *detailsButton;

    public:
        MapperFileToolBar(AreaMap *worldMap = NULL);
        ~MapperFileToolBar();

        gcn::ImageButton *getNewDocButton()const;
        gcn::ImageButton *getSaveDocButton()const;
        gcn::ImageButton *getSaveAsButton()const;
        gcn::ImageButton *getOpenDocButton()const;
        gcn::ImageButton *getReopenDocButton()const;
        gcn::ImageButton *getCloseDocButton()const;
        gcn::ImageButton *getDetailsButton()const;
};




#endif
