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


// Layer.cpp

#include "Layer.hpp"

#include <iostream>
#include "fonts.hpp"
#include "colours.hpp"
//draw
#include "globalFunc.hpp"
#include "INFPacket.hpp"


using namespace std;


Layer::Layer(int newWidth, int newHeight) :
    name("Unamed Layer"),
    width(newWidth),
    height(newHeight),
    tileWidth(32),
    tileHeight(32),
    layerGrid(newWidth*newHeight),
    showAttributes( false ),
    cacheImage(NULL),
    cacheReady( false )
{
    if( (width == 0) || (height == 0) ){
        layerGrid.reserve(500);
    }
}

Layer::Layer(const Layer& src){
    name = src.name;
    width = src.width;
    height = src.height;
    tileWidth = src.tileWidth;
    tileHeight = src.tileHeight;
    layerGrid = src.layerGrid;
    showAttributes = src.showAttributes;
    cacheImage = NULL;
    cacheReady = false;

    layerGrid = src.layerGrid;
}

Layer& Layer::operator=(const Layer& rhs){
    if( &rhs != this ){
        name = rhs.name;
        width = rhs.width;
        height = rhs.height;
        tileWidth = rhs.tileWidth;
        tileHeight = rhs.tileHeight;
        layerGrid = rhs.layerGrid;
        showAttributes = rhs.showAttributes;
        cacheImage = NULL;
        cacheReady = false;

        layerGrid = rhs.layerGrid;
    }
    return *this;
}


Layer::~Layer(){
    if( cacheImage != NULL ){ SDL_FreeSurface(cacheImage); };
}


