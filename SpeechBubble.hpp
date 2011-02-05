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


#ifndef SPEECHBUBBLE_HPP_INCLUDED
#define SPEECHBUBBLE_HPP_INCLUDED


//  SpeechBubble.hpp


#include <SDL/SDL.h>
#include <string>

#include "Tile.hpp"
#include "Timer.hpp"

/*******************************************************************************

                            SpeechBubble

    Makes a little bubble around text, for character speech; and maybe popup
    help dialogues.

*******************************************************************************/


class SpeechBubble{
    public:
        SpeechBubble(const std::string& newText = " ");
        SpeechBubble(const SpeechBubble &src);
        SpeechBubble& operator=(const SpeechBubble& rhs);
        ~SpeechBubble();

        void setText(const std::string& newText);
        const std::string& getText()const;

        int getWidth()const;
        int getHeight()const;

        void draw(SDL_Surface* dest, int x, int y);



    private:
        static bool firstInstance_;

        // how long the bubble will stay visible (ms)
        static const long timeout_ = 5000;
        Timer visibleTimer_;

        //Text that will be drawn in the bubble
        std::string text_;
        //Image of the bubble with frame and background
        SDL_Surface *composedImage_;

        //image
        static Tileset tileset_;

        //side images
        static Tile top_;
        static Tile bottom_;
        static Tile left_;
        static Tile right_;

        //center, ie background
        static Tile center_;

        //corner images
        static Tile TL_;
        static Tile TR_;
        static Tile BL_;
        static Tile BR_;


        //Composes an image of the bubble with text and frame
        void makeBubble();
};




#endif // SPEECHBUBBLE_HPP_INCLUDED
