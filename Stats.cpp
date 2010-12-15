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


//Stats.cpp

#include "Stats.hpp"

#include "INFPacket.hpp"


bool Stats::validStat(int stat){
    return ( (stat==HP)      || (stat==MP)      || (stat==EXP)    ||
             (stat==STAM)    || (stat==STR)     || (stat==DEF)    ||
             (stat==SPD)     || (stat==INT)     || (stat==DEX)    ||
             (stat==CHAR)    || (stat==RES)     || (stat==CON)    ||
             (stat==LUCK)    || (stat==LVL)     || (stat==MAX_HP) ||
             (stat==MAX_EXP) || (stat==MAX_MP)  || (stat==MAX_STAM)     );
}

Stats::Stats(){
    for( unsigned i = 0; i < size_; ++i ){ values_[i] = 0; }
}

Stats::~Stats(){}


const Sint32& Stats::getValue(const enum StatObjects& stat)const{ return values_[stat]; }

void Stats::setValue(const enum StatObjects& stat, const Sint32 value){
    values_[stat] = value;
}

void Stats::addValue(const enum StatObjects& stat, const Sint32 value){
    values_[stat] += value;
}

bool Stats::savePacket( inp::INFPacket& packet )const{
    for(int i = 0; i < size_; ++i){
        packet << values_[i];
    }
    return true;
}

bool Stats::loadPacket( inp::INFPacket& packet ){
    for(int i = 0; i < size_; ++i){
        if( packet.readDone() ){ return false; }
        packet >> values_[i];
    }
    return true;
}
