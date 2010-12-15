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


#include "MakeTilesetBox.hpp"

//MakeTilesetBox.cpp

#include "guichan.hpp"
//string proccessing and validation
#include "globalFunc.hpp"
#include "InfraellyScroller.hpp"
#include "TilesetIcon.hpp"
#include "NumberField.hpp"
#include "caches.hpp"


using namespace std;


MakeTilesetBox::MakeTilesetBox(){
    setCaption("Make a new tileset (from image)");

    //initialise the labels
    label[0] = new gcn::Label("Image Filename: ");
    label[1] = new gcn::Label("Tileset Alias: ");
    label[2] = new gcn::Label("Tile Width: ");
    label[3] = new gcn::Label("Tile Height:");
    label[4] = new gcn::Label("Tileset X-Offset: ");
    label[5] = new gcn::Label("Tileset Y-Offset: ");
    label[6] = new gcn::Label("Save Filename: ");
    //initialise the textFields
    textField[0] = new gcn::TextField("data/images/");
    textField[1] = new gcn::TextField("Untitled Tileset");
    textField[2] = new NumberField("32");
    textField[3] = new NumberField("32");
    textField[4] = new NumberField("0");
    textField[5] = new NumberField("0");
    textField[6] = new gcn::TextField("someTileset.xml");

    //initialise the preview
    tsIcon_ = new TilesetIcon(&ts_);
    tsScroller_ = new InfraellyScroller(tsIcon_);


    //set positions
    //find widest label
    int width = 0;
    for( unsigned int i = 0; i < 7; ++i ){
        if( label[i]->getWidth() > width ){ width = label[i]->getWidth()+10; };
    }

    //set the x values of the txt boxes according to widest label
    //y : under ea. other
    for( unsigned int i = 0; i < 7; ++i ){
        textField[i]->setX(width);
        textField[i]->setY( (textField[i]->getHeight()*i)+textField[0]->getY()+(2*i) );
    }

    //set the labels, x = 0;
    // y : match txt boxes
    for( unsigned int i = 0; i < 7; ++i ){
        label[i]->setPosition(0, textField[i]->getY()+2 );
    }
    tsScroller_->setPosition(10, 150);

    //set standard sizes and alignment
    for( unsigned int i = 0; i < 7; ++i ){
        label[i]->setWidth(width);
        label[i]->setAlignment(gcn::Graphics::RIGHT);
        textField[i]->setWidth(100);
    }
    tsScroller_->setFrameSize(1);
    tsScroller_->setSize(250,200);


    //add objects listeners
    for( unsigned int i = 0; i < 7; ++i ){
        textField[i]->addKeyListener(this);
    }



    //add objects
    for( unsigned int i = 0; i < 7; ++i ){
        add(label[i]);
        add(textField[i]);
    }
    add(tsScroller_);

    //re resize tot fit the buttons
    resizeToContent();
    setWidth(getWidth()+10);
    // position info form buttons (ok, cancel)
    positionDefaultBtns();
}

MakeTilesetBox::~MakeTilesetBox(){
    for( unsigned int i = 0; i < 7; ++i ){
        delete label[i];
        label[i] = NULL;

        delete textField[i];
        textField[i] = NULL;
    }
    delete tsIcon_;
    tsIcon_ = NULL;
    delete tsScroller_;
    tsScroller_ = NULL;
}

std::string MakeTilesetBox::getFilename(){ return textField[0]->getText(); }

std::string MakeTilesetBox::getAlias(){ return textField[1]->getText(); }

int MakeTilesetBox::getTileWidth(){
    return atoi( textField[2]->getText() );
}

int MakeTilesetBox::getTileHeight(){
    return atoi( textField[3]->getText() );
}

int MakeTilesetBox::getOffsetX(){
    return atoi( textField[4]->getText() );
}

int MakeTilesetBox::getOffsetY(){
    return atoi( textField[5]->getText() );
}

std::string MakeTilesetBox::getSaveFilename(){ return textField[6]->getText(); }

void MakeTilesetBox::keyPressed(gcn::KeyEvent& keyEvent){
//    if( keyEvent.isConsumed() ){ return; }

    if( (keyEvent.getSource() == textField[0])  ||
        (keyEvent.getSource() == textField[2])  ||
        (keyEvent.getSource() == textField[3])  ||
        (keyEvent.getSource() == textField[4])  ||
        (keyEvent.getSource() == textField[5])  ||
        (keyEvent.getSource() == textField[6]) ){
        loadPrevTileset();
    }
}



void MakeTilesetBox::doneAction(){
    // add ts to global ts array
    if( ts_.getImage() != NULL ){
        if( cache::tilesets.get( ts_.getFilename() ) == NULL ){
            Tileset *newTs = new Tileset(ts_);
            newTs->setFilename( ts_.getFilename() );
            cache::tilesets.addPointer( newTs );
        }
    }
}


bool MakeTilesetBox::loadPrevTileset(){
    ts_.loadImage( textField[0]->getText(),
              textField[1]->getText(),
              atoi(textField[2]->getText()),
              atoi(textField[3]->getText()) );

    ts_.setXOffset( atoi(textField[4]->getText()) );
    ts_.setYOffset( atoi(textField[5]->getText()) );
    ts_.setFilename( textField[6]->getText() );

    if( ts_.getImage() != NULL ){
        tsIcon_->setTileset(&ts_);
        return 1;
   } else {
        return 0;
    }
}


