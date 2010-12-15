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


#include "AnimToolBar.hpp"
#include "guichan.hpp"
#include "InfraellyImageLoader.hpp"
#include "Tileset.hpp"
#include "caches.hpp"

AnimToolBar::AnimToolBar(){
    //set caption of window
    setCaption("Select Animation Type");

    //load images
    InfraellyImageLoader loader;

    //load tileset buttons as tileset
    cache::tilesets.loadResource("tilesets/animatorButtons.xml");
    //store the pointer locally to make code more readble
    Tileset *btn_ptr = cache::tilesets.get("tilesets/animatorButtons.xml");

    //initialise buttons with tiles from above tileset
    typeItemBtn = new gcn::ImageButton(   loader.load( Tile(btn_ptr, 0, 0)  )   );
    typeCharBtn = new gcn::ImageButton(   loader.load( Tile(btn_ptr, 1, 0)  )   );
    typeObjectBtn = new gcn::ImageButton( loader.load( Tile(btn_ptr, 2, 0)  )   );
    typeClipBtn = new gcn::ImageButton(   loader.load( Tile(btn_ptr, 3, 0)  )   );

    // set sizes
    typeItemBtn->setFrameSize(0);
    typeCharBtn->setFrameSize(0);
    typeObjectBtn->setFrameSize(0);
    typeClipBtn->setFrameSize(0);

    //set othr properties
    typeItemBtn->setFocusable(false);
    typeCharBtn->setFocusable(false);
    typeObjectBtn->setFocusable(false);
    typeClipBtn->setFocusable(false);


    //position stuff
    typeItemBtn->setPosition(0, 0);
    typeCharBtn->setPosition(typeItemBtn->getX()+typeItemBtn->getWidth(), 0);
    typeObjectBtn->setPosition(typeCharBtn->getX()+typeCharBtn->getWidth(), 0);
    typeClipBtn->setPosition(typeObjectBtn->getX()+typeObjectBtn->getWidth(), 0);


    //set colours


    //add
    add(typeItemBtn);
    add(typeCharBtn);
    add(typeObjectBtn);
    add(typeClipBtn);

    resizeToContent();
}

AnimToolBar::~AnimToolBar(){
    //delete                // assign nul to pointers
    delete typeItemBtn;     typeItemBtn = NULL;
    delete typeCharBtn;     typeCharBtn = NULL;
    delete typeObjectBtn;   typeObjectBtn = NULL;
    delete typeClipBtn;     typeClipBtn = NULL;
}
