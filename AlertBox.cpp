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


// center
#include "globalFunc.hpp"

//  AlertBox.cpp

#include "AlertBox.hpp"

AlertBox::AlertBox(std::string caption, std::string newBodyText){
    //initialise
    setCaption( caption );
    okButton = new gcn::Button("Ok");
    okButton->setFrameSize(0);
    bodyText->setCaption( newBodyText );

    //add widgets to window
    add(okButton);

    resetPositions();
}

void AlertBox::resetPositions(){
    //position the elements
    //Ajust size of label
    bodyText->adjustSize();
    bodyText->setAlignment(gcn::Graphics::CENTER);
    setWidth(bodyText->getWidth());

    //make the window a standard size
    if( getWidth() < 150 ){
        setWidth(150);
    }
    setHeight(100);

    //label y at 15
    bodyText->setY(15);
    //center label x
    bodyText->setX( centerX(bodyText->getWidth(), getWidth()) );

    //center button x
    okButton->setX( centerX(okButton->getWidth(), getWidth()) );
    //bottom y at 30 from bottom
    okButton->setY( getHeight() - (okButton->getHeight()+getTitleBarHeight()+10));
}


AlertBox::~AlertBox(){
    delete okButton;
}


void AlertBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    //click from the ok button
    if( mouseEvent.getSource() == okButton ){
        setVisible( false );
    }
}
