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


#include "AnimFrameList.hpp"
#include <algorithm>

using std::string;
using std::pair;


bool AnimFrameList::empty()const{ return values.empty(); }

AnimFrame& AnimFrameList::at(size_t i){ return values.at(i).second; }

std::pair<std::string, AnimFrame>& AnimFrameList::getPair(size_t i){ return values.at(i); }

int AnimFrameList::addElement(const string name, const AnimFrame& newFrame){
    pair<string, AnimFrame> temp(name, newFrame);
    values.push_back(temp);
    return 1;
}

void AnimFrameList::removeElementAt(size_t i){
    if( (i >= 0) && (i < values.size()) ){
        std::vector< pair<string, AnimFrame> >::iterator itr = values.begin();
        std::advance(itr, i);
        values.erase(itr);
    }
}

void AnimFrameList::clear(){ values.clear(); }

void AnimFrameList::swap(size_t a, size_t b) throw() {
    if( !values.empty() ){
        if( (a < values.size()) && (b < values.size()) ){
            std::swap( values.at(a), values.at(b) );
        }
    }
}



int AnimFrameList::getNumberOfElements(){ return values.size(); }

std::string AnimFrameList::getElementAt(int i){
    if( i == values.size() ){ return "access violation"; }

    return values.at(i).first;
}
