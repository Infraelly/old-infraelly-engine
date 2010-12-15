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


#ifndef LAYERCONTROL_HPP
#define LAYERCONTROL_HPP

//  LayerControl.hpp


#include "guichan.hpp"
#include "RadioBar.hpp"


class LayerControl : public gcn::Container{
    public:
        LayerControl();
        LayerControl(const std::string& group, bool checked = false);
        ~LayerControl();


        //  Returns the layer number
        int getNumber();
        //  Returns the name of the layer
        const std::string& getName();
        //  Returns whether the layer should be viewed or not
        bool isView();
        //  Returns whether the attributes shuold be viewed
        bool isViewAttribute();
        //  Returns if te radio bar is selected
        bool isSelected();




        //  Sets the layer number to "newNumber"
        void setNumber(int newNumber);
        //  Sets the name to "newName"
        void setName(const std::string& newName);
        //  Sets whether to view the map or not
        void setView(bool value);
        //  Sets whether to view the Attributes of teh layer or not
        void setViewAttribute(bool value);
        //  Sets whether the radio bar is selected
        void setSelected(bool value);




    private:
        RadioBar *radioBar;
        gcn::Label *number;
        gcn::TextField *name;
        gcn::CheckBox *view;
        gcn::CheckBox *attributeView;

        void init();
};





#endif
