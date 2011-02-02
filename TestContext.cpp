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


#include <string>
#include <iostream>
#include "globalFunc.hpp"
#include "Screen.hpp"
#include "TestContext.hpp"
#include "TSpriteIcon.hpp"
#include "TilesetIcon.hpp"
#include "Tileset.hpp"
#include "caches.hpp"
#include "Tile.hpp"
#include "Layer.hpp"
#include "AreaMap.hpp"
#include "SDL/SDL_gfxBlitFunc.h"
#include "INFPacket.hpp"
#include <sstream>
#include "Connection.hpp"
#include "INFPacket.hpp"
#include "AreaMap.hpp"
#include <unzip.h>


using namespace inp;
using namespace std;

TestContext::TestCallBack::TestCallBack(){}
TestContext::TestCallBack::~TestCallBack(){}
void TestContext::TestCallBack::operator()(const std::string& text){}



TestContext::TestContext() :
    console(new Console(screen)),
    firstRun_(true)
{
    // Set window caption
    SDL_WM_SetCaption("Infraelly TestBed", NULL);
    //set fps
    fpsManager.setMaxFps(30);
    //set console call back
    console->setInputCallBack(&testCallBack);
    //initialise the local player
    player.setX(Screen::getWidth()/2);
    player.setY(Screen::getHeight()/2);
    player.setName("Bob");

    //start
    run();
}

TestContext::~TestContext(){
    delete console;
}


void TestContext::events(SDL_Event &event){
    console->pushInput(event);
}

void TestContext::input(Uint8 *keys){
    //directions
    if ( keys[SDLK_UP] ) { if(player.getY() > 0) player.move( UP ); }
    if ( keys[SDLK_LEFT] ) { if(player.getX() > 0) player.move( LEFT ); }
    if ( keys[SDLK_DOWN] ) { if(player.getY() < screen->h-70) player.move( DOWN ); }
    if ( keys[SDLK_RIGHT] ) { if(player.getX() < screen->w-40) player.move( RIGHT ); }

    //attack with space
    if ( keys[SDLK_SPACE] ) { player.attack(); }
    if ( keys[SDLK_RETURN] ) { player.stop(); }
}

void TestContext::logic(){
    //things here will only be run once
    if(firstRun_){
        firstRun_ = false;

        unzFile testArch = unzOpen( correctFilepath("data/test.zip").c_str() );
        if(testArch == NULL){
            console->push_back("error occured opening file: data/test.zip");
        } else {
            unz_global_info testInfo;
            unzGetGlobalInfo(testArch, &testInfo);
            int file_count = testInfo.number_entry;
            console->push_back("Entries in file: " + itos(file_count));

            unzGoToFirstFile(testArch);
            char fn[256];
            for(int i = 0; i < file_count; ++i){
                unzOpenCurrentFile(testArch);
                unzGetCurrentFileInfo64( testArch,
                                        NULL,
                                        &fn[0], 256,
                                        NULL, NULL,
                                        NULL, NULL );
                console->push_back(fn);
                unzCloseCurrentFile(testArch);
                unzGoToNextFile(testArch);
            }
            unzGoToFirstFile(testArch);
        }

        unzClose(testArch);
    }


    // let giuchan do its thang!
    console->logic();
}


void TestContext::draw(){
    //  let guichan draw on the screen =]
    console->draw();

    player.draw(screen);

    SDL_Flip(screen);
}
