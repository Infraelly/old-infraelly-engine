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


//  NPC.cpp

#include "NPC.hpp"

#include <iostream>
#include "globalFunc.hpp"

using namespace std;

NPC::NPC(){}

NPC::NPC(const std::string& filename){
    load( correctFilepath(filename) );
}

NPC::~NPC(){}


//  save/load
bool NPC::save(std::ofstream &file){ return 0; }

bool NPC::load(std::ifstream &file){ return 0; }

bool NPC::save(const std::string& filename){
    std::string fixedFn = correctFilepath(filename);
    ofstream file(fixedFn.c_str());
    if (!file.is_open()){
        file.close();
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to open file (ofstream): " << fixedFn << endl;
        return 0;
    } else {
        return save(file);
    }
}

bool NPC::load(const std::string& filename){
    std::string fixedFn = correctFilepath(filename);
    ifstream file(fixedFn.c_str(), ios::binary);
    if (!file.is_open()){
        file.close();
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to open file (ifstream): " << fixedFn << endl;
        return 0;
    } else {
        return load(file);
    }
}

