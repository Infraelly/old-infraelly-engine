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


//  IntroContext.hpp

#include "IntroContext.hpp"

#include <iostream>
#include <sstream>
#include <cstddef>
#include <SDL/SDL_image.h>
#include "globalFunc.hpp"
#include "colours.hpp"
#include "fonts.hpp"
#include "InfraellyMixer.hpp"
#include "version.h"

#include "dataPacks.hpp"

IntroContext::IntroContext() :
    gui( new IntroGui(screen) ),
    background( duplicateSurface(screen) ),
    sky( NULL ),
    hillsColour( NULL ),
    hills( NULL ),
    sun( NULL ),
    clouds(6),
    cloudSpawn(0),
    sdl_powered(NULL)
{
    // Set window caption
    SDL_WM_SetCaption("Welcome Infraelly", NULL);
    //to control fps
    FpsManager fpsManager(40);

    //start music
    InfraellyMixer::clearMusicQueue();
    InfraellyMixer::playMusic("audio/are_we_talking_yet.ogg", -1);


    // load images
    ResourceFile rcf;
    packs::mainPack.getResource("images/intro/skyColour.png", rcf);
    sky = loadImage( SDL_RWFromMem( (void*)&packs::mainPack.getRawData()[rcf.start], rcf.size), true );

    packs::mainPack.getResource("images/intro/hillColour.png", rcf);
    hillsColour = loadImage( SDL_RWFromMem( (void*)&packs::mainPack.getRawData()[rcf.start], rcf.size), true );

    packs::mainPack.getResource("images/intro/hills.png", rcf);
    hills = loadImage( SDL_RWFromMem( (void*)&packs::mainPack.getRawData()[rcf.start], rcf.size), true );

    packs::mainPack.getResource("images/intro/sun.png", rcf);
    sun = loadImage( SDL_RWFromMem( (void*)&packs::mainPack.getRawData()[rcf.start], rcf.size), true );



    //initialise cloud positions/speeds
    for( size_t i = 0; i < clouds.size(); ++i ){
        clouds.at(i).setX( random(0, screen->w) );
        clouds.at(i).setY( random(0, screen->h/4) );
        clouds.at(i).setSpeed( random(1, 8) );
    }
    if( IntroCloud::getWindDir() && (IntroCloud::getCloudImage() != NULL) &&
        (background != NULL) ){
        cloudSpawn = -IntroCloud::getCloudImage()->w;
    } else {
        cloudSpawn = background->w;
    }

    // sdl_logo
    {
        #include "SDL_POWERED.XPM"
        sdl_powered = IMG_ReadXPMFromArray(SDL_POWERED_xpm);
    }
    if(!sdl_powered) {
       std::cerr << __FILE__ << " " << __LINE__ << ": " << "IMG_ReadXPMFromArray: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetAlpha(sdl_powered, SDL_SRCALPHA|SDL_RLEACCEL, 128);
    }

    //generate the landscape
    buildBackground();

    //start
    run();
}

IntroContext::~IntroContext(){
    delete gui;
    SDL_FreeSurface(background);
    SDL_FreeSurface(sky);
    SDL_FreeSurface(hillsColour);
    SDL_FreeSurface(hills);
    SDL_FreeSurface(sun);
    SDL_FreeSurface(sdl_powered);
}

void IntroContext::buildBackground(){
    if( background == NULL ){
        background = duplicateSurface(screen);
    }
    if( background != NULL ){
        //re create bg (taken from above)
        SDL_FreeSurface(background);
        background = duplicateSurface(screen);

        //sky
        for( int x = 0; x < background->w; x += sky->w){
            drawSurface(sky, background, x, 0);
        }
        //sun
        drawSurface(sun, background, background->w - sun->w, 0);
        //hills
        for( int x = 0; x < background->w; x += hills->w){
            drawSurface(hills, background, x, background->h/2);
        }
        //hills' colour
        for( int x = 0; x < background->w; x += hillsColour->w){
            drawSurface(hillsColour, background, x, (background->h/2)+hills->h-1);
        }
        // reset cloud start pos
        if( (IntroCloud::getWindDir()) && (IntroCloud::getCloudImage() != NULL) &&
            (background != NULL) ){
            cloudSpawn = -IntroCloud::getCloudImage()->w;
        } else {
            cloudSpawn = background->w;
        }
    }
}

void IntroContext::events(SDL_Event &event){
    if( (event.type == SDL_VIDEORESIZE) || (event.key.keysym.sym == SDLK_F4) ){
        // Window has been resized
        buildBackground();
        gui->resetPositions();
    }

    if ( (event.key.keysym.sym == SDLK_ESCAPE) ||
         (event.type == SDL_QUIT) ){
         done = true;
    }

    gui->pushInput(event);
}

void IntroContext::input(Uint8 *keys){}

void IntroContext::logic(){
    //move clouds across sky
    for(size_t i = 0; i < clouds.size(); ++i){
        clouds.at(i).move();
        if( (IntroCloud::getCloudImage() != NULL) &&
            ((clouds.at(i).getX() > screen->w+IntroCloud::getCloudImage()->w+10)
            || (clouds.at(i).getX() < -10-IntroCloud::getCloudImage()->w)) ){
            clouds.at(i).setX(cloudSpawn);
        }
    }

    //  mixer logic \O.o/
    InfraellyMixer::logic();
    // let giuchan do its thang!
    gui->logic();
}

void IntroContext::draw(){
    //put bg onto screen
    drawSurface(background, screen, 0, 0);

    //draw clouds
    for(size_t i = 0; i < clouds.size(); ++i){
        clouds.at(i).draw(screen);
    }

    fpsManager.draw(screen, 0, 0);

    //draw sdl thing
    drawSurface(sdl_powered, screen, screen->w-sdl_powered->w, screen->h-sdl_powered->h);

    //draw version number
    drawText(itos(AutoVersion::_REVISION), font::mainFont.at(16), colour::blue, screen, 1, screen->h-12 );

    //draw gui
    gui->draw();
}
