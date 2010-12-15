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


#include <iostream>
//  centering
#include "globalFunc.hpp"


//  BaseGui.cpp

#include "BaseGui.hpp"



BaseGui::BaseGui(){
    init();
}


BaseGui::BaseGui(SDL_Surface *screen){
    init();

    graphics->setTarget(screen);
}

BaseGui::~BaseGui(){
    delete input;
    delete graphics;
    delete container;
    delete notification;
    delete alertBox;

    input = NULL;
    graphics = NULL;
    container = NULL;
    customContainer = NULL;
    notification = NULL;
    alertBox = NULL;
}

void BaseGui::init(){
    customContainer = NULL;
    baseContainer = false;

    //  make an SDLgraphics for gui chan to draw to
    graphics = new gcn::SDLGraphics;
    //  make an SDLInput so the gui can get input
    input = new gcn::SDLInput;
    //  assign the gaphics and input objects to teh gui
    setGraphics(graphics);
    setInput(input);

    //make the container and set it as the base widget
    container = new gcn::Container;
    container->setOpaque( false );
    setTop(container);

    //make notyfier widgets
    notification = new Notification;
    alertBox = new AlertBox;

    //add the notyfiers to the container
    container->add(notification);
    container->add(alertBox);

    //set the positions of the widgets
    resetPositions();
}



void BaseGui::resetPositions(){
    if( graphics->getTarget() != NULL ){
        if( baseContainer ){
            //container (size of screen)
            container->setWidth( graphics->getTarget()->w );
            container->setHeight( graphics->getTarget()->h );
        } else {

            if( customContainer != NULL ){
                customContainer->setWidth( graphics->getTarget()->w );
                customContainer->setHeight( graphics->getTarget()->h );
            } else {
                baseContainer = 1;
            }

        }
    } else {
        if( baseContainer ){
            container->setWidth( 640 );
            container->setHeight( 800 );
        } else {

            if( customContainer != NULL ){
                customContainer->setWidth( 640 );
                customContainer->setHeight( 800 );
            } else {
                baseContainer = 1;
            }

        }
    }
    notification->resetPositions();
    alertBox->resetPositions();
}





void BaseGui::alert(const std::string& message){
    container->moveToTop(alertBox);
    alertBox->requestFocus();
    alertBox->setBodyText( message );
    alertBox->resetPositions();
    alertBox->setVisible( true );
    //  Center on screen
    alertBox->setX( centerX(alertBox->getWidth(), getContainer()->getWidth()) );
    alertBox->setY( centerY(alertBox->getHeight(), getContainer()->getHeight()) );
}


void BaseGui::notify(const std::string& message){
    container->moveToTop(notification);
    notification->requestFocus();
    notification->setBodyText( message );
    notification->resetPositions();
    notification->setVisible( true );
    //  Center on screen
    notification->setX( centerX(notification->getWidth(), getContainer()->getWidth()) );
    notification->setY( centerY(notification->getHeight(), getContainer()->getHeight()) );
}

void BaseGui::unNotify(){
    notification->setVisible( false );
}







const gcn::Container *BaseGui::getContainer()const{
    if( baseContainer ){
        return container;
    } else {
        return customContainer;
    }
}

const gcn::Container *BaseGui::getBaseContainer()const{ return container; }


void BaseGui::setContainer(gcn::Container *newContainer){
    if( newContainer != NULL){
        customContainer = newContainer;
        if( newContainer == container ){
            //set old container
            setTop(container);
            baseContainer = 1;
        } else {
            //set new container
            setTop(customContainer);
            //add the notifiers
            customContainer->add(notification);
            customContainer->add(alertBox);
            baseContainer = 0;
        }
        //reset the position of all UI objects
        resetPositions();
    }
}





void BaseGui::add(gcn::Widget *wgt){
    if( baseContainer ){
        container->add(wgt);
    } else {
        if( customContainer != NULL ){ customContainer->add(wgt); }
    }
}




void BaseGui::setTarget(SDL_Surface *screen){
    graphics->setTarget(screen);
    resetPositions();
}

void BaseGui::pushInput(SDL_Event event){
    input->pushInput(event);
}
