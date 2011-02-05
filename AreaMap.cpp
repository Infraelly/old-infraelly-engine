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


//  AreaMap.cpp

#include "AreaMap.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <tinyxml.h>
#include "globalFunc.hpp"
#include "INFPacket.hpp"

#include "caches.hpp"
#include "dataPacks.hpp"

using namespace std;


const std::string AreaMap::version = "0.4.0";
const std::string& AreaMap::getVersion(){ return version; }


AreaMap::AreaMap() :
    filename("NULL"),
    mapName("NULL"),
    exclusive(0),
    loadedVersion("-1")
{ layers.reserve(6); }

AreaMap::AreaMap(const std::string& newFilename) :
    filename(correctFilepath(newFilename)),
    mapName("NULL"),
    exclusive(0),
    loadedVersion("-1")
{ layers.reserve(6); }


AreaMap::~AreaMap(){
    unload();
}

void AreaMap::unload(){
    filename = "NULL";
    mapName = "NULL";
    layers.clear();
}


const std::string& AreaMap::getLoadedVersion(){ return loadedVersion; }






//  Saves the current map data to file (filename)
bool AreaMap::saveMap(const std::string& filename){
    if( layers.empty() ){ return 0; }

    //open file
    std::string fixedFn = correctFilepath(filename);
    std::ofstream file( fixedFn.c_str(), std::ios::out|std::ios::binary );
    if( (!file.good()) || (!file.is_open()) ){
        file.close();
        std::cout << "Unable to open file \"" << fixedFn << "\"" << std::endl;
        return -1;
    }

    inp::INFPacket pack;


    /*--------------------------------------------------------------------
                            Gather Data

        scan tiles to find number of tilesets used, and their filenames
    --------------------------------------------------------------------*/
    //vector to hold filenames of tilesets
    vector<string> tilesetFilenames;

    //string to store the filename of the tilset currently being searched
    string curTilesetFilename = "";

    //cycle through layers
    for( size_t i = 0; i < layers.size(); ++i ){

        //scan through tiles in layer
        for( size_t j = 0; j < layers[i].size(); ++j){
            //get filename of tile's tileset details
            if(layers[i].index(j).getTileset() == NULL){
                //tile has no tileset source, warn
                cerr << __FILE__ << " " << __LINE__ << ": " << "Warning: Saving tile with no tileset. Layer:" << i << " Index:" << j << endl;
                //make tileset filename ""
                curTilesetFilename = "";
            } else {
                //store tile's tileset's filename
                curTilesetFilename = correctFilepath(layers[i].index(j).getTileset()->getFilename());
            }


            //if tileset filename exists, put it onto tilesetFilename vector
            if(curTilesetFilename != ""){
                //if the vector is empty, it CANT allredy have the curent filename in it.
                if (tilesetFilenames.empty()){
                    tilesetFilenames.push_back(curTilesetFilename);
                    continue;
                }

                //search through tilesetFilenames vector
                if( find( tilesetFilenames.begin(), tilesetFilenames.end(), curTilesetFilename ) != tilesetFilenames.end() ){
                    //the entry is not in the vector yet, add it
                    tilesetFilenames.push_back(curTilesetFilename);
                }
            }

        }//end tile scan
    }   //end layer cycle


    //--------------------------------------------------------------------------
    //
    //            Make sure all the tilesets exists as a physical file
    //                  ( the editor creates tilesets in memory )
    //
    //--------------------------------------------------------------------------
    for( size_t i = 0; i < tilesetFilenames.size(); ++i ){
        if( getFileSize(tilesetFilenames.at(i)) == 0 ){
            Tileset *toSave = cache::tilesets.loadResource( tilesetFilenames.at(i) );
            if( toSave == NULL ){ return 0; }
            if( toSave->saveXml("tilesets/"+toSave->getFilename()) == 0 ){
                return 0;
            }
            toSave->setFilename("tilesets/"+toSave->getFilename());
        }
    }




	//set map version
    pack << version;
    pack << static_cast<int>(exclusive);

    //write number of tilesets used
    pack << (int) tilesetFilenames.size();
    //write tileset filenames
    for( size_t i = 0; i < tilesetFilenames.size(); ++i ){
        pack << tilesetFilenames.at(i);
    }

	//save number of layers
    pack << (int) layers.size();

    //save all the layers
    for(size_t i = 0; i < layers.size(); ++i){
        if( !layers[i].savePacket(pack) ){ return false; }
    }

    file.write( (char*)pack.getBuf(), pack.getLength() );
    file.close();

    cout << pack;

	return true;
}

