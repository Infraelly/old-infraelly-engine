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


#ifndef RUNCONTEXT_HPP_INCLUDED
#define RUNCONTEXT_HPP_INCLUDED

#include <SDL/SDL.h>
#include "FpsManager.hpp"

/*******************************************************************************

                                RunContext

    This class is an abstraction of the "Game Loop" concept.
    It allows easy and uniform creation and use of different game loops.

    To pass an argument to a game loop, a private member variable of the
    appropriate data type should be defined. The argument should be passed to
    a specialised constuctor, which stores the value into the private member.

    The constuctor should call run() at the end of its 'constructing'.

    Run always opperates in such an order:
    events, input, logic, draw, (frame limit).

    Control vairbles: the FPS, and wether or not to poll events should be set
    int the Ctor.

    All game loops should inherit from this bas class.


*******************************************************************************/


class RunContext{
    public:
        RunContext();
        virtual ~RunContext(){};

    protected:
        void run();
        virtual void events(SDL_Event &event) = 0;
        virtual void input(Uint8 *keys) = 0;
        virtual void logic() = 0;
        virtual void draw() = 0;

        //  The surface on which drawing occurs
        SDL_Surface *screen;
        //  Causes run to return
        bool done;
        //  True to poll events, false to sleep until input is recieved
        bool poll_events;
        //  Set fps to -1 to disable
        FpsManager fpsManager;


    private:
        void sysKeys(SDL_Event &event);
};


#endif // RunContext_HPP_INCLUDED
