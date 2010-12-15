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


//  GameGui.cpp

#include "GameGui.hpp"



GameGui::GameGui(){
    init();
    //reset the position of all UI objects
    resetPositions();
}

GameGui::GameGui(SDL_Surface *screen){
    init();
    setTarget(screen);
    //reset the position of all UI objects
    resetPositions();
}


GameGui::GameGui(SDL_Surface *screen, Stats *statsSource){
    init();
    statBox->setStatsSource(statsSource);
    healthBox->setStatsSource(statsSource);
    setTarget(screen);
    //reset the position of all UI objects
    resetPositions();
}

GameGui::GameGui(SDL_Surface *screen, Character *characterSource){
    init();
    if( characterSource != NULL ){
        statBox->setCharacterSource(characterSource);
        statBox->setStatsSource(&characterSource->stats);
        healthBox->setCharacterSource(characterSource);
        healthBox->setStatsSource(&characterSource->stats);
        inventoryBox->setInventory(&characterSource->inventory);
    }

    setTarget(screen);
    //reset the position of all UI objects
    resetPositions();
}

GameGui::~GameGui(){
    //only delete stuff made in constructor
    delete statBox;
    delete chatBox;
    delete healthBox;
    delete inventoryBox;
}

void GameGui::init(){
    //disable tabbing
    setTabbingEnabled(0);

    //make statbox + chatbox
    statBox = new StatBox;
    chatBox = new ChatBox;
    healthBox = new HealthBox;
    inventoryBox = new InventoryBox;

    //put the gui objects into the container
    container->add(statBox);
    container->add(chatBox);
    container->add(healthBox);
    container->add(inventoryBox);
}

void GameGui::resetPositions(){
    BaseGui::resetPositions();

    //set co-ords
    // statbox
    // top left
    statBox->setX(0);
    statBox->setY(0);

    //health box
    //top right
    healthBox->setX( container->getWidth() - healthBox->getWidth() );
    healthBox->setY( 0 );


    // chatbox
    // if posible, stick the chat box in the bottom left corner of teh screen
    chatBox->setX( 0 );
    chatBox->setY( container->getHeight() - chatBox->getHeight() );

    //inventory
    //under the healthbox
    inventoryBox->setX( container->getWidth() - inventoryBox->getWidth() );
    inventoryBox->setY(healthBox->getY() + healthBox->getHeight() );
}





StatBox *GameGui::getStatBox()const{ return statBox; }

ChatBox *GameGui::getChatBox()const{ return chatBox; }

HealthBox *GameGui::getHealthBox()const{ return healthBox; }

InventoryBox *GameGui::getInventoryBox()const{ return inventoryBox; }
