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


#ifndef SCREEN_HPP_INCLUDED
#define SCREEN_HPP_INCLUDED

//  Screen.hpp

#include <string>
#include <SDL/SDL.h>


class Screen{
    public:
        //  (re)Initialises the screen surface
        static bool init(int width, int height, int bpp, Uint32 flags);

        //  Calls init, but seems more natural
        static bool resize(int width, int height);
        static bool resize(int width, int height, int bpp);
        static bool resize(int width, int height, int bpp, Uint32 flags);


        //  Returns width, 640 if none
        static int getWidth();
        //  Returns Height, 480 if none
        static int getHeight();
        //  Returns Bpp, 32 if none
        static int getBpp();


        //  Clears screen to colour "colour"
        static void clear(Uint8 r, Uint8 g, Uint8 b);
        //  Clears screen to colour "colour"
        static void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        //  Flips video surface
        static void flip();
        //  Returns pointer to screen surface
        static SDL_Surface *getSurface();



        //  Returns true if in fullscreen mode
        static bool isFullscreen();
        //  Sets fullscreen mode
        static void setFullscreen(bool vaule);
        //  Toggles fullscreen mode
        static void toggleFullscreen();


        //  Returns true if the surface has been initialised
        static bool isReady();
        //  Returns pointer to information about the video hardware
        static const SDL_VideoInfo *getVideoInfo();


        //  Sets the game window's icon to "image"
        static void setIcon(SDL_Surface *image);
        //  Sets the game window's title text to caption
        static void setCaption(const std::string& newCaption);
        //  Returns the current caption
        static const std::string& getCaption();


        //  Returns true if cursor is visiable
        static bool isShowCursor();
        //  Sets whether to show cursor or not
        static void setShowCursor(bool vaule);
        //  Toggles show cursor
        static void toggleShowCursor();





    private:
        static SDL_Surface *screenSurface;
        static bool fullscreen;
        static const SDL_VideoInfo *info;
        static std::string caption;
};




#endif // SCREEN_HPP_INCLUDED
