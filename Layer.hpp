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


#ifndef LAYER_HPP
#define LAYER_HPP

//  Layer.hpp

#include "MapTile.hpp"
#include <vector>


class Layer{
    public:
        static const int revision_ = 1;

        // Optional parameters
        //  newWidth : how many tiles in a row
        //  newHeight : how many tiles in a collumn
        Layer(int newWidth = 0, int newHeight = 0);
        Layer(const Layer& src);
        Layer& operator=(const Layer& rhs);
        ~Layer();



        //  Returns the tile stored at "i" (tiles stored in 1D array)
        //  returns NULL if out of range
        MapTile& index(int i);
        //  Returns the tile stored at "i" (tiles stored in 1D array)
        //  returns NULL if out of range
        MapTile& operator[](int i);
        //  Returns tile corresponding to co-ordinates (x,y)
        //  returns NULL if out of range
        MapTile& at(int x, int y);



        //  Returns name of layer
        const std::string& getName()const;
        //  Returns total number of tiles stored
        int size()const;
        //  Returns number of tiles in a row
        int getWidth()const;
        //  Returns number of tiles in a column
        int getHeight()const;
        //  Returns true if showing attribute flags
        bool isShowingAttributes()const;



        //  Returns horizontal gap between tiles (in px)
		int getTileWidth()const;
        //  Returns vertical gap between tiles (in px)
        int getTileHeight()const;
        //  True if there are tiles stored in structure
        bool empty()const;
        //  True if array has been constructed
        bool isInitiated()const;




        //  Sets the name of the layer to "name"
        void setName(const std::string& newName);
        //  Sets the amount of tiles per row to "newWidth"
        void setWidth(unsigned int newWidth);
        //  Sets the amount of tiles per column to "newHeight"
        void setHeight(unsigned int newHeight);
        //  Sets the amount of tiles per row, and per column
        void setSize(unsigned int newWidth, unsigned int newHeight);
        //  Sets whether or not to draw attibutes, changes visible when cahce is updated
        void setShowAttributes(bool value);

        //  Sets horizontal gap between tiles (in px)
		void setTileWidth(unsigned int newTileWidth);
		//  Returns vertical gap between tiles (in px)
        void setTileHeight(unsigned int newTileHeight);
        //  Sets horizontal and vertical gap between tiles (in px)
        void setTileSize(unsigned int newTileWidth, unsigned int newTileHeight);




        //  Tells the layer to update it's image
        void updateImage();



        //Returns a INFPacket containing the layer
        bool savePacket(inp::INFPacket& pack)const;

        //  Loads data from a packet
        bool loadPacket( inp::INFPacket& pack );



        //  Draws all tiles to "dest" at (0,0)
        void draw(SDL_Surface *dest);
        //  Draws all tiles to "dest" at (x,y)
        void draw(SDL_Surface *dest, int x, int y);
        //  Draw tiles within boundries of (startX,startY,w,h) to (x,y)
        //  all parameters in px
        void draw( SDL_Surface *dest,
                   const int& x,                 const int& y,
                   const unsigned int& startX,   const unsigned int& startY,
                   const unsigned int& w,        const unsigned int& h );




    private:
        std::string name;           //  Name of layer. Used in mapper

        int width;                  //  Width of layer (in tiles)
        int height;                 //  Height of layer (in tiles)

		int tileWidth;              //  Horizontal gap between tiles (in px)
		int tileHeight;             //  Vertical gap between tiles (in px)

        std::vector<MapTile> layerGrid; //  Vector of tiles

        bool showAttributes;        //  Whether or not to draw attribute tags

        SDL_Surface *cacheImage;    //  The Surface to store the predrawn image
        bool cacheReady;            //  Whether or not the cache is ready
};


#endif



