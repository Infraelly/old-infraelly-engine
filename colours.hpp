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


#ifndef COLOURS_HPP
#define COLOURS_HPP

// colours.hpp


#include <SDL/SDL.h>

#include "guichan.hpp"

namespace colour{
    //some predefined colours

    //SDL colours
    extern SDL_Colour white;
    extern SDL_Colour black;

    extern SDL_Colour red;
    extern SDL_Colour green;
    extern SDL_Colour blue;

    extern SDL_Colour yellow;
    extern SDL_Colour pink;
    extern SDL_Colour l_blue;

    extern SDL_Colour purple;
    extern SDL_Colour grey;


    //Guichan colours
    extern gcn::Color guiTransparent;

    extern gcn::Color guiWhite;
    extern gcn::Color guiBlack;

    extern gcn::Color guiRed;
    extern gcn::Color guiGreen;
    extern gcn::Color guiBlue;

    extern gcn::Color guiYellow;
    extern gcn::Color guiPink;
    extern gcn::Color guiL_blue;

    extern gcn::Color guiPurple;
    extern gcn::Color guiGrey;
}

#endif
