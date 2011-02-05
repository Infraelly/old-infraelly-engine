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


#include "AnimatorGui.hpp"
#include "AnimToolBar.hpp"
#include "ItemAnimBox.hpp"
#include "CharacterAnimBox.hpp"
#include "LoadTilesetBox.hpp"
#include "MakeTilesetBox.hpp"
#include "LoadTSpriteBox.hpp"
#include "MakeTSpriteBox.hpp"


AnimatorGui::AnimatorGui(){
    init();

    //reset the position of all UI objects
    resetPositions();
}

AnimatorGui::AnimatorGui(SDL_Surface *screen){
    init();

    setTarget(screen);
    //reset the position of all UI objects
    resetPositions();
}

AnimatorGui::~AnimatorGui(){
    //delete
    delete newAnimBox_;
    delete itemAnimBox_;
    delete charAnimBox_;
    delete loadTSpriteBox_;
    delete makeTSpriteBox_;
    delete loadTilesetBox_;
    delete makeTilesetBox_;
}


void AnimatorGui::init(){
    //create widgets
    newAnimBox_ = new AnimToolBar();
    newAnimBox_->setVisible(true);

    itemAnimBox_ = new ItemAnimBox;
    itemAnimBox_->setVisible(true);

    charAnimBox_ = new CharacterAnimBox();

    loadTSpriteBox_ = new LoadTSpriteBox;
    loadTSpriteBox_->setVisible(true);

    makeTSpriteBox_ = new MakeTSpriteBox;
    makeTSpriteBox_->setVisible(true);

    loadTilesetBox_ = new LoadTilesetBox;
    loadTilesetBox_->setVisible(true);

    makeTilesetBox_ = new MakeTilesetBox;
    makeTilesetBox_->setVisible(true);


    //add widgets
    add(loadTSpriteBox_);
    add(loadTilesetBox_);
    add(makeTilesetBox_);
    add(makeTSpriteBox_);
    add(newAnimBox_);
    add(itemAnimBox_);
    add(charAnimBox_);
}

//  Resets positions of widgets
void AnimatorGui::resetPositions(){
    BaseGui::resetPositions();
}


//  Used for geting info from popups
//  info is collected when  widgets are hideen
void AnimatorGui::widgetHidden(const gcn::Event& event){}
//  Listener, for the drop down (tileset info)
void AnimatorGui::valueChanged(const gcn::SelectionEvent& event){}
//mouse listener
void AnimatorGui::mouseClicked(gcn::MouseEvent& mouseEvent){}


