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


//  MapDetailsBox.cpp

#include "MapDetailsBox.hpp"
//string proccessing and validation
#include "globalFunc.hpp"


MapDetailsBox::MapDetailsBox(AreaMap *newInfoSource) {
    infoSource = newInfoSource;
    for( unsigned int i = 0; i < 6; ++i ){
        textField[i]->setEnabled( false );
    };
    playerExclusiveCheckBox->setEnabled( false );
    cancelButton->setVisible( false );
}

void MapDetailsBox::setInfoSource(AreaMap *newInfoSource) {
    infoSource = newInfoSource;
}

AreaMap *MapDetailsBox::getInfoSource()const {
    return infoSource;
}

void MapDetailsBox::logic(){
    LoadMapBox::logic();

    if( infoSource != NULL ){
        setCaption( ("Map Info - " + infoSource->getMapName()) );
        textField[0]->setText( infoSource->getFilename() );
        textField[1]->setText( infoSource->getMapName() );
        if( !infoSource->getLayer(0).empty() ){
            textField[2]->setText( itos(infoSource->getLayer(0).getTileWidth()) );
            textField[3]->setText( itos(infoSource->getLayer(0).getTileHeight()) );
            textField[4]->setText( itos(infoSource->getLayer(0).getWidth()) );
            textField[5]->setText( itos(infoSource->getLayer(0).getHeight()) );
        } else {
            textField[2]->setText( "-----" );
            textField[3]->setText( "-----" );
            textField[4]->setText( "-----" );
            textField[5]->setText( "-----" );
        }
        playerExclusiveCheckBox->setSelected( infoSource->isExclusive() );
    } else {
        setCaption("Map Info");
        textField[0]->setText( "-----" );
        textField[1]->setText( "-----" );
        textField[2]->setText( "-----" );
        textField[3]->setText( "-----" );
        textField[4]->setText( "-----" );
        textField[5]->setText( "-----" );
        playerExclusiveCheckBox->setSelected( false );
    }
}
