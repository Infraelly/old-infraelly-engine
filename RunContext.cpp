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


#include "RunContext.hpp"

#include "Screen.hpp"

RunContext::RunContext() :
    screen(Screen::getSurface()),
    done(false),
    poll_events(true),
    fpsManager(30)
{ }

void RunContext::sysKeys(SDL_Event &event){
    switch(event.type){
        // User requests to exit
        case SDL_QUIT:
            done = 1;
            break;

        // Window has been resized
        case SDL_VIDEORESIZE:
            if( Screen::resize(event.resize.w, event.resize.h) == 0 ){
                done = 1;
            }
            break;

        // A key has been pressed
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ){
                case SDLK_ESCAPE:
                    //exit
                    done = 1;
                    break;

                case SDLK_F4:
                    //fullscreen toggle
                    Screen::toggleFullscreen();
                    break;
            }
            break;

        default:
            return;
    }
}

void RunContext::run(){
    while( !done ){
        fpsManager.startFrame();

        SDL_Event event;
        if( poll_events ){
            while( SDL_PollEvent(&event) && !done ){
                sysKeys(event);
                events(event);
            }
        } else {
            SDL_WaitEvent(&event);
            sysKeys(event);
            events(event);
        }
        input( SDL_GetKeyState(NULL) );
        logic();
        draw();
        SDL_Flip(screen);

        fpsManager.endFrame();
    }
}
