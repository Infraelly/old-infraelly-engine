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


#ifndef TILESET_HPP
#define TILESET_HPP

//  Tileset.hpp

#include <map>
#include <string>
#include <SDL/SDL.h>
#include "ResourcePack.hpp"
#include "Cacheable.hpp"
#include "globalFunc.hpp"
#include "dataPacks.hpp"
#include "tinyxml.h"


class Tileset : public Cacheable {
    public:
        Tileset();
        Tileset(const std::string& fileName);
        Tileset(const std::string& fileName, const std::string& theAlias);
        Tileset(const std::string& fileName, const std::string& theAlias, const SDL_Color& transKey);
        Tileset(const std::string& fileName, const std::string& theAlias, unsigned int newTileWidth, unsigned int newTileHeight);
        Tileset(const std::string& fileName, const std::string& theAlias, const SDL_Color& transKey, unsigned int newTileWidth, unsigned int newTileHeight);

        //copy ctor
        Tileset(const Tileset& src);
        //assignment operator
        Tileset& operator=(const Tileset& rhs);


        ~Tileset();


        //  Load image to use as tileset from file
        int loadImage(const std::string& imageFilename);
        int loadImage(const std::string& imageFilename, const std::string& theAlias);
        int loadImage(const std::string& imageFilename, const std::string& theAlias, const SDL_Color& transKey);
        int loadImage(const std::string& imageFilename, const std::string& theAlias, unsigned int newTileWidth, unsigned int newTileHeight);
        int loadImage(const std::string& imageFilename, const std::string& theAlias, const SDL_Color& transKey, unsigned int newTileWidth, unsigned int newTileHeight);


        //save tileset data to an xml file
        bool saveXml( const std::string& filename );
        //load tileset data from an xml doc
        bool loadXml( const TiXmlDocument& doc );

        //load from file
        bool load( const std::string& filename );
        //load from datapack
        bool load( const ResourcePack& pack, const std::string& filepath );

        //  Load tileset(xml format) file to use as tileset by resource cache
        bool cache_load( const std::string& filename ){ return load(packs::mainPack, filename); }



        //  Return the image the tileset is using
        SDL_Surface *getImage()const;
        //  Return the transparrent colour (NULL if none)
        SDL_Color* getTransKey()const;
        //  Return the X-offset of the tileset's image
        int getXOffset()const;
        //  Return the Y-offset of the tileset's image
        int getYOffset()const;
        //  Return the width of a tile
        int getTileWidth()const;
        //  Return the height of a tile
        int getTileHeight()const;
        //  Return the width of the tileset's image
        int getTilesetWidth()const;
        //  Return the height of the tileset's image
        int getTilesetHeight()const;
        //  Return the number of rows
        int getRows()const;
        //  Return the number of cols
        int getCols()const;
        //  Return the Alias of the tileset
        const std::string& getAlias()const;
        //  Return the filepath of the tileset's image
        const std::string& getImageFilename()const;



        //  Set the image the tileset is using
        void setImage(SDL_Surface *newSource);
        //  Set the image the tileset is using, and the alias
        void setImage(SDL_Surface *newSource, const std::string& theAlias);
        //  Set the transparrent colour (NULL if none)
        void setTransKey(const SDL_Color& newTransKey);
        //  Set the X-offset of the tileset's image
        void setXOffset(int newXOffset);
        //  Set the Y-offset of the tileset's image
        void setYOffset(int newYOffset);
        //  Set the width of a tile
        void setTileWidth(unsigned int newTileWidth);
        //  Set the height of the tileset's image
        void setTileHeight(unsigned int newTileHeight);
        //  Set the Alias of the tileset
        void setAlias(const std::string& alias);



    private:
        SDL_Surface *image;         //  Image for current tileset
        SDL_Color *transKey;        //  The transparrent colour (NULL if none)
        int tileWidth;              //  The width of a tile
        int tileHeight;             //  The Height of a tile
        int xOffset;                //  x offset of image
        int yOffset;                //  y offset of image
        std::string imageFilename;  //  The path from which the image was loaded
        std::string alias;          //  The alias for the tileset
};



#endif
