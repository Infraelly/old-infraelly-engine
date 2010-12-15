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


#ifndef ANIMFX_HPP_INCLUDED
#define ANIMFX_HPP_INCLUDED

//******************************************************************************
//
//                                  AnimFx
//
//  AnimFx details a single stage(or frame) of an object's special FX during
//  an animation.
//
//  Predominatly used within cutscenes, AnimFx will allow special FX such as
//  Flashing light, particles effects, screen tint, flash screen to colour,
//  fade screen to something.
//
//  Special FX' should inherrit from this class, and overide start() (to
//  initialise the effect; ie. timers and such); actions(); (this will perform
//  the bulk of the fx, being called until stop() is called (by user, or self);
//  and stop() which will perform any cleanup, prior to dtor.
//
//  It is crucial that actions() be non blocking, as these Fx must take place
//  concurrently with other animation events, in a single threaded environment.
//
//  NOTE: Call order:
//                + start();
//                  + run(); (until stop())
//                    - if active -
//                    + actions();
//                  + stop(); (once)
//
//******************************************************************************

#include <string>

class AnimFx{
    public:
        AnimFx();
        virtual ~AnimFx();

        std::string getId()const;
        void setId(const std::string& id);

        bool isActive()const;

        virtual void start() = 0;
        void run();
        void stop();


    protected:
        bool active_;
        std::string id_;

        virtual void actions() = 0;
};

#endif // ANIMFX_HPP_INCLUDED
