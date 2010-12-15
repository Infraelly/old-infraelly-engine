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


#include "LoadMapBox.hpp"

//LoadMapBox.cpp

//string proccessing and validation
#include "globalFunc.hpp"

using namespace std;

LoadMapBox::LoadMapBox(){
    setCaption("Create a new Map");


    //initialise the labels
    label[0] = new gcn::Label("Filename: ");
    label[1] = new gcn::Label("Map Name: ");
    label[2] = new gcn::Label("Tile Width: ");
    label[3] = new gcn::Label("Tile Height: ");
    label[4] = new gcn::Label("Map Width (in tiles): ");
    label[5] = new gcn::Label("Map Height (in tiles):");
    label[6] = new gcn::Label("Player Exclusive: ");
    label[7] = new gcn::Label("Size: 0x0 (px)");


    //initialise the textfields
    textField[0] = new gcn::TextField("ForrestGreen.imf");
    textField[1] = new gcn::TextField("Forrest of the Green");
    textField[2] = new gcn::TextField("32");
    textField[3] = new gcn::TextField("32");
    textField[4] = new gcn::TextField("32");
    textField[5] = new gcn::TextField("32");

    //initialise the check box
    playerExclusiveCheckBox = new gcn::CheckBox( "", false );


    //set positions
    //find widest label
    int width = 0;
    for( unsigned int i = 0; i < 7; ++i ){
        if( label[i]->getWidth() > width ){ width = label[i]->getWidth(); };
    };

    //set the x values of the txt boxes according to widest label
    //y : under ea. other
    for( unsigned int i = 0; i < 6; ++i ){
        textField[i]->setX(width);
        textField[i]->setY( (textField[i]->getHeight()*i)+textField[0]->getY()+2 );
    }
    //set the check box
    playerExclusiveCheckBox->setX(width);
    playerExclusiveCheckBox->setY( (textField[5]->getHeight()*6)+textField[0]->getY() );


    //set the labels, x = 0;
    // y : match txt boxes
    for( unsigned int i = 0; i < 6; ++i ){
        label[i]->setPosition(0, textField[i]->getY() );
    }
    label[6]->setPosition(0, playerExclusiveCheckBox->getY() );
    label[7]->setPosition(0, label[6]->getY() + label[6]->getHeight() +20 );


    //set the buttons bellow
    doneButton->setPosition(0, (label[7]->getY()+label[7]->getHeight()) +20);
    cancelButton->setPosition(doneButton->getX()+doneButton->getWidth(),
                                doneButton->getY());



    //set standard sizes and alignment
    for( unsigned int i = 0; i < 7; ++i ){
        label[i]->setWidth(width);
        label[i]->setAlignment(gcn::Graphics::RIGHT);
    }
    label[7]->setWidth( width + 150);
    label[7]->setAlignment(gcn::Graphics::CENTER);

    for( unsigned int i = 0; i < 6; ++i ){
        textField[i]->setWidth(150);
    }


    //add objects
    for( unsigned int i = 0; i < 6; ++i ){
        add(label[i]);
        add(textField[i]);
    };
    add(label[6]);
    add(label[7]);

    add(playerExclusiveCheckBox);


    //re resize tot fit the buttons
    resizeToContent();
}

LoadMapBox::~LoadMapBox(){
    for( unsigned int i = 0; i < 6; ++i ){
        delete label[i];
        delete textField[i];
    };
    delete label[6];
    delete label[7];
}



std::string LoadMapBox::getFilename(){ return textField[0]->getText(); }

std::string LoadMapBox::getMapName(){ return textField[1]->getText(); }

int LoadMapBox::getTileWidth(){
    if( !isNumber(textField[2]->getText()) ){
        return 32;
    } else {
        return atoi( textField[2]->getText().c_str() );
    }
}

int LoadMapBox::getTileHeight(){
    if( !isNumber(textField[3]->getText()) ){
        return 32;
    } else {
        return atoi( textField[3]->getText().c_str() );
    }
}

int LoadMapBox::getMapWidth(){
    if( !isNumber(textField[4]->getText()) ){
        return 32;
    } else {
        return atoi( textField[4]->getText().c_str() );
    }
}

int LoadMapBox::getMapHeight(){
    if( !isNumber(textField[5]->getText()) ){
        return 32;
    } else {
        return atoi( textField[5]->getText().c_str() );
    }
}

bool LoadMapBox::isExclusive(){
    return playerExclusiveCheckBox->isSelected();
}



void LoadMapBox::logic(){
    //Displays the size of the map (px) at bottom of box
    std::string sizeStr = "Size: " +
        itos( getMapWidth()*getTileWidth() ) + "x" +
        itos( getMapHeight()*getTileHeight() ) + " (px)";

    label[7]->setCaption( sizeStr );
}
