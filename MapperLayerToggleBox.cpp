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


//  MapperLayerToggleBox.cpp

#include "MapperLayerToggleBox.hpp"

#include <iostream>

using namespace std;


MapperLayerToggleBox::MapperLayerToggleBox(){
    setCaption("Layer");

    //initialise the title labels
    layerNumberLabel = new gcn::Label("No.");
    layerNameLabel = new gcn::Label("Name");
    viewLabel = new gcn::Label("Viu");
    viewAttributesLabel = new gcn::Label("ViuAt.");
    //initialise the middle labels
    itemLayerLabel = new InfraellyLabel("Item Layer");
    playerNpcLabel = new InfraellyLabel("Players & NPCS");
    infraellyGuiLabel = new InfraellyLabel("Infraelly Gui");

    //initialise layerControls
    for( unsigned int i = 0; i < 6; ++i ){
        layerControl[i] = new LayerControl("LAYER_SELECTORS", false);
        layerControl[i]->setFocusable( false );
        layerControl[i]->setNumber( i );
    };
    //set to be working on first layer
    layerControl[0]->setSelected( true );


    //set positions
    //layer no label to be 0,0
    layerNumberLabel->setPosition(5, 0);
    layerNameLabel->setPosition(layerNumberLabel->getX()+layerNumberLabel->getWidth() +25, 0);

    viewLabel->setPosition(layerNameLabel->getX()+layerNameLabel->getWidth() +55, 0);
    viewAttributesLabel->setPosition(viewLabel->getX()+viewLabel->getWidth() +10, 0);
    //layers 1,2
    layerControl[0]->setPosition( 0, viewLabel->getY()+viewLabel->getHeight() );
    layerControl[1]->setPosition( 0, layerControl[0]->getY()+layerControl[0]->getHeight() );
    //item layer label
    itemLayerLabel->setPosition( 0, layerControl[1]->getY()+layerControl[1]->getHeight() );
    //layers 3,4
    layerControl[2]->setPosition( 0, itemLayerLabel->getY()+itemLayerLabel->getHeight() );
    layerControl[3]->setPosition( 0, layerControl[2]->getY()+layerControl[2]->getHeight() );
    //player&NPC label
    playerNpcLabel->setPosition( 0, layerControl[3]->getY()+layerControl[3]->getHeight() );
    //layers 5,6
    layerControl[4]->setPosition( 0, playerNpcLabel->getY()+playerNpcLabel->getHeight() );
    layerControl[5]->setPosition( 0, layerControl[4]->getY()+layerControl[4]->getHeight() );
    //gui label
    infraellyGuiLabel->setPosition( 0, layerControl[5]->getY()+layerControl[5]->getHeight() );



    //  set opaque
    itemLayerLabel->setOpaque( true );
    playerNpcLabel->setOpaque( true );
    infraellyGuiLabel->setOpaque( true );
    //  set colours
    itemLayerLabel->setBackgroundColor( gcn::Color(10, 50,200) );
    playerNpcLabel->setBackgroundColor( gcn::Color(10, 50,200) );
    infraellyGuiLabel->setBackgroundColor( gcn::Color(10, 50,200) );
    itemLayerLabel->setForegroundColor( gcn::Color(255, 255, 255) );
    playerNpcLabel->setForegroundColor( gcn::Color(255, 255, 255) );
    infraellyGuiLabel->setForegroundColor( gcn::Color(255, 255, 255) );



    //set no focus
    layerNumberLabel->setFocusable( false );
    layerNameLabel->setFocusable( false );
    viewLabel->setFocusable( false );

    itemLayerLabel->setFocusable( false );
    playerNpcLabel->setFocusable( false );
    infraellyGuiLabel->setFocusable( false );



    //add the buttons to self
    add(layerNumberLabel);
    add(layerNameLabel);
    add(viewLabel);
    add(viewAttributesLabel);
    add(itemLayerLabel);
    add(playerNpcLabel);
    add(infraellyGuiLabel);

    for( unsigned int i = 0; i < 6; ++i ){
        add(layerControl[i]);
    };

    //resize window to fit stuffs
    resizeToContent();


    //  resize labels' widths
    itemLayerLabel->setWidth( getWidth() );
    playerNpcLabel->setWidth( getWidth() );
    infraellyGuiLabel->setWidth( getWidth() );
    //  set text orientation
    itemLayerLabel->setAlignment(gcn::Graphics::CENTER);
    playerNpcLabel->setAlignment(gcn::Graphics::CENTER);
    infraellyGuiLabel->setAlignment(gcn::Graphics::CENTER);
}

MapperLayerToggleBox::~MapperLayerToggleBox(){
    delete layerNumberLabel;
    delete layerNameLabel;
    delete viewLabel;

    delete itemLayerLabel;
    delete playerNpcLabel;
    delete infraellyGuiLabel;

    for( unsigned int i = 0; i < 6; ++i ){
        delete layerControl[i];
    };
}


LayerControl *MapperLayerToggleBox::getLayerControl(int layerNumber)const{
    if( (layerNumber > 6) || (layerNumber < 0)  ){ return NULL; };
    return layerControl[layerNumber];
}




int MapperLayerToggleBox::getWorkingLayer()const{
    for( unsigned int i = 0; i < 6; ++i ){
        if( layerControl[i]->isSelected() ){ return i; };
    }
    std::cerr << __FILE__ << " " << __LINE__ << ": " << "MapperLayerToggleBox->workingLayerBox[] :none active" << std::endl;
    return 0;
}


bool MapperLayerToggleBox::isView(int layerNumber)const{
    if( layerNumber >= 0 && layerNumber < 6 ){
        return layerControl[layerNumber]->isView();
    } else {
        std::cerr << __FILE__ << " " << __LINE__ << ": " << "MapperLayerToggleBox was passed a layerNumber out of layerViewBox[] range: " << layerNumber << std::endl;
        return 0;
    }
}

bool MapperLayerToggleBox::isViewAttributes(int layerNumber)const{
    if( layerNumber >= 0 && layerNumber < 6 ){
        return layerControl[layerNumber]->isViewAttribute();
    } else {
        std::cerr << __FILE__ << " " << __LINE__ << ": " << "MapperLayerToggleBox was passed a layerNumber out of layerViewBox[] range: " << layerNumber << std::endl;
        return 0;
    }
}
