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


#ifndef FONTS_HPP
#define FONTS_HPP

// fonts.hpp

#include <vector>

#include <SDL/SDL_ttf.h>
#include "guichan/contrib/sdl/sdltruetypefont.hpp"


namespace font{

    //load fonts
    void loadFonts();

    //frees preloaded fonts
    void freeFonts();


    //filenames of fonts
    extern std::string guiTitleFilename;
    extern std::string guiTitleFontFilename;

    extern std::string mainFontFilename;
    extern std::string creditFontFilename;

    //standard font sizes
    extern unsigned short stdChatFontSize;
    extern unsigned short stdGuiTitleFontSize;
    extern unsigned short stdGuiFontSize;


    //some preloaded fonts, sizes 0-75
    //please dont uses size 0 =_=
    //size 0 is only there for easy referencing. mainfont.at(1) = size1; get it?
    extern std::vector<TTF_Font *> mainFont;
    extern std::vector<TTF_Font *> creditFont;


    //these are guichan fonts
    extern std::vector<gcn::contrib::SDLTrueTypeFont *> guiFont;
    extern std::vector<gcn::contrib::SDLTrueTypeFont *> guiTitleFont;
}



#endif
