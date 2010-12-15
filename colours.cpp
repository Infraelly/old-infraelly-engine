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


// colours.cpp

#include <SDL/SDL.h>

#include "guichan.hpp"

namespace colour{

    //SDL Colours
    SDL_Colour white = {255, 255, 255};
    SDL_Colour black = {0, 0, 0};

    SDL_Colour red = {255, 0, 0};
    SDL_Colour green = {0, 255, 0};
    SDL_Colour blue = {0, 0, 255};

    SDL_Colour yellow = {255, 255, 0};
    SDL_Colour pink = {255, 0, 255};
    SDL_Colour l_blue = {0, 255, 255};

    SDL_Colour purple = {180, 0, 255};
    SDL_Colour grey = {200, 200, 200};


    //Guichan colours
    gcn::Color guiTransparent(0, 0, 0, 0);

    gcn::Color guiWhite(255, 255, 255, 255);
    gcn::Color guiBlack(0, 0, 0, 255);

    gcn::Color guiRed(255, 0, 0, 255);
    gcn::Color guiGreen(0, 255, 0, 255);
    gcn::Color guiBlue(0, 0, 255, 255);

    gcn::Color guiYellow(255, 255, 0, 255);
    gcn::Color guiPink(255, 0, 255, 255);
    gcn::Color guiL_blue(0, 255, 255, 255);

    gcn::Color guiPurple(180, 0, 255, 255);
    gcn::Color guiGrey(200, 200, 200, 255);
}
