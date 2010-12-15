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


#ifndef MAPTILE_HPP
#define MAPTILE_HPP

//  MapTile.hpp

#include <string>
#include "Tile.hpp"


class MapTile : public Tile{
    public:
        static const int revision_ = 1;

        //  Tile attributes
        enum TileAttributes{
            REGULAR,
            BLOCK,
            WARP,
            HEAL,
            HURT,
            KILL,
            BAN
        };

        static bool validAttribute(int attrib);



        //  Default ctor
        MapTile();
        //  Conversion ctor from Tile
        MapTile(const Tile& source);
        //  Copy ctor
        //explicit MapTile(const MapTile& source);
        //  Standard ctor
        MapTile(const enum TileAttributes& newAttribute, const std::string& newWarpMapFilename, int newWarpX, int newWarpY);
        //  Default dtor
        ~MapTile();



        //  Returns X-coordinate of warp destination
        int getWarpX()const;
        //  Returns Y-coordinate of warp destination
        int getWarpY()const;
        //  Returns filename of map to warp to
        const std::string& getWarpMapFilename()const;
        //  Returns Attribute of tile
        enum TileAttributes getAttribute()const;



        //  Sets X-coordinate of warp destination
        void setWarpX(int warpX);
        //  Sets Y-coordinate of warp destination
        void setWarpY(int warpY);
        //  Sets filename of map to warp to
        void setWarpMapFilename(const std::string& newWarpMapFilename);
        //  Sets attribute of tile
        void setAttribute(const enum TileAttributes& newAttribute);


        //Returns a INFPacket containing the tile
        virtual bool savePacket(inp::INFPacket &pack)const;

        //  Loads data from a packet
        bool loadPacket( inp::INFPacket& pack );



    protected:
        int warpX;                      //  X-coordinate of warp destination
        int warpY;                      //  Y-coordinate of warp destination
        std::string warpMapFilename;    //  Filename of map to warp to
        enum TileAttributes attribute;  //  Attribute of tile
};




#endif
