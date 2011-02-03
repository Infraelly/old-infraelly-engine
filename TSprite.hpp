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


#ifndef TSPRITE_HPP
#define TSPRITE_HPP

//  TSpirte.hpp

#include <map>
#include <vector>
#include <string>

#include <SDL/SDL.h>
#include <string>

#include "Tileset.hpp"
#include "Tile.hpp"

#include "Cacheable.hpp"
#include "dataPacks.hpp"


class TSprite : public Cacheable {
    public:
        friend class Tile;
        friend class Item;

        //  ctor
        TSprite();
        TSprite(const Tile &src);
        ~TSprite();


        //  Returns the number of frame
        int getFrameCount()const;
        //  Returns the speed of the sprite (in FPS)
        int getFps()const;
        //  Returns which frame the animation is up to
        int getFrameProgression();


        //  Returns the source ts
        Tileset *getSource()const;
        //  Returns number of images in anim
        int getNumImages()const;
        //  Returns number of image per row
        int getImagesPerRow()const;
        //  Returns distance from left to start taking images (Tiles)
        int getXOffset()const;
        //  Returns distance from right to start taking images (Tiles)
        int getYOffset()const;
        //  Return the Alias of the tileset
        const std::string& getAlias()const;


        //returns width of images
        int getWidth()const;
        //returns height of images
        int getHeight()const;

        //returns the frame at frameNo
        const Tile* getFrame(size_t frameNo);
        //  Set the Alias of the tileset
        void setAlias(const std::string& alias);


        //  Sets the frame number
        void setFrameProgression(unsigned frameNumber);
        //  Sets speed of animation (in fps)
        void setFps(unsigned int FPS);



        //  Skips one frame in animation
        void frameForward();
        //  Skip "numberOfFrames" number of frames
        void frameForward(unsigned numberOfFrames);
        //  Rewinds one frame backward in animation
        void frameBackwards();
        //  Rewinds "numberOfFrames" number of frames backwards
        void frameBackwards(unsigned numberOfFrames);



        //  Load animation
        int load(  Tileset *srcTileset,
                   unsigned int numImages,      unsigned int imagesPerRow,
                   unsigned int xOffsetTiles,   unsigned int yOffsetTiles,
                   int fps );


        //save tsprite data to an xml file
        bool saveXml( const std::string& filename );
        //load tsprite data from an xml doc
        bool loadXml( const TiXmlDocument& doc );

        //load from file
        bool load( const std::string& filename );
        //load from datapack
        bool load( const ResourcePack& pack, const std::string& filepath );

        //  Load tsprite(xml format) file to use as tsprite by resource cache
        bool cache_load( const std::string& filename ){ return load(packs::mainPack, filename); }




        //  Unloads frames
        void unload();



        //  Draws the Tsprite to "dest" at (x,y)
        void draw(SDL_Surface *dest, int x, int y);
        //  Draws frame number "frameNumber" of TSprite to "dest" at (x,y)
        void draw(SDL_Surface *dest, int x, int y, unsigned int frameNumber);



    private:
        void advanceFrames();

        //  Frames of the animation strip
        std::vector<Tile> frames_;

        Tileset *src_;                  //  Where the tiles come from
        size_t numImages_;              //  The number of tiles in the anim
        int imagesPerRow_;              //  The number of images in one row
        int xOffset_;                   //  Distance from left to start taking images (Tiles)
        int yOffset_;                   //  Distance from top to start taking images (Tiles)

        int frameDelay_;               //  The time gap between frame advances (ms)
        int lastFrameAdvance_;         //  The time the last frame was advanced
        int frameProgression_;          //  The frame the animation is up to

        std::string alias;              //  The alias for the tileset
};



#endif
