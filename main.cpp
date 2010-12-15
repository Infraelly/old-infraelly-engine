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


// main.cpp

#include "init.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "Screen.hpp"
#include "colours.hpp"

#include "AnimatorContext.hpp"
#include "GameContext.hpp"
#include "IntroContext.hpp"
#include "ItemMakerContext.hpp"
#include "MapperContext.hpp"
#include "ServerContext.hpp"
#include "TestContext.hpp"
#include "PackerContext.hpp"

/*--------------
access checkers
---------------*/
#include "checkExcogitator.hpp"
#include "checkForge.hpp"
#include "checkBreath.hpp"
#include "checkAnimator.hpp"
#include "checkNucleus.hpp"
#include "checkTest.hpp"
#include "checkPacker.hpp"

#include "GameConfig.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


int main(int argc, char *argv[]){
    enum RunMode{
        ANIMATOR,
        GAME,
        ITEM_EDITOR,
        MAP_EDITOR,
        NPC_EDITOR,
        PACKER,
        SERVER,
        TEST
    }runMode;
    runMode = GAME;

    //  Commandline
    if (argc > 1){
        std::string arg1 = argv[1];
        arg1 = upperCase(arg1);
        if( arg1 == "-ANIMATOR" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkAnimator())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering animator mode..." << endl << endl;
                runMode = ANIMATOR;
            }
        } else
        if( arg1 == "-ITEM" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkForge())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering item editor mode..." << endl << endl;
                runMode = ITEM_EDITOR;
            }
        } else
        if( arg1 == "-MAPPER" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkExcogitator())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering map editor mode..." << endl << endl;
                runMode = MAP_EDITOR;
            }
        } else
        if( arg1 == "-NPC" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkBreath())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering item editor mode..." << endl << endl;
                runMode = NPC_EDITOR;
            }
        } else
        if( arg1 == "-PACKER" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkPacker())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering packer mode..." << endl << endl;
                runMode = PACKER;
            }
        } else
        if( arg1 == "-TEST" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkTest())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering test bed..." << endl << endl;
                GameConfig::wScreenWidth = GameConfig::wTestScreenWidth;
                GameConfig::wScreenHeight = GameConfig::wTestScreenHeight;
                runMode = TEST;
            }
        } else
        if( arg1 == "-SERVER" ){
            //check permision file
            #ifndef DEBUG
            if(isk::checkNucleus())
            #endif
            {
                cerr << __FILE__ << " " << __LINE__ << ": " << "Entering server mode..." << endl << endl;
                GameConfig::wScreenBpp = 8;
                GameConfig::fScreenBpp = 8;
                runMode = SERVER;
            }
        } else
        if( arg1 == "-SUP" ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "nm, u?" << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "kthnx bye" << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "Exiting" << endl;
            return 0;
        } else
        if( arg1 == "-CRASH" ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Entering crash mode..." << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "SHIT, you encountered a level 666 Satan!" << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "Trying to kill Satan." << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "You swing your sword, dealing 0 damage to Satan." << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "Satan Spits Flames of Hell's Fury at you, dealing 666 damage." << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "Your take 666 burn damage." << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "You Fail. Infraelly is dead..." << endl << endl << endl;
            int i = 0;
            long *foo[1];
            while(++i){ delete foo[i]; }
        } else {
            runMode = GAME;
        }
    }


    try{
        init();
        switch( runMode ){
            case GAME:
                if( !GameConfig::bypassMainMenu ){
                    IntroContext();
                } else {
                    GameContext(NULL);
                }
                break;

            case SERVER:
                ServerContext();
                break;

            case MAP_EDITOR:
                MapperContext();
                break;

            case ITEM_EDITOR:
                ItemMakerContext();
                break;

            case ANIMATOR:
                AnimatorContext();
                break;

            case PACKER:
                if(argc > 2 && upperCase(argv[2]) == "-S"){
                    PackerContext(true);
                } else {
                    PackerContext();
                }
                break;

            case NPC_EDITOR:
                cerr << __FILE__ << " " << __LINE__ << ": " << "todo: npc maker" << endl;
                //NpcMakerContext();
                break;

            case TEST:
                TestContext();
                break;

            default:
                cerr << __FILE__ << " " << __LINE__ << ": " << "HOLY CRAP! Unknown runMode!!!" << endl;
                break;
        }
    }
    catch (gcn::Exception e){
        std::cerr << e.getMessage() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception e){
        std::cerr << "Std exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...){
        std::cout << "Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }

    fadeTo(Screen::getSurface(), colour::black, 250);
    return EXIT_SUCCESS;
}
