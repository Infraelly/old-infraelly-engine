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


#include "LoadTilesetBox.hpp"

//LoadTilesetBox.cpp

#include "guichan.hpp"
//string proccessing and validation
#include "globalFunc.hpp"
#include "InfraellyScroller.hpp"
#include "TilesetIcon.hpp"
#include "NumberField.hpp"
#include "caches.hpp"


using namespace std;


LoadTilesetBox::LoadTilesetBox(){
    setCaption("Load tileset (from xml)");

    //initialise the labels
    fnLabel = new gcn::Label("XML Filename: ");

    //initialise the textFields
    fnTxtField = new gcn::TextField("data/tilesets/");


    //initialise the preview
    tsIcon_ = new TilesetIcon(&ts_);
    tsScroller_ = new InfraellyScroller(tsIcon_);


    //set positions
    fnLabel->setX( 10 );
    fnLabel->setY( 10 );
    //set the x values of the txt boxes according to widest label
    //y : under ea. other
    fnTxtField->setX( 100 );
    fnTxtField->setY( 10 );
    fnTxtField->setWidth(100);

    tsScroller_->setPosition(10, 50);

    tsScroller_->setFrameSize(1);
    tsScroller_->setSize(250,200);


    //add objects listeners
    fnTxtField->addKeyListener(this);


    //add objects
    add(fnLabel);
    add(fnTxtField);

    add(tsScroller_);

    //re resize tot fit the buttons
    resizeToContent();
    setWidth(getWidth()+10);
    // position info form buttons (ok, cancel)
    positionDefaultBtns();
}

LoadTilesetBox::~LoadTilesetBox(){
    delete fnLabel;
    delete fnTxtField;
    fnLabel = NULL;
    fnTxtField = NULL;
    delete tsIcon_;
    tsIcon_ = NULL;
    delete tsScroller_;
    tsScroller_ = NULL;
}

std::string LoadTilesetBox::getFilename(){ return fnTxtField->getText(); }

void LoadTilesetBox::keyPressed(gcn::KeyEvent& keyEvent){
//    if( keyEvent.isConsumed() ){ return; }

    if( keyEvent.getSource() == fnTxtField ){
        if( ts_.loadXml(fnTxtField->getText()) ){
            tsIcon_->setTileset(&ts_);
        }
    }
}



void LoadTilesetBox::doneAction(){
    // add ts to global ts array
    if( ts_.getImage() != NULL ){
        cache::tilesets.loadResource( fnTxtField->getText() );
    }
}
