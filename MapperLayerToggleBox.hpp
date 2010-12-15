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


#ifndef MAPPERLAYERTOGGLEBAR_HPP
#define MAPPERLAYERTOGGLEBAR_HPP

//  MapperLayerToggleBar.hpp


#include <string>

#include "guichan.hpp"

#include "InfraellyWindow.hpp"
#include "InfraellyLabel.hpp"
#include "LayerControl.hpp"


class MapperLayerToggleBox : public InfraellyWindow{
    public:
        MapperLayerToggleBox();
        ~MapperLayerToggleBox();


        LayerControl *getLayerControl(int layerNumber)const;


        int getWorkingLayer()const;
        bool isView(int layerNumber)const;
        bool isViewAttributes(int layerNumber)const;




    protected:
        gcn::Label *layerNumberLabel;
        gcn::Label *layerNameLabel;
        gcn::Label *viewLabel;
        gcn::Label *viewAttributesLabel;

        InfraellyLabel *itemLayerLabel;
        InfraellyLabel *playerNpcLabel;
        InfraellyLabel *infraellyGuiLabel;

        LayerControl *layerControl[6];
};



#endif

