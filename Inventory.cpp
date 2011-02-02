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


//  Inventoy.cpp

#include "Inventory.hpp"

#include <cassert>
#include <algorithm>

#include "INFPacket.hpp"


Inventory::Inventory() :
    maxSize_(startSize_)
{
    list_.reserve(startSize_*2);
}

Inventory::~Inventory(){}


void Inventory::clear(){ list_.clear(); }

bool Inventory::empty()const{ return list_.empty(); }

size_t Inventory::size()const{ return list_.size(); }

const Item& Inventory::at(size_t i)const{
    assert(i < list_.size());
    return list_[i];
}

void Inventory::setMaxSize(int size){
    maxSize_ = size;
    list_.reserve(maxSize_);
}

int Inventory::getMaxSize()const{ return maxSize_; }


void Inventory::gainItem(const Item &newItem){
    list_.push_back(newItem);
}


void Inventory::removeItem(size_t i){
    assert(i < list_.size());
    ItemList::iterator itr = list_.begin();
    std::advance(itr, i);
    list_.erase( itr );
}



bool Inventory::savePacket(inp::INFPacket& pack)const{
    pack << revision_;
    //write number of items
    pack << (int) list_.size();

    //write items
    for( size_t i = 0; i < list_.size(); ++i ){
        if( !list_[i].savePacket(pack) ){ return false; }
    }

    return true;
}

bool Inventory::load(inp::INFPacket &pack){
    {
        int fileVer;
        pack >> fileVer;
        if( fileVer != revision_ ){
            std::cerr << __FILE__ << __LINE__ << "incompatible file version" << std::endl;
            return 0;
        }
    }

    //clean out existing inventory
    clear();

    int numOfItems;
    pack >> numOfItems;

    for( size_t i = 0; i < numOfItems; ++i ){
        if( pack.readDone() ){ return 0; }

        Item tmp;
        if( !tmp.loadPacket(pack) ){ return false; }
        list_.push_back(tmp);
    }

    return true;
}


