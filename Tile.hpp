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


#ifndef TILE_HPP
#define TILE_HPP

#include <SDL/SDL.h>
#include <tinyxml.h>
#include "Tileset.hpp"


namespace inp{ class INFPacket; }


class Tile{
    public:
        static const int revision_ = 1;

        //  ctor
        Tile();
        explicit Tile(Tileset *srcTileset);
        explicit Tile(Tileset *srcTileset, unsigned int row, unsigned int col);
        virtual ~Tile();



        //  Returns the Tileset the current tile is referencing
        Tileset *getTileset()const;
        //  Returns which row in the tileset the tile is in
        unsigned int getRow()const;
        //  Returns which column in the tileset the tile is in
        unsigned int getCol()const;
        //  Returns the size of the tile (px)
        const SDL_Rect& getSize()const;
        //  Returns the x-coordinate of the tile, on the tileset
        unsigned int getX()const;
        //  Returns the y-coordinate of the tile, on the tileset
        unsigned int getY()const;
        //  Returns the width tile
        unsigned int getWidth()const;
        //  Returns the height tile
        unsigned int getHeight()const;



        //  Sets which column the tile is in the tileset
        void setCol(unsigned int);
        //  Sets which row the tile is in the tileset
        void setRow(unsigned int);
        //  Sets which Tileset tile should reference
        void setTileset(Tileset *newTileset);
        //  Sets the source of the tile
        void setSource(Tileset *newTileset, unsigned int col, unsigned int row);



        //  Resets tile
        void clear();


        //Returns false on fail
        virtual bool savePacket(inp::INFPacket& pack)const;

        //  Loads data from a packet
        virtual bool loadPacket( inp::INFPacket& pack );


        //  Draws tile to "dest" at (x,y)
        virtual void draw(SDL_Surface *dest, int x, int y, bool blend = 1);




    protected:
        Tileset *srcTileset;        //  The Tilesets the tile references
        SDL_Rect tileSize;          //  The dimensions of the tile (px)
        int row;                    //  Which row in the tileset the tile is in
        int col;                    //  Which column in the tileset the tile is in
};



#endif
