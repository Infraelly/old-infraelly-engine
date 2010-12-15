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


//  FilenameBox.cpp

#include "FilenameBox.hpp"
#include "globalFunc.hpp"

FilenameBox::FilenameBox(std::string caption){
    //initialise
    infoReady = 0;
    setCaption( caption );
    input = new gcn::TextField("");
    bodyText = new gcn::Label( "Filename: " );

    //add widgets to window
    add(bodyText);
    add(input);

    resetPositions();
}

void FilenameBox::resetPositions(){
    //make the window a standard size
    setDimension( gcn::Rectangle(0,0,450,100) );

    //position the elements
    bodyText->setX( 0 );
    //input field
    input->setX( bodyText->getX() + bodyText->getWidth() +2 );
    input->setWidth( getWidth() - (input->getX()+10) );

    //buttons
    cancelButton->setX( getWidth() - cancelButton->getWidth() -10 );
    doneButton->setX( cancelButton->getX() - doneButton->getWidth() );

    //label y near center
    bodyText->setY( 30 );
    input->setY( 30 );

    //button right bottom
    doneButton->setY( getHeight() - doneButton->getHeight() -20);
    doneButton->setFrameSize(0);
    cancelButton->setY( doneButton->getY() );
    cancelButton->setFrameSize(0);

    //put window in center
    setPosition((1024+getWidth())/4, (764+getHeight())/4);
}


FilenameBox::~FilenameBox(){
    delete bodyText;
    delete input;
}


void FilenameBox::setFilename(const std::string& text){ input->setText(correctFilepath(text)); }

std::string FilenameBox::getFilename()const{ return input->getText(); }
