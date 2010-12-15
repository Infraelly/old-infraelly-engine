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


#ifndef MAPPERTILESETBOX_HPP
#define MAPPERTILESETBOX_HPP

//  MapperTilesetBox.hpp

#include <string>

#include "guichan.hpp"

#include "InfraellyWindow.hpp"
#include "InfraellyScroller.hpp"
#include "TilesetList.hpp"

#include "MapTile.hpp"
#include "Tileset.hpp"

#include "AreaMap.hpp"
#include "TilesetIcon.hpp"


class MapperTilesetBox : public InfraellyWindow,
                         public gcn::SelectionListener
{
    public:
        MapperTilesetBox();
        MapperTilesetBox(AreaMap *newWorld);
        ~MapperTilesetBox();

        void loadTileset(Tileset *tileset);
        void setTilesetPreview(Tileset *tileset);

        void setMap(AreaMap *newWorld);
        void resetPositions();
        void setWidth(int width);
        void setHeight(int height);

        AreaMap *getWorld()const;
        MapTile getSelectedTile()const;
        Tileset *getSelectedTileset()const;
        const Tileset *getAttribTileset()const;


        gcn::DropDown *getTilesetDropDown()const;
        gcn::ImageButton *getMakeTilesetButton()const;
        gcn::ImageButton *getLoadTilesetButton()const;
        gcn::ImageButton *getCloseTilesetButton()const;
        gcn::ImageButton *getTilesetDetailsButton()const;


        void freeTilesets();

        //mouse listener
        virtual void mouseClicked(gcn::MouseEvent& mouseEvent);
        //selection listener, for the drop down
        virtual void valueChanged(const gcn::SelectionEvent& event);

        virtual void draw(gcn::Graphics* graphics);


    private:
        void init();

        AreaMap *world;

        Tileset *attribTs;
        MapTile selectedTile;
        MapTile::TileAttributes attribGrid[2][3];

        gcn::ImageButton *newTilesetButton;
        gcn::ImageButton *loadTilesetButton;
        gcn::ImageButton *tilesetDetailsButton;
        gcn::ImageButton *closeTilesetButton;

        TilesetList *tilesetList;
        gcn::DropDown *tilesetDropDown;

        TilesetIcon* tilesetImage;
        InfraellyScroller* tilesetImageScroller;
};




#endif

