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


#include "TilesetDetailsBox.hpp"

//  TilesetDetailsBox.cpp

#include "globalFunc.hpp"


TilesetDetailsBox::TilesetDetailsBox(Tileset *infoSource){
    source = infoSource;

    for( unsigned int i = 0; i < 6; ++i ){
        textField[i]->setBackgroundColor( gcn::Color( 255, 255, 255 ) );
        textField[i]->setText("-----");
        textField[i]->setEnabled( false );
    }

    //overide the text of the done button
    doneButton->setCaption("Ok");
    doneButton->adjustSize();

    //overide the cancel button to get rid of it
    cancelButton->setEnabled( false );
    cancelButton->setVisible( false );
}




Tileset *TilesetDetailsBox::getInfoSource()const{ return source; }

void TilesetDetailsBox::setInfoSource(Tileset *infoSource){ source = infoSource; }




void TilesetDetailsBox::logic(){
    if( source != NULL ){
        textField[0]->setText( source->getFilename() );
        textField[1]->setText( source->getAlias() );
        textField[2]->setText( itos(source->getTileWidth()) );
        textField[3]->setText( itos(source->getTileHeight()) );
        textField[4]->setText( itos(source->getXOffset()) );
        textField[5]->setText( itos(source->getYOffset()) );
    } else {
        textField[0]->setText("-----");
        textField[1]->setText("-----");
        textField[2]->setText("-----");
        textField[3]->setText("-----");
        textField[4]->setText("-----");
        textField[5]->setText("-----");
    }
}
