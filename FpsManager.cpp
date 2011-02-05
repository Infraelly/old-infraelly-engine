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


// FpsManager.cpp

#include "FpsManager.hpp"

#include <string>
#include <sstream>
#include "fonts.hpp"
#include "colours.hpp"
//drawText()
#include "globalFunc.hpp"


FpsManager::FpsManager() :
    frameStartTime(0),
    capStartTime( SDL_GetTicks() ),
    nowTime(0),
    fps(0),
    maxFps(-1),
    framesDrawn(0)
{ }

FpsManager::FpsManager(int Fps) :
    frameStartTime(0),
    capStartTime( SDL_GetTicks() ),
    nowTime(0),
    fps(0),
    maxFps(Fps),
    framesDrawn(0)
{ }

FpsManager::~FpsManager(){}




int FpsManager::getFps()const{ return fps; }

int FpsManager::getMaxFps()const{ return maxFps; }



void FpsManager::setMaxFps(int newMaxFps){ maxFps = newMaxFps; }



void FpsManager::startFrame(){ frameStartTime = SDL_GetTicks(); }

void FpsManager::endFrame(){
    //increment the number of frames drawn
    ++framesDrawn;

    //get the time atm
    nowTime = SDL_GetTicks();

    //if nessesary, pause
    if( maxFps > 0 ){
        if ( nowTime-frameStartTime < 1000/maxFps ){
            SDL_Delay( (1000/maxFps) - (nowTime-frameStartTime) );
        }
    }

    //a second has passed, store fps and reset framesDrawn and start time
    if ( nowTime-capStartTime >= 1000 ){
        fps = framesDrawn;
        framesDrawn = 0;
        capStartTime = SDL_GetTicks();
    }
}

void FpsManager::newFrame(){ endFrame(); startFrame(); }


//draws Fps to "screen" at "xy"
void FpsManager::draw(SDL_Surface *dest, int x, int y){
    std::ostringstream fpsText;
    fpsText << fps;
    drawText(fpsText.str(), font::mainFont.at(14), colour::white, dest, x, y );
    drawText(fpsText.str(), font::mainFont.at(14), colour::red, dest, ++x, ++y );
}
