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


#ifndef ITEMANIMBOX_HPP_INCLUDED
#define ITEMANIMBOX_HPP_INCLUDED

//  ItemAnimBox.hpp

#include "InfraellyWindow.hpp"
#include "Animation.hpp"

class NumberField;
class KeyPointList;
class SnapPointList;
class AnimFrameList;
class InfraellyScroller;
class AnimPreviewIcon;


class ItemAnimBox : public InfraellyWindow {
    public:
        ItemAnimBox();
        ~ItemAnimBox();

        virtual void mouseClicked(gcn::MouseEvent& mouseEvent);


    private:
        bool save(const std::string &filename, bool discrete = 0);
        bool load(const std::string &filename, bool discrete = 0);

        //  GUI
        //--------------
        KeyPointList *keyPointLst;
        SnapPointList *snapPointLst;
        AnimFrameList *frameLst;
        //--------------
        gcn::Label *filenameLbl;
        gcn::TextField *filenameFld;
        gcn::Label *nameLbl;
        gcn::TextField *nameFld;
        gcn::Label *snapLbl;
        gcn::DropDown *snapDrpLst;
        gcn::Label *originLbl;
        gcn::DropDown *originDrpLst;
        gcn::Label *alignLbl;
        gcn::DropDown *alignDrpLst;
        //--------------
        gcn::ListBox *frameLstBox;
        InfraellyScroller* frameScroller;
        gcn::Button *listUpBtn;
        gcn::Button *listDownBtn;
        gcn::Button *listDelBtn;
        gcn::Button *listAddBtn;
        gcn::Label *xLbl;
        NumberField *xFld;
        gcn::Label *yLbl;
        NumberField *yFld;
        gcn::Label *angleLbl;
        NumberField *angleFld;
        gcn::Label *zoomLbl;
        NumberField *zoomFld;
        //--------------
        AnimPreviewIcon *animPVBox;
        gcn::Label *rateLbl;
        NumberField *rateFld;
        gcn::Button *playBtn;
        gcn::Button *saveBtn;
        gcn::Button *loadBtn;

        //  Animation object being edited
        Animation anim;
};


#endif // ITEMANIMBOX_HPP_INCLUDED
