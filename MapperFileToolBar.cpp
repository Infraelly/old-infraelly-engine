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


//  MapperFileToolBar.cpp

#include "MapperFileToolBar.hpp"
#include "InfraellyImageLoader.hpp"
#include "Tile.hpp"
#include "Tileset.hpp"
#include "caches.hpp"

using namespace std;


MapperFileToolBar::MapperFileToolBar(AreaMap *worldMap){
    setCaption("File");
    //setBaseColor( gcn::Color(170, 200, 245, alpha) );
    //setBackgroundColor( gcn::Color(216, 239, 255, alpha) );
    //setForegroundColor( gcn::Color(0 ,0 ,0 , alpha) );

    world = worldMap;


    InfraellyImageLoader *loader = dynamic_cast<InfraellyImageLoader *>(gcn::Image::getImageLoader());
    if( loader == NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Image Loader is not an InfraellyImageLoader object" << endl;
        return;
    }

    //load tileset buttons as tileset
    Tileset *btn_ptr = cache::tilesets.loadResource("tilesets/editorButtons.xml");

    //initialise buttons with tiles from above tileset
    newDocButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 0, 0) )   );
    saveDocButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 1, 0)  )   );
    saveAsButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 2, 0)  )   );
    openDocButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 3, 0) )   );
    reopenDocButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 4, 0)  )   );
    closeDocButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 5, 0)  )   );
    detailsButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 6, 0)  )   );


    //set positions
    newDocButton->setPosition(0, 0);
    saveDocButton->setPosition(newDocButton->getX()+newDocButton->getWidth(), 0);
    saveAsButton->setPosition(saveDocButton->getX()+saveDocButton->getWidth(), 0);
    openDocButton->setPosition(saveAsButton->getX()+saveAsButton->getWidth(), 0);
    reopenDocButton->setPosition(openDocButton->getX()+openDocButton->getWidth(), 0);
    closeDocButton->setPosition(reopenDocButton->getX()+reopenDocButton->getWidth(), 0);
    detailsButton->setPosition(closeDocButton->getX()+closeDocButton->getWidth(), 0);


    //set none to be focusable
    newDocButton->setFocusable( false );
    saveDocButton->setFocusable( false );
    saveAsButton->setFocusable( false );
    openDocButton->setFocusable( false );
    reopenDocButton->setFocusable( false );
    closeDocButton->setFocusable( false );
    detailsButton->setFocusable( false );


    //add the buttons to self
    add(newDocButton);
    add(saveDocButton);
    add(saveAsButton);
    add(openDocButton);
    add(reopenDocButton);
    add(closeDocButton);
    add(detailsButton);

    //resize window to fit stuffs
    resizeToContent();
}

MapperFileToolBar::~MapperFileToolBar(){
    delete newDocButton;
    delete saveDocButton;
    delete saveAsButton;
    delete openDocButton;
    delete reopenDocButton;
    delete closeDocButton;
    delete detailsButton;
}

gcn::ImageButton *MapperFileToolBar::getNewDocButton()const{ return newDocButton; }

gcn::ImageButton *MapperFileToolBar::getSaveDocButton()const{ return saveDocButton; }

gcn::ImageButton *MapperFileToolBar::getSaveAsButton()const{ return saveAsButton; }

gcn::ImageButton *MapperFileToolBar::getOpenDocButton()const{ return openDocButton; }

gcn::ImageButton *MapperFileToolBar::getReopenDocButton()const{ return reopenDocButton; }

gcn::ImageButton *MapperFileToolBar::getCloseDocButton()const{ return closeDocButton; }

gcn::ImageButton *MapperFileToolBar::getDetailsButton()const{ return detailsButton; }
