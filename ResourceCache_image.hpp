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


#ifndef RESOURCECACHE_IMAGE_HPP_INCLUDED
#define RESOURCECACHE_IMAGE_HPP_INCLUDED

#include <iostream>
#include <string>
#include <memory>
#include <SDL/SDL.h>
#include "globalFunc.hpp"


#if USE_PACKED_RESOURCES
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include "dataPacks.hpp"
#endif


//******************************************************************************
//
//  This file contains a partiall specialised template definition/declaration
//  for loading images using SDL_image
//
//  Do not include this file directly, include "ResourceCache.hpp"
//
//
//  addPointer( T* ) hidden
//  loadResource( string ) specialised
//      + loadResource( string, transkey )
//  erase specialised
//  unload() specialised
//
//******************************************************************************

template< >
class ResourceCache<SDL_Surface> {
    public:
        ResourceCache(const std::string& cacheName) :
            name(cacheName)
        {}
        ~ResourceCache(){ unload(); }

        //  Gets the name of the cache
        const std::string& getName()const{ return name; }
        //  Sets the name of the cache
        void setName(const std::string& newName){ name = newName; }

        //  Loads the resource from a file
        //  If already loaded, will do nothing
        //  Returns a pointer to the resource, or NULL on fail
        SDL_Surface *loadResource( std::string filename ){
            makeUnixFilePath(filename);
            if( resources.find(filename) == resources.end() ){
                SDL_Surface *newImage;
                #if USE_PACKED_RESOURCES
                    ResourceFile rcf;
                    packs::mainPack.getResource(filename, rcf);
                    newImage = IMG_Load_RW( SDL_RWFromMem( const_cast<unsigned char*>(&packs::mainPack.getRawData()[rcf.start]), rcf.size), true);
                #else
                    newImage = loadImage(filename);
                #endif
                if( newImage != NULL ){
                    //load is good, put on map.
                    resources[filename] = newImage;
                }
                return newImage;
            }
            return resources[filename];
        }
        SDL_Surface *loadResource( std::string filename, const SDL_Color& transKey ){
            makeUnixFilePath(filename);
            if( resources.find(filename) == resources.end() ){
                SDL_Surface *newImage = loadImage(filename, transKey);
                if( newImage != NULL ){
                    //load is good, put on map.
                    resources[filename] = newImage;
                }
                return newImage;
            }
            return resources[filename];
        }

        //  Returns a pointer to object with the filename "filename", NULL on fail
        SDL_Surface *get(std::string filename){
            makeUnixFilePath(filename);
            StorageMediumIterator itr = resources.find(filename);
            if( itr == resources.end() ){
                return NULL;
            } else {
                return itr->second;
            }
        }

        //  Returns true if there are no preloaded Resources
        bool empty(){ return resources.empty(); }

        //  Returns the Resources store at the index i
        SDL_Surface *at(size_t i){
            if( resources.empty() || (i > resources.size()-1) ){
                std::cerr << __FILE__ << " " << __LINE__ << ": " << "Index out of bounds" << std::endl;
                return NULL;
            }
            StorageMediumIterator iter = resources.begin();
            // iter += i;
            for( size_t j = 0; j < i; ++j ){
                ++iter;
            }
            return iter->second;
        }


        //  Returns number of Resources preloaded
        int size(){ return resources.size(); }

        //  Removes given Resources from tileset stack
        void erase(SDL_Surface *toRemove){
            if( resources.empty() ){ return; }
            if( toRemove == NULL ){ return; }

            StorageMediumIterator iter = resources.begin();

            for( ; iter != resources.end(); ++iter ){
                //if found a matching pointer: remove entry
                if( iter->second == toRemove ){
                    SDL_FreeSurface(iter->second);
                    iter->second = NULL;
                    resources.erase( iter );
                    return;
                }
            }
        }

        void erase(std::string filename){
            makeUnixFilePath(filename);
            if( resources.empty() ){ return; }

            StorageMediumIterator iter;
            iter = resources.find( filename );

            if( iter == resources.end() ){
                return;
            } else {
                //if found a mathcing pointer: delete and remove entry
                SDL_FreeSurface( resources[filename] );
                resources[filename] = NULL;
                resources.erase( iter );
                return;
            }
        }


        //  Adds pointer to Resources stack
        //  use highly un-recomended, please use loadResources(...) instead
        //  returns 1 if added, 0 on fail(already exists)
        //  NOTE: Calling unload will also delete resources added using this
        //      method
        bool addPointer(SDL_Surface *newRc, std::string filename){
            makeUnixFilePath(filename);
            if( resources.find(filename) == resources.end() ){
                resources[filename] = newRc;
                return 1;
            } else {
                return 0;
            }
        }


        //  Removes pointer of Resources from the vector (without calling delete)
        void removePointer(SDL_Surface *toRemove){
            if( resources.empty() ){ return; }

            StorageMediumIterator iter = resources.begin();

            for( ; iter != resources.end(); ++iter ){
                //if found a matching pointer: remove entry
                if( iter->second == toRemove ){
                    resources.erase( iter );
                    return;
                }
            }
        }

        //  Closes safely frees and clears all Resources loaded
        void unload(){
            if( resources.empty() ){ return; }

            StorageMediumIterator iter = resources.begin();

            std::cerr << __FILE__ << " " << __LINE__ << ": " << "Clearing ResourceCache:" << name << std::endl;
            for( ; iter != resources.end(); ++iter ){
                //clear tilesets
                SDL_FreeSurface( iter->second );
                iter->second = NULL;
            }
            resources.clear();
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "Cleared ResourceCache:" << name << std::endl << std::endl;
        }



    private:
        bool addPointer(SDL_Surface *newRc){ return 0; }

        //  The name we will use to refer to this particilar resource cache
        std::string name;
        //  We use a map to store pointers to everything =]
        typedef std::map<std::string, SDL_Surface*> StorageMedium;
        typedef std::map<std::string, SDL_Surface*>::iterator StorageMediumIterator;
        StorageMedium resources;

        // please dont try to copy the cache... if you realy want to, iterate
        //  it with a for loop, using size and at.
        //  just want to make sure no one inadvertantly copies it..
        ResourceCache(const ResourceCache& src);
        ResourceCache& operator=(ResourceCache& rhs);
};


#endif // RESOURCECACHE_IMAGE_HPP_INCLUDED
