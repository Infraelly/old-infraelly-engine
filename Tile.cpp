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


//  Tile.cpp

#include "Tile.hpp"
#include "globalFunc.hpp"
#include "caches.hpp"

#include "INFPacket.hpp"

using namespace std;


Tile::Tile() :
    srcTileset(NULL),
    row(0),
    col(0)
{
    tileSize.x = 0;
    tileSize.y = 0;
    tileSize.w = 0;
    tileSize.h = 0;
}


Tile::Tile(Tileset *newSrcTileset) :
    srcTileset(newSrcTileset),
    row(0),
    col(0)
{
    tileSize.x = 0;
    tileSize.y = 0;
    tileSize.w = 0;
    tileSize.h = 0;
}


Tile::Tile(Tileset *newSrcTileset, unsigned int newCol, unsigned int newRow) :
    srcTileset(newSrcTileset)
{
    tileSize.x = 0;
    tileSize.y = 0;
    tileSize.w = 0;
    tileSize.h = 0;

    setRow(newRow);
    setCol(newCol);
}

Tile::~Tile(){}



void Tile::clear(){
    srcTileset = NULL;
    tileSize.x = 0;
    tileSize.y = 0;
    tileSize.w = 0;
    tileSize.h = 0;
    row = 0;
    col = 0;
}




Tileset* Tile::getTileset()const{ return srcTileset; }

unsigned int Tile::getRow()const{ return row; }

unsigned int Tile::getCol()const{ return col; }

const SDL_Rect& Tile::getSize()const{ return tileSize; }

unsigned int Tile::getWidth()const{ return tileSize.w; }

unsigned int Tile::getHeight()const{ return tileSize.h; }

unsigned int Tile::getX()const{ return tileSize.x; }

unsigned int Tile::getY()const{ return tileSize.y; }


void Tile::setSource(Tileset *newTileset, unsigned int newCol, unsigned int newRow){
    srcTileset = newTileset;
    setRow(newRow);
    setCol(newCol);
}

void Tile::setTileset(Tileset *newTileset){
    srcTileset = newTileset;
    //make tileset's offset information is loaded
    setRow(0);
    setCol(0);
}

void Tile::setRow(unsigned int newRow){
    row = newRow;

    //if has a source
    if(srcTileset != NULL){
        tileSize.y = (srcTileset->getTileHeight()*row) + srcTileset->getYOffset();
        tileSize.h = srcTileset->getTileHeight();
    } else {
        clear();
    }
}

void  Tile::setCol(unsigned int newCol){
    col = newCol;
    //if has a source
    if(srcTileset != NULL){
        tileSize.x = (srcTileset->getTileWidth()*col) + srcTileset->getXOffset();
        tileSize.w = srcTileset->getTileWidth();
    } else {
        clear();
    }
}


//Returns a INFPacket containing the tile
bool Tile::savePacket(inp::INFPacket& pack)const{
    pack << revision_;

    //Tile source filename
    if( srcTileset != NULL ){
        pack << srcTileset->getFilename();
    } else {
        pack << "";
    }

	//TilePosition
    pack << row;
    pack << col;

    return true;
}

//  Loads data from a packet
bool Tile::loadPacket( inp::INFPacket& pack ){
    if( pack.empty() || pack.readDone() ){ return 0; }

    {
        int fileVer;
        pack >> fileVer;
        if( fileVer != revision_ ){
            std::cerr << __FILE__ << __LINE__ << "incompatible file version" << std::endl;
            return 0;
        }
    }

    std::string tsFilename = "";

    //Tile source
    pack >> tsFilename;
    if( pack.readDone() ){ return 0; }

	//TilePosition
    pack >> row;
    if( pack.readDone() ){ return 0; }
    pack >> col;

    if( cache::tilesets.get(tsFilename) != NULL ){
        setSource(cache::tilesets.get(tsFilename), col, row);
    }

    return true;
}



void Tile::draw(SDL_Surface *dest, int x, int y, bool blend){
    if(srcTileset != NULL){
        if(srcTileset->getImage() != NULL){
            if( blend ){
                drawSurfacePPAlpha(srcTileset->getImage(), tileSize, dest, x, y);
            } else {
                //no blend, copy pix over
                drawSurface(srcTileset->getImage(), tileSize, dest, x, y);
            }
        }
    }
}
