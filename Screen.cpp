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


//  Screen.cpp

#include "Screen.hpp"

#include <iostream>
#include "GameConfig.hpp"


SDL_Surface *Screen::screenSurface = NULL;

bool Screen::fullscreen = false ;

const SDL_VideoInfo *Screen::info = SDL_GetVideoInfo();

std::string Screen::caption = "SDL Window";





bool Screen::init(int width, int height, int bpp, Uint32 flags){
    screenSurface = SDL_SetVideoMode(width, height, bpp, flags);
    if( screenSurface == NULL ){
        std::cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to initialise screen:" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
        fullscreen = false;
        return false;
    } else {
        if( screenSurface->flags & SDL_FULLSCREEN ){
            fullscreen = true;
        } else {
            fullscreen = false;
        }
    }
    info = SDL_GetVideoInfo();
    return true;
}


int Screen::getWidth(){
    if( screenSurface == NULL ){ return -1; }
    return screenSurface->w;
}

int Screen::getHeight(){
    if( screenSurface == NULL ){ return -1; }
    return screenSurface->h;
}

int Screen::getBpp(){
    if( screenSurface == NULL ){ return -1; }
    return screenSurface->format->BitsPerPixel;
}


void Screen::clear(Uint8 r, Uint8 g, Uint8 b){
    SDL_FillRect(screenSurface,NULL,SDL_MapRGB(screenSurface->format,r,g,b));
}

void Screen::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    SDL_FillRect(screenSurface,NULL,SDL_MapRGBA(screenSurface->format,r,g,b,a));
}

void Screen::flip(){
    if( SDL_Flip(screenSurface) == -1 ){
        std::cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to flip screen" << SDL_GetError() << std::endl;
    }
}

SDL_Surface* Screen::getSurface(){ return screenSurface; }






bool Screen::resize(int width, int height){
    return resize(width, height, screenSurface->format->BitsPerPixel);
}

bool Screen::resize(int width, int height, int bpp){
    return resize(width, height, screenSurface->format->BitsPerPixel, screenSurface->flags);
}

bool Screen::resize(int width, int height, int bpp, Uint32 flags){
    return init(width, height, bpp, flags);
}









bool Screen::isFullscreen(){ return fullscreen; }

void Screen::setFullscreen(bool value){
    if( screenSurface != NULL ){
        if( value ){
           init( GameConfig::fScreenWidth, GameConfig::fScreenHeight,
                  GameConfig::fScreenBpp,
                  screenSurface->flags|SDL_FULLSCREEN );
        } else {
            init( GameConfig::wScreenWidth, GameConfig::wScreenHeight,
                  GameConfig::wScreenBpp,
                  (screenSurface->flags&(screenSurface->flags^SDL_FULLSCREEN))|SDL_RESIZABLE );

        }
    } else {
        std::cerr << __FILE__ << " " << __LINE__ << ": " << "Warning: Trying to set fullscreen uninitiated surface" << std::endl;
        fullscreen = false;
    }
}



void Screen::toggleFullscreen(){
    if( fullscreen ){
        setFullscreen( false );
    } else {
        setFullscreen( true );
    }
}



bool Screen::isReady(){
    if( screenSurface != NULL ){ return true; } else { return false; }
}

const SDL_VideoInfo *Screen::getVideoInfo(){ return info; }





void Screen::setIcon(SDL_Surface *image){ SDL_WM_SetIcon(image, NULL); }

void Screen::setCaption(const std::string& newCaption){
    caption = newCaption;
    SDL_WM_SetCaption(caption.c_str(), NULL);
}

const std::string& Screen::getCaption(){
    char **tmpStr;
    SDL_WM_GetCaption(tmpStr, NULL);
    caption = *tmpStr;
    return caption;
}





bool Screen::isShowCursor(){
    if( SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE ){
        return true;
    } else {
        return false;
    }
}

void Screen::setShowCursor(bool value){
    if( value == true ){
        SDL_ShowCursor(SDL_ENABLE);
    } else {
        SDL_ShowCursor(SDL_DISABLE);
    }
}

void Screen::toggleShowCursor(){
    if( SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE ){
        SDL_ShowCursor(SDL_DISABLE);
    } else {
        SDL_ShowCursor(SDL_ENABLE);
    }
}
