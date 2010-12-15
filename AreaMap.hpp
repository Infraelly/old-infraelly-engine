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


#ifndef AREAMAP_HPP
#define AREAMAP_HPP

//  AreaMap.hpp


#include <string>
#include <SDL/SDL.h>

#include <vector>
#include "Layer.hpp"
#include "Cacheable.hpp"



class AreaMap : public Cacheable {
    public:
        //get versionof Map Class
        static const std::string& getVersion();

        //  Returns the loaded AreaMap version of the loaded map
        const std::string& getLoadedVersion();



        //standard ctor/dtor
        AreaMap();
        explicit AreaMap(const std::string& filename);
        ~AreaMap();


        //  Clears out data stored
        void unload();

        //  Clears out data and reloads currently loaded map from file
        bool reload();


        //  Returns the filename(path) of currently load map. "NULL" if none
        const std::string& getFilename()const;

        //  Returns the Display Name of currently loaded map. "NULL" if none
        const std::string& getMapName()const;

        //  Returns ref to a layer
        Layer& getLayer(size_t i);

        //  Returns number of layers
        int size()const;

        //  Returns true if no layers
        bool empty()const;

        //  Returns if map is to be multiplayer enabled
        bool isExclusive()const;



        //  Sets the filename for the currently loaded map
        void setFilename(const std::string& filename);

        //  Sets the display name for the currently loaded map
        void setMapName(const std::string& newMapName);

        //  Sets if the map is to be multiplayer enabled
        void setExclusive(bool newValue);

        //  Sets number of layers
        void setSize(int i);



        //  Saves the current map data to file (filename)
        bool saveMap(const std::string& filename);

        //  Loads map from file
        bool loadMap(const std::string& filename);

        //  Loads map file from inf packet
        bool loadMap(inp::INFPacket& packet);

        //  Loads map from Resource pack
        bool loadMap( const ResourcePack& pack, const std::string& filepath );

        //  Load map file to use as map by resource cache
        bool cache_load( const std::string& filename ){ return loadMap(packs::mainPack, filename); }



        //adds layer to back
        //  returns new number of layers
        size_t addLayer(const Layer &layer);

        //remove layer
        //  returns new number of layers
        void removeLayer(size_t i);

        //swaps location of two layers
        void swapLayers(size_t i, size_t j);


        //  Returns the attribute at reqeuted position
        enum MapTile::TileAttributes getAttribute(size_t x, size_t y);


        //  update layer images
        void imageUpdate();



        //  Draws all layers to "dest" at (0,0)
        void draw(SDL_Surface *dest);

        //  Draws all layers to "dest" at (x,y)
        void draw(SDL_Surface *dest, int x, int y);

        //  Draws all layers within the boundries of (startX,startY,w,h) to (x,y)
        void draw(SDL_Surface *dest,
                    int x,                int y,
                    unsigned int startX,  unsigned int startY,
                    unsigned int w,       unsigned int h);




    private:
        //  version of map class
        static const std::string version;

        std::vector<Layer> layers;      //  vector to 6 layers

        std::string filename;           //  For reloading
        std::string mapName;            //  The display name of the map
        bool exclusive;                 //  if multiplayer is enabled on the map
        std::string loadedVersion;      //  Version of map class if loaded from file
};





#endif
