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


#include "MapperContext.hpp"

MapperContext::MapperContext()  :
    world(new AreaMap),
    gui(new MapperGui(screen, world))
{
    // Set window caption
    SDL_WM_SetCaption("Excogitator (Infraelly Map Editor)", NULL);
    //set to not wait (instead of poll) for events
    poll_events = false;

    run();
}

MapperContext::~MapperContext(){
    delete gui;
    delete world;
}



void MapperContext::events(SDL_Event &event){
    if( (event.type == SDL_VIDEORESIZE) || (event.key.keysym.sym == SDLK_F4) ){
        // Window has been resized
        gui->resetPositions();
    }
    gui->pushInput(event);
}

void MapperContext::input(Uint8 *keys){
}

void MapperContext::logic(){
    // let giuchan do its thang!
    gui->logic();
}

void MapperContext::draw(){
    //clear
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,255,255,255));
    //let guichan draw on the screen =]
    gui->draw();
}
