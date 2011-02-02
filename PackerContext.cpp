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
#include <fstream>
#include <algorithm>
#include "globalFunc.hpp"
#include "Screen.hpp"
#include "PackerContext.hpp"
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
#include "ResourcePack.hpp"

using namespace inp;
using namespace std;

PackerContext::PackerContextCallBack::PackerContextCallBack(){}
PackerContext::PackerContextCallBack::~PackerContextCallBack(){}
void PackerContext::PackerContextCallBack::operator()(const std::string& text){}



PackerContext::PackerContext(bool silent) :
    console(new Console(screen)),
    firstRun_(true),
    silent_(silent)
{
    // Set window caption
    SDL_WM_SetCaption("Infraelly Resource Packer", NULL);
    //set fps
    fpsManager.setMaxFps(30);
    //set console call back
    console->setInputCallBack(&testCallBack);

    //start
    run();
}

PackerContext::~PackerContext(){
    std::cout << std::endl << "-Packer Console dump-" << std::endl;
    std::cout << console->getTextArea()->getText() << std::endl << std::endl;

    delete console;
}


void PackerContext::events(SDL_Event &event){
    console->pushInput(event);
}

void PackerContext::input(Uint8 *keys){
}

void PackerContext::logic(){
    //things here will only be run once
    if(firstRun_){
        firstRun_ = false;

        bool error = false;

        ResourcePack dataPack;
        vector<string> filelist;
        std::ifstream filelist_if;
        filelist_if.open("dataFilelist.txt");
        if( !filelist_if.is_open() ){
            console->push_back("Failed to open fileist: dataFilelist.txt");
        } else {
            //read file list
            while( !filelist_if.eof() && filelist_if.good() ){
                char buff[512];
                filelist_if.getline(&buff[0], 512);
                string tmp = correctFilepath(buff);
                if( tmp == "." ) continue;
                if( tmp.find("./") == 0 ){
                    tmp.erase( tmp.begin() );
                    tmp.erase( tmp.begin() );
                }
                filelist.push_back( tmp );
            }
            //make reletive paths
            // this will turn any absolute filepaths into paths reletive to the
            // current directory. the packer will try to add all files in the
            //  filelist which are also in a directory named "data" in the
            //  current working directory
            string rootDir = "data/";
            int pathStart = rootDir.length();
            int rootDirStart = filelist[0].rfind(rootDir);
            if( rootDirStart != -1 ){
                pathStart += rootDirStart;
            }
            string basepath( filelist[0], 0, pathStart );
            makeReletivePaths(basepath, filelist);

            //add files to pack
            for(unsigned int i = 0; i < filelist.size(); ++i){
                if( !isSpace(filelist[i]) && !isDir(rootDir+filelist[i]) ){
                    if(dataPack.addFile(filelist[i])){
                        console->push_back("File added: " + filelist[i]);
                    } else {
                        console->push_back("File add failed: " + filelist[i]);
                    }
                }
            }

            //write data pack to file
            if( dataPack.writePack("data.ipf") ){
                console->push_back("Pack written: data.ipf");
            } else {
                console->push_back("Pack write failed");
                error = true;
            }

            //test pack
            if( dataPack.loadPack("data.ipf") ){
                console->push_back("Pack reloaded sucessfuly");
            } else {
                console->push_back("Pack load failed =[");
                error = true;
            }
            if(dataPack.checkPack()){
                console->push_back("Pack check good");
            } else {
                console->push_back("Pack check bad =[");
                error = true;
            }
        }

        dataPack.dumpIndex("data_ipf_index.txt");

        console->push_back("Done");

        if(!error && silent_){ done=true; }
    }

    // let giuchan do its thang!
    console->logic();
}


void PackerContext::draw(){
    //  let guichan draw on the screen =]
    console->draw();

    SDL_Flip(screen);
}
