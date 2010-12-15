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


#include "KeyPointList.hpp"

using std::string;
using std::pair;

KeyPointList::KeyPointList(){
    pair<string, enum Animation::KeyPoint> TL("Top Left", Animation::TL);
    pair<string, enum Animation::KeyPoint> TOP("Top", Animation::TOP);
    pair<string, enum Animation::KeyPoint> TR("Top Right", Animation::TR);
    pair<string, enum Animation::KeyPoint> LEFT("Left", Animation::LEFT);
    pair<string, enum Animation::KeyPoint> MID("Middle", Animation::MID);
    pair<string, enum Animation::KeyPoint> RIGHT("Right", Animation::RIGHT);
    pair<string, enum Animation::KeyPoint> BL("Bottom Left", Animation::BL);
    pair<string, enum Animation::KeyPoint> BOT("Bottom", Animation::BOT);
    pair<string, enum Animation::KeyPoint> BR("Bottom Right", Animation::BR);

    values.push_back(TL);
    values.push_back(TOP);
    values.push_back(TR);
    values.push_back(LEFT);
    values.push_back(MID);
    values.push_back(RIGHT);
    values.push_back(BL);
    values.push_back(BOT);
    values.push_back(BR);
}

KeyPointList::~KeyPointList(){ ; }

enum Animation::KeyPoint KeyPointList::at(int i){
    return values.at(i).second;
}


int KeyPointList::getNumberOfElements(){ return values.size(); }

std::string KeyPointList::getElementAt(int i){
    if( i == values.size() ){ return "access violation"; }
    return values.at(i).first;
}
