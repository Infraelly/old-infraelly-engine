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


//  MapTile.cpp

#include "MapTile.hpp"

#include "INFPacket.hpp"
#include "globalFunc.hpp"

using namespace std;

bool MapTile::validAttribute(int attrib){
    return (  (attrib==REGULAR) || (attrib==BLOCK) || (attrib==WARP) ||
              (attrib==HEAL)    || (attrib==HURT)  || (attrib==KILL) ||
              (attrib==BAN)  );
}


MapTile::MapTile() :
    warpX(0),
    warpY(0),
    warpMapFilename(""),
    attribute(REGULAR)
{}

MapTile::MapTile(const Tile& source) :
    warpX(0),
    warpY(0),
    warpMapFilename(""),
    attribute(REGULAR)
{
    srcTileset = source.getTileset();
    row = source.getRow();
    col = source.getCol();
    tileSize = source.getSize();
}

MapTile::MapTile( const enum TileAttributes& newAttribute,
                  const std::string& newWarpMapFilename,
                  int newWarpX,
                  int newWarpY ) :
    warpX(newWarpX),
    warpY(newWarpY),
    warpMapFilename(newWarpMapFilename),
    attribute(newAttribute)
{}


MapTile::~MapTile(){}


int MapTile::getWarpX()const {
    return warpX;
}

int MapTile::getWarpY()const {
    return warpY;
}

const std::string& MapTile::getWarpMapFilename()const {
    return warpMapFilename;
}

enum MapTile::TileAttributes MapTile::getAttribute()const {
    return attribute;
}



void MapTile::setWarpX(int newWarpX) {
    warpX = newWarpX;
}

void MapTile::setWarpY(int newWarpY) {
    warpY = newWarpY;
}

void MapTile::setWarpMapFilename(const std::string& newWarpMapFilename) {
    warpMapFilename = newWarpMapFilename;
}

void MapTile::setAttribute(const enum MapTile::TileAttributes& newAttribute) {
    attribute = newAttribute;
}


//Returns a INFPacket containing the tile
bool MapTile::savePacket(inp::INFPacket& pack)const{
    //  All we do is add an extra section to the Tile's save block
    pack << revision_;

    Tile::savePacket(pack);

    //Attributes
	pack << static_cast<Uint16>(attribute);

    //Only write warp block if needed
    if( attribute == WARP ){
        //warp file
        pack << warpMapFilename;
        //warp position
        pack << warpX;
        pack << warpY;
    }
    return true;
}

//  Loads data from a packet
bool MapTile::loadPacket( inp::INFPacket& pack ){
    if( pack.empty() || pack.readDone() ){ return 0; }

    {
        int fileVer;
        pack >> fileVer;
        if( fileVer != revision_ ){
            std::cerr << __FILE__ << __LINE__ << "incompatible file version" << std::endl;
            return 0;
        }
    }

    //load the base tile
    Tile::loadPacket( pack );

    //Attributes
    Uint16 atrib;
	pack >> atrib;
	if( !validAttribute(atrib) ){ return 0; }
	attribute = static_cast<TileAttributes>(atrib);
	if( pack.readDone() ){ return 0; }

    //Only write warp block if needed
    if( attribute == WARP ){
        //warp file
        pack >> warpMapFilename;
        if( pack.readDone() ){ return 0; }
        //warp position
        pack >> warpX;
        if( pack.readDone() ){ return 0; }
        pack >> warpY;
    }

    return true;
}
