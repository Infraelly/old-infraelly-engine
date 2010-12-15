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


//  InfraellyImageLoader.cpp

#include "InfraellyImageLoader.hpp"
#include <SDL/SDL_image.h>
#include <string>
#include "globalFunc.hpp"

gcn::Image* InfraellyImageLoader::load(SDL_Surface *surface, bool freeSurface){
    return new gcn::SDLImage(surface, freeSurface);
}

gcn::Image* InfraellyImageLoader::load(SDL_RWops *source_Rwop, bool freeRWOP, bool convertToDisplayFormat){
    //load the rWop into a SDL Surface
    SDL_Surface *loadedSurface = IMG_Load_RW(source_Rwop, freeRWOP);

    if (loadedSurface == NULL)
    {
        throw GCN_EXCEPTION( std::string("Unable to load image file: ") );
    }

    SDL_Surface *surface = convertToStandardFormat(loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (surface == NULL)
    {
        throw GCN_EXCEPTION( std::string("Not enough memory to load: ") );
    }

    gcn::Image *image = new gcn::SDLImage(surface, true);

    if (convertToDisplayFormat)
    {
        image->convertToDisplayFormat();
    }

    return image;
}


gcn::Image* InfraellyImageLoader::load(unsigned char *buffer, long bufferLength, bool convertToDisplayFormat){
    //make rWop out of character buffer
    SDL_RWops *source_Rwop = SDL_RWFromMem(buffer, bufferLength);

    //load the rWop into a SDL Surface
    SDL_Surface *loadedSurface = IMG_Load_RW(source_Rwop, 1);

    if (loadedSurface == NULL)
    {
        throw GCN_EXCEPTION( std::string("Unable to load image file: ") );
    }

    SDL_Surface *surface = convertToStandardFormat(loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (surface == NULL)
    {
        throw GCN_EXCEPTION( std::string("Not enough memory to load: ") );
    }

    gcn::Image *image = new gcn::SDLImage(surface, true);

    if (convertToDisplayFormat)
    {
        image->convertToDisplayFormat();
    }

    return image;
}



gcn::Image* InfraellyImageLoader::load(const Tileset *sourceTileset){
    if( sourceTileset != NULL ){
        if( sourceTileset->getImage() != NULL ){
            return new gcn::SDLImage(sourceTileset->getImage(), 0);
        } else {
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "Warning: InfraellyImageLoader::load(Tileset*) was passed a sourceTileset->getImage() == NULL" << std::endl;
            return NULL;
        }
    } else {
        std::cerr << __FILE__ << " " << __LINE__ << ": " << "Warning: InfraellyImageLoader::load(Tileset*) was passed a NULL" << std::endl;
        return NULL;
    }
}

gcn::Image* InfraellyImageLoader::load(const Tileset &sourceTileset){
    return load( &const_cast<Tileset&>(sourceTileset) );
}


gcn::Image* InfraellyImageLoader::load(const Tile& sourceTile){
    //put the pixel data at the pos of tile onto surface
    //dimensions of tile's image
    int startX = sourceTile.getX();
    int startY = sourceTile.getY();
    int finX = sourceTile.getX()+sourceTile.getWidth();
    int finY = sourceTile.getY()+sourceTile.getHeight();
    //cache a pointer to the image copying from to avoid function overhead
    if( sourceTile.getTileset() == NULL ){ return NULL; }
    SDL_Surface *tileSurface = sourceTile.getTileset()->getImage();
    if( tileSurface == NULL ){ return NULL; }


    //create the surface
    SDL_Surface *copiedSurface = createSurface( sourceTile.getWidth(),
                                                sourceTile.getHeight(),
                                                tileSurface->format->BitsPerPixel,
                                                tileSurface->flags );

    //lock the surface
    LOCK(copiedSurface);
    LOCK(tileSurface);

    int destX = 0;
    int destY = 0;
    Uint8 r,b,g,a;
    //put pixels, running down
    for( int srcY = startY; srcY < finY; ++srcY ){
        //put pixels, running across
        for( int srcX = startX; srcX < finX; ++srcX ){
            //get the rbg values of the pixel in the source
            SDL_GetRGBA(getpixel(tileSurface, srcX, srcY), tileSurface->format, &r, &g, &b, &a);
            //map the rbg vales from source's format to the dest's format
            putpixel( copiedSurface, destX, destY, SDL_MapRGBA(copiedSurface->format,r,g,b,a) );
            ++destX;
        }
        destX = 0;
        ++destY;
    }


    //unlock the surface
    UNLOCK(copiedSurface);
    UNLOCK(tileSurface);

    gcn::Image *image = new gcn::SDLImage(copiedSurface, true);

    return image;
}


gcn::Image* InfraellyImageLoader::load(const Tile* sourceTile){
    if( sourceTile != NULL ){
        return load(*sourceTile);
    }
    return NULL;
}
