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


//  InfoForm.cpp

#include "InfoForm.hpp"

//string proccessing and validation
#include "globalFunc.hpp"


using namespace std;

InfoForm::InfoForm() :
    //initialise buttons
    doneButton(new gcn::Button("Ok")),
    cancelButton(new gcn::Button("Cancel")),
    infoReady(false)
{
    setCaption("Info Form");
    //cant tab out of form
    setTabOutEnabled(false);
    setVisible( false );

    //add self as listeners to the buttons
    doneButton->addMouseListener(this);
    cancelButton->addMouseListener(this);

    //remove collapse button
    removeViewControls();
    //remove frame
    doneButton->setFrameSize(0);
    cancelButton->setFrameSize(0);
    //unfocusable
    doneButton->setFocusable(false);
    cancelButton->setFocusable(false);

    add(doneButton);
    add(cancelButton);
}

InfoForm::~InfoForm(){
    delete doneButton;
    delete cancelButton;
}


void InfoForm::infoTaken(){ infoReady = 0; }

bool InfoForm::isInfoReady()const{ return infoReady; }


void InfoForm::positionDefaultBtns(){
    setHeight(getHeight() + cancelButton->getHeight() + 10);

    doneButton->setPosition(5, getHeight()-(doneButton->getHeight()+20));

    cancelButton->setPosition( getWidth()-(cancelButton->getWidth()+10),
                               getHeight()-(cancelButton->getHeight()+20) );
}


void InfoForm::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    //click from the ok
    if( mouseEvent.getSource() == doneButton ){
        infoReady = 1;
        setVisible(0);
        doneAction();
    }

    //click from the ok
    if( mouseEvent.getSource() == cancelButton ){
        infoReady = 0;
        setVisible(0);
        cancelAction();
    }
}