MapTile& Layer::operator[](int i){
    if( !layerGrid.empty() ){
        if(i < width*height){
            cacheReady = false;
            return layerGrid[i];
        } else {
            cerr << __FILE__ << " " << __LINE__ << ": " << "Layer: Array fencepost reference error. Size:"
                 << width*height <<  " i:" << i << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Layer: Accessed uninitiated" << endl;
        exit(EXIT_FAILURE);
    }
}

MapTile& Layer::index(int i){
    if( !layerGrid.empty() ){
        if(i < width*height){
            cacheReady = false;
            return layerGrid[i];
        } else {
            cerr << __FILE__ << " " << __LINE__ << ": " << "Layer: Array fencepost reference error." << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accessed uninitiated layer" << endl;
        exit(EXIT_FAILURE);
    }
}

MapTile& Layer::at(int x, int y){
    if( !layerGrid.empty() ){
        if((y*width + x) < width*height){
            cacheReady = false;
            return layerGrid[(y*width + x)];
        } else {
            cerr << __FILE__ << " " << __LINE__ << ": " << "Layer: Array fencepost reference error." << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accessed uninitiated layer" << endl;
        exit(EXIT_FAILURE);
    }
}




const std::string& Layer::getName()const{ return name; }

int Layer::size()const{ return width*height; }

int Layer::getWidth()const{ return width; }

int Layer::getHeight()const{ return height; }

bool Layer::isShowingAttributes()const{ return showAttributes; }



int Layer::getTileWidth()const{ return tileWidth; }

int Layer::getTileHeight()const{ return tileHeight; }

bool Layer::empty()const{ return layerGrid.empty(); }

bool Layer::isInitiated()const{ return layerGrid.empty(); }






void Layer::setName(const std::string& newName){
    name = newName;
}

void Layer::setSize(unsigned int newWidth, unsigned int newHeight){
    layerGrid.clear();
    layerGrid.resize( newWidth*newHeight );
    width = newWidth;
    height = newHeight;
    cacheReady = false;
}

void Layer::setWidth(unsigned int newWidth){
    layerGrid.clear();
    layerGrid.resize( newWidth*height );
    width = newWidth;
    cacheReady = false;
}

void Layer::setHeight(unsigned int newHeight){
    layerGrid.clear();
    layerGrid.resize( width*newHeight );
    height = newHeight;
    cacheReady = false;
}

void Layer::setShowAttributes(bool value){ showAttributes = value; }



void Layer::setTileWidth(unsigned int newTileWidth){
    tileWidth = newTileWidth;
    cacheReady = false;
}

void Layer::setTileHeight(unsigned int newTileHeight){
    tileHeight = newTileHeight;
    cacheReady = false;
}

void Layer::setTileSize(unsigned int newTileWidth, unsigned int newTileHeight){
    tileWidth = newTileWidth;
    tileHeight = newTileHeight;
    cacheReady = false;
}








void Layer::updateImage(){ cacheReady = false; }






//Returns a INFPacket containing the layer
bool Layer::savePacket(inp::INFPacket& pack)const{
    pack << revision_;

    //Layer Name
    pack << name;

    //Layer size
    pack << (int) width;
    pack << (int) height;

    //Tile size
    pack << (int) tileWidth;
    pack << (int) tileHeight;

    //Show attrib
    pack << showAttributes;


    //  if layer is empty, return.
    if( layerGrid.empty() ){
        //write whether the layerGrid is empty
        pack << true;
        return true;
    } else {
        //write whether the layerGrid is empty
        pack << false;
        //add all the tile's
        for(unsigned int i = 0; i < layerGrid.size(); ++i){
            if( !layerGrid[i].savePacket(pack) ){
                return false;
            }
        }
    }

    return true;
}

//  Loads data from a packet
bool Layer::loadPacket( inp::INFPacket& pack ){
    if( pack.empty() || pack.readDone() ){ return 0; }

    {
        int fileVer;
        pack >> fileVer;
        if( fileVer != revision_ ){
            std::cerr << __FILE__ << __LINE__ << "incompatible file version" << std::endl;
            return 0;
        }
    }

    //Layer Name
    pack >> name;
    if( pack.readDone() ){ return 0; }

    //Layer size
    pack >> width;
    if( pack.readDone() ){ return 0; }
    pack >> height;
    if( pack.readDone() ){ return 0; }

    //Tile size
    pack >> tileWidth;
    if( pack.readDone() ){ return 0; }
    pack >> tileHeight;
    if( pack.readDone() ){ return 0; }

    //Show attrib
    pack >> showAttributes;
    if( pack.readDone() ){ return 0; }


    //  If size is 0, empty layer, no probrem
    //  if size > 0, then alocate space
    if( (width == 0) || (height == 0) ){ return true; } else {
        layerGrid.resize(width*height);
    }

    //check whtehr the layerGrid is empty
    bool empty = 0;
    pack >> empty;

    if( !empty ){
        //Load Tile data
        for(int i = 0; i < width*height; ++i ){
            if( pack.readDone() ){ return 0; }
            layerGrid.at(i).loadPacket( pack );
        }
    }

    return true;
}



void Layer::draw(SDL_Surface *dest){ draw(dest, 0, 0); }

void Layer::draw(SDL_Surface *dest, int x, int y){ draw(dest, x, y, 0, 0, width*tileWidth, height*tileHeight); }

void Layer::draw(  SDL_Surface *dest,
                   const int& x,                 const int& y,
                   const unsigned int& startX,   const unsigned int& startY,
                   const unsigned int& w,        const unsigned int& h )
{
    //check if theres any thing to draw
    if( !layerGrid.empty() ){
        //check if there is a predrawn image to use
        if( !cacheReady ){
            //  Ensure the cache surface exists and is of the right dimensions
            //  check if the predrawn image's surface exists
            if( cacheImage == NULL ){
                cacheImage = createSurface(width*tileWidth, height*tileHeight, dest->format->BitsPerPixel, dest->flags);
            } else if( (cacheImage->w != width*tileWidth) || (cacheImage->h != height*tileHeight) ){
                //if the cacheImage is not of the right dimensions, recreate it
                SDL_FreeSurface(cacheImage);
                cacheImage = createSurface(width*tileWidth, height*tileHeight, dest->format->BitsPerPixel, dest->flags);
            }

            //  Regenerate the layer cache image
            SDL_FillRect(cacheImage, NULL, SDL_MapRGBA(cacheImage->format, 0,0,0,0));
            for( int tmpY = 0; tmpY < height; ++tmpY){
                for( int tmpX = 0; tmpX < width; ++tmpX){
                    if( at(tmpX, tmpY).getTileset() != NULL ){
                        at(tmpX, tmpY).draw( cacheImage, (tmpX*tileWidth), (tmpY*tileHeight) );
                    }
                }
            }
            if( showAttributes ){
                string attLbl = "Reg";
                for( int tmpY = 0; tmpY < height; ++tmpY){
                    for( int tmpX = 0; tmpX < width; ++tmpX){
                        switch( at(tmpX, tmpY).getAttribute() ){
                            case MapTile::REGULAR:
                                attLbl = " ";
                                break;
                            case MapTile::BLOCK:
                                attLbl = "Bl";
                                break;
                            case MapTile::WARP:
                                attLbl = "Wa";
                                break;
                            case MapTile::HEAL:
                                attLbl = "He";
                                break;
                            case MapTile::HURT:
                                attLbl = "Hu";
                                break;
                            case MapTile::KILL:
                                attLbl = "Ki";
                                break;
                            case MapTile::BAN:
                                attLbl = "Ba";
                                break;
                            default:
                                attLbl = " ";
                                break;
                        }
                        drawText(attLbl, font::mainFont.at(16), colour::blue, cacheImage, (tmpX*tileWidth), (tmpY*tileHeight));
                    }
                }
            }
            cacheReady = true;
        }
        //cache surface exists and is now ready for use
        drawSurfacePPAlpha(cacheImage, startX, startY, w, h, dest, x, y);
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Layer::Warning: Attempting to draw uninitiated layer" << endl;
    }
}
