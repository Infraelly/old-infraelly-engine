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


#include "SnapPointList.hpp"

using std::string;
using std::pair;


SnapPointList::SnapPointList(){
    pair<string, enum Animation::SnapPoint> HEAD("Head", Animation::HEAD);
    pair<string, enum Animation::SnapPoint> BODY("Body", Animation::BODY);
    pair<string, enum Animation::SnapPoint> LHAND("Left Hand", Animation::LEFT_HAND);
    pair<string, enum Animation::SnapPoint> RHAND("Right Hand", Animation::RIGHT_HAND);
    pair<string, enum Animation::SnapPoint> LFOOT("Left Foot", Animation::LEFT_FOOT);
    pair<string, enum Animation::SnapPoint> RFOOT("Right Foot", Animation::RIGHT_FOOT);

    values.push_back(HEAD);
    values.push_back(BODY);
    values.push_back(LHAND);
    values.push_back(RHAND);
    values.push_back(LFOOT);
    values.push_back(RFOOT);
}

SnapPointList::~SnapPointList(){ ; }

enum Animation::SnapPoint SnapPointList::at(int i){
    return values.at(i).second;
}


int SnapPointList::getNumberOfElements(){ return values.size(); }

std::string SnapPointList::getElementAt(int i){
    if( i == values.size() ){ return "access violation"; }
    return values.at(i).first;
}
