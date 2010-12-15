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


#ifndef ANIMATORGUI_HPP_INCLUDED
#define ANIMATORGUI_HPP_INCLUDED

//  AnimatorGui.hpp

#include <SDL/SDL.h>

#include "BaseGui.hpp"


class AnimToolBar;
class ItemAnimBox;
class CharacterAnimBox;
class MakeTSpriteBox;
class LoadTSpriteBox;
class LoadTilesetBox;
class MakeTilesetBox;



class AnimatorGui : public BaseGui,
                    public gcn::WidgetListener,
                    public gcn::SelectionListener
{
    public:
        AnimatorGui();
        AnimatorGui(SDL_Surface *screen);
        ~AnimatorGui();


        //  Resets positions of widgets
        void resetPositions();


        //  Used for geting info from popups
        //  info is collected when  widgets are hideen
        void widgetHidden(const gcn::Event& event);
        //  Listener, for the drop down (tileset info)
        void valueChanged(const gcn::SelectionEvent& event);
        //mouse listener
        void mouseClicked(gcn::MouseEvent& mouseEvent);


    private:
        void init();                    //  Initialises object

        AnimToolBar *newAnimBox_;
        ItemAnimBox *itemAnimBox_;
        CharacterAnimBox *charAnimBox_;
        MakeTSpriteBox *makeTSpriteBox_;
        LoadTSpriteBox *loadTSpriteBox_;
        LoadTilesetBox *loadTilesetBox_;
        MakeTilesetBox *makeTilesetBox_;
};



#endif // ANIMATORGUI_HPP_INCLUDED
