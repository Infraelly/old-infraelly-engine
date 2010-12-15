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


#include "LoadTSpriteBox.hpp"

//LoadTSpriteBox.cpp

#include "guichan.hpp"
//string proccessing and validation
#include "globalFunc.hpp"
#include "InfraellyScroller.hpp"
#include "TSpriteIcon.hpp"
#include "NumberField.hpp"
#include "caches.hpp"


using namespace std;


LoadTSpriteBox::LoadTSpriteBox(){
    setCaption("Load TSprite (from xml)");

    //initialise the labels
    fnLabel = new gcn::Label("XML Filename: ");

    //initialise the textFields
    fnTxtField = new gcn::TextField("data/tsprites/");


    //initialise the preview
    tsprIcon_ = new TSpriteIcon(&tspr_);
    tsprScroller_ = new InfraellyScroller(tsprIcon_);


    //set positions
    fnLabel->setX( 10 );
    fnLabel->setY( 10 );
    //set the x values of the txt boxes according to widest label
    //y : under ea. other
    fnTxtField->setX( 100 );
    fnTxtField->setY( 10 );
    fnTxtField->setWidth(100);

    tsprScroller_->setPosition(10, 50);

    tsprScroller_->setFrameSize(1);
    tsprScroller_->setSize(250,200);


    //add objects listeners
    fnTxtField->addKeyListener(this);


    //add objects
    add(fnLabel);
    add(fnTxtField);

    add(tsprScroller_);

    //re resize tot fit the buttons
    resizeToContent();
    setWidth(getWidth()+10);
    // position info form buttons (ok, cancel)
    positionDefaultBtns();
}

LoadTSpriteBox::~LoadTSpriteBox(){
    delete fnLabel;
    delete fnTxtField;
    fnLabel = NULL;
    fnTxtField = NULL;
    delete tsprIcon_;
    tsprIcon_ = NULL;
    delete tsprScroller_;
    tsprScroller_ = NULL;
}

std::string LoadTSpriteBox::getFilename(){ return fnTxtField->getText(); }

void LoadTSpriteBox::keyPressed(gcn::KeyEvent& keyEvent){
//    if( keyEvent.isConsumed() ){ return; }

    if( keyEvent.getSource() == fnTxtField ){
        if( tspr_.loadXml(fnTxtField->getText()) ){
            tsprIcon_->setTSprite(&tspr_);
        }
    }
}



void LoadTSpriteBox::doneAction(){
    // add ts to global ts array
    if( tspr_.getSource() != NULL ){
        cache::tsprites.loadResource( fnTxtField->getText() );
    }
}