bool AreaMap::loadMap(inp::INFPacket& pack){
	//set map version
    pack >> loadedVersion;
    if( loadedVersion != version ){ return 0; }
    if( pack.readDone() ){ return 0; }

    //check if is exclusive
    pack >> exclusive;
    if( pack.readDone() ){ return 0; }

    //read number of tilesets used
    int numTs = 0;
    pack >> numTs;

    //read tileset filenames
    string tempStr = "";
    for( int i = 0; i < numTs; ++i ){
        pack >> tempStr;
        if( pack.readDone() ){ return 0; }
        //load ts
        cache::tilesets.loadResource(tempStr);
    }

	//read number of layers
	int numLayers = 0;
    pack >> numLayers;
    //  dont check
    if( pack.readDone() ){ return 0; }

    //load all the layers
    for(size_t i = 0; i < numLayers; ++i){
        layers.push_back( Layer() );
        if( layers[i].loadPacket(pack) == 0 ){
            return 0;
        }
    }

    return true;
}

//  Loads map from file
bool AreaMap::loadMap(const std::string& filename){
    //open file
    std::string fixedFn = correctFilepath(filename);
    std::ifstream file( fixedFn.c_str(), std::ios::in|std::ios::binary );
    if( (!file.good()) || (!file.is_open()) ){
        file.close();
        std::cout << "Unable to open file \"" << fixedFn << "\"" << std::endl;
        return 0;
    }
    //get filesize
    size_t filesize = getFileSize(file);

    // read file into packet
    inp::INFPacket pack;
    pack.readFile( file, filesize );

    //close file
    file.close();

    //clean map
    unload();

    filename_ = fixedFn;

	return loadMap(pack);
}

//load from rc files
bool AreaMap::loadMap( const ResourcePack& pack, const std::string& filepath ){
    filename_ = correctFilepath(filepath);
    ResourceFile rcf;
    packs::mainPack.getResource( filepath, rcf );

    inp::INFPacket packet;
    packet.readBuf( &packs::mainPack.getRawData()[rcf.start], rcf.size );
    return loadMap(packet);
}




bool AreaMap::reload(){ return cache::maps.loadResource(filename); }

const std::string& AreaMap::getFilename()const{ return filename; }

const std::string& AreaMap::getMapName()const{ return mapName; }

Layer& AreaMap::getLayer(size_t i){
    if( (i < 0) || (i > layers.size()) ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "AreaMap::getLayer: Array fencepost reference error." << endl;
        exit(EXIT_FAILURE);
    }
    return layers[i];
}

int AreaMap::size()const{ return layers.size(); }

bool AreaMap::empty()const{ return layers.empty(); }


bool AreaMap::isExclusive()const{ return exclusive; }



void AreaMap::setFilename(const std::string& newFilename){ filename = correctFilepath(newFilename); }

void AreaMap::setMapName(const std::string& newMapName){ mapName = newMapName; }

void AreaMap::setExclusive(bool newValue){ exclusive = newValue; }

void AreaMap::setSize(int i){ layers.resize(i); }


void AreaMap::imageUpdate(){
    for( unsigned int i = 0; i < layers.size(); ++i ){
        layers[i].updateImage();
    }
}

//  returns new number of layers
size_t AreaMap::addLayer(const Layer &layer){
    layers.push_back(layer);
    return layers.size();
}


//remove layer
//  returns new number of layers
void AreaMap::removeLayer(size_t i){
    if( (i > 0) && (i < layers.size()) ){
        std::vector<Layer>::iterator itr = layers.begin();
        advance(itr, i);
        layers.erase(itr);
    }
}

//swaps location of two layers
void AreaMap::swapLayers(size_t i, size_t j){
    if( (i > 0) && (i < layers.size()) && (j > 0) && (j < layers.size()) ){
        std::vector<Layer>::iterator itrI = layers.begin();
        std::vector<Layer>::iterator itrJ = layers.begin();
        advance(itrI, i);
        advance(itrJ, j);
        swap(itrI, itrJ);
    }
}



enum MapTile::TileAttributes AreaMap::getAttribute(size_t x, size_t y){
    if( !layers.empty() ){
        if( (x < layers[0].getWidth()) && (y < layers[0].getHeight()) ){
            return layers[0].at(x, y).getAttribute();
        }
    }

    return MapTile::REGULAR;
}


//draw functions
void AreaMap::draw(SDL_Surface *dest){
    draw(dest, 0, 0);
}

void AreaMap::draw(SDL_Surface *dest, int x, int y){
    //cycle through layers
    for( unsigned int i = 0; i < layers.size(); ++i ){
        layers[i].draw(dest, x, y);
    }
}

void AreaMap::draw(SDL_Surface *dest, int x, int y, unsigned int startX, unsigned int startY, unsigned int w, unsigned int h){
    //cycle through layers
    for( unsigned int i = 0; i < layers.size(); ++i ){
        layers[i].draw(dest, x, y, startX, startY, w, h);
    }
}
