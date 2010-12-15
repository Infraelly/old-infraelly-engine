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


//  Notification.cpp

#include "Notification.hpp"
//center
#include "globalFunc.hpp"


Notification::Notification(std::string caption, std::string newBodyText){
    //initialise
    setCaption( caption );
    bodyText = new gcn::Label( newBodyText );

    setVisible( false );

    removeViewControls();

    //add widgets to window
    add(bodyText);

    resetPositions();
}


void Notification::setBodyText(const std::string& newBodyText){
    bodyText->setCaption( newBodyText );
    resetPositions();
}


void Notification::resetPositions(){
    //position the elements
    //Ajust size of label
    bodyText->adjustSize();
    bodyText->setAlignment(gcn::Graphics::CENTER);
    setWidth(bodyText->getWidth() + 2*getFrameSize() + 20);

    //make the window a standard size
    if( getWidth() < 150 ){
        setWidth(150);
    }
    setHeight(100);

    //label y at 15
    bodyText->setY(15);
    //center label x
    bodyText->setX( centerX(bodyText->getWidth(), getWidth()) );
}


Notification::~Notification(){
    delete bodyText;
}
