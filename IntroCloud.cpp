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


//image loader
#include "IntroCloud.hpp"
#include "dataPacks.hpp"

int IntroCloud::nInstances = 0;
bool IntroCloud::windDir = 0;
SDL_Surface* IntroCloud::cloudImage = NULL;

IntroCloud::IntroCloud() :
    speed(0),
    x(0),
    y(0)
{
    if( nInstances == 0 ){
        windDir = random(0, 1);
        ResourceFile rcf;
        packs::mainPack.getResource("images/intro/cloud.png", rcf);
        cloudImage = loadImage( SDL_RWFromMem( (void*)&packs::mainPack.getRawData()[rcf.start], rcf.size), true );
    }
    ++nInstances;
}

IntroCloud::IntroCloud(const IntroCloud& rhs){
    ++nInstances;
    speed = rhs.speed;
    x = rhs.x;
    y = rhs.y;
}
IntroCloud& IntroCloud::operator=(IntroCloud& rhs){
    if( &rhs != this ){
        ++nInstances;
        speed = rhs.speed;
        x = rhs.x;
        y = rhs.y;
    }
    return *this;
}


IntroCloud::~IntroCloud(){
    --nInstances;
    if( cloudImage != NULL ){
        SDL_FreeSurface(cloudImage);
    }
}


int IntroCloud::getWindDir(){ return windDir; }

SDL_Surface *IntroCloud::getCloudImage(){ return cloudImage; }

int IntroCloud::getSpeed()const{ return speed; }

int IntroCloud::getX()const{ return x; }

int IntroCloud::getY()const{ return y; }


void IntroCloud::setWindDir(bool newWindDir){ windDir = newWindDir; }

void IntroCloud::setSpeed(int newSpeed){ speed = newSpeed; }

void IntroCloud::setX(int newX){ x = newX; }

void IntroCloud::setY(int newY){ y = newY; }


void IntroCloud::move(){
    if( windDir == 0 ){
        x -= speed;
    } else {
        x += speed;
    }
}

void IntroCloud::draw(SDL_Surface *screen){
    if( cloudImage != NULL ){
        drawSurface( cloudImage, screen, x, y);
    }
}
