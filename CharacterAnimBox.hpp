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


#ifndef CHARACTERANIMATOR_HPP_INCLUDED
#define CHARACTERANIMATOR_HPP_INCLUDED

//  CharacterAnimatorBox.hpp

#include "InfraellyWindow.hpp"
#include "guichan.hpp"
#include <vector>
#include "CharAnimation.hpp"
#include "globalFunc.hpp"

class CharAnimBuildBox;
class CharAnimPreviewBox;

class NumberField;
class AnimFrameList;
class InfraellyScroller;
class TSpriteIcon;
class InfraellyList;


class CharacterAnimBox : public InfraellyWindow, public gcn::SelectionListener {
    public:
        CharacterAnimBox();
        ~CharacterAnimBox();

        virtual void logic();

        virtual void mouseMoved(gcn::MouseEvent& mouseEvent);
        virtual void mouseClicked(gcn::MouseEvent& mouseEvent);
        virtual void valueChanged(const gcn::SelectionEvent& event);

        virtual void draw(gcn::Graphics *graphics);

    private:
        int mouseX_, mouseY_;
        int animOriginX_, animOriginY_;
        bool editing_;
        bool drawAnim_;
        CharAnimation anim_;
        CharAnimation::BodyParts activePart_;   // bodypart
        enum Directions activeDir_;             //direction
        std::vector<AnimFrame> defaultFrames_;

        void addListItem();
        void addListItem(const AnimFrame &frame);
        void editListItem();
        bool save(const std::string &filename, bool discrete = 0);
        bool load(const std::string &filename, bool discrete = 0);


        //  GUI
        //--------------
        gcn::Label *filenameLbl;
        gcn::TextField *filenameFld;
        gcn::Label *nameLbl;
        gcn::TextField *nameFld;
        //--------------
        //4(directions) sets of 6(ea body part) framelists
        std::vector< std::vector<AnimFrameList*> >frameLists;
        InfraellyList *dirList;
        gcn::DropDown *dirDrpList;
        gcn::ListBox *frameLstBox;
        InfraellyScroller* frameScroller;
        gcn::Button *listUpBtn;
        gcn::Button *listDownBtn;
        gcn::Button *listDelBtn;
        gcn::Button *listAddBtn;
        gcn::Button *listDupeBtn;
        //--------------
        std::vector<TSpriteIcon*>bodyEditBtns;
        std::vector<gcn::Button*>bodyToggleBtns;
        //--------------
        gcn::CheckBox *lockEdit;    // only allows the selected part to be moved
        gcn::Label *angleLbl;
        NumberField *angleFld;
        gcn::Label *zoomLbl;
        NumberField *zoomFld;
        gcn::Button *setRotoZoomBtn;
        //--------------
        gcn::Container *animPVBox;
        gcn::Label *rateLbl;
        NumberField *rateFld;
        gcn::Button *playBtn;
        gcn::Button *saveBtn;
        gcn::Button *loadBtn;
        //--------------
        gcn::Label *infobarLbl;
};

#endif // CHARACTERANIMATOR_HPP_INCLUDED
