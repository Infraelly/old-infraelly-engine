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


//  AnimFxControl.cpp

#include "AnimFxControl.hpp"
#include "AnimFx.hpp"


bool AnimFxControl::validFxType(int fx){
    return ( (fx==FLASH) || (fx==PARTICLE) || (fx==TINT) || (fx==FADE) );
}

bool AnimFxControl::validAction(int act){
    return ( (act==NOTHING) || (act==START) || (act==STOP) );
}



AnimFxControl::AnimFxControl() :
    type_(FLASH),
    action_(NOTHING),
    id_("no_id_given")
{ ; }


AnimFxControl::~AnimFxControl(){}


enum AnimFxControl::FxType AnimFxControl::getType()const{ return type_; }

enum AnimFxControl::Action AnimFxControl::getAction()const{ return action_; }

std::string AnimFxControl::getId()const{ return id_; }


void AnimFxControl::setType(enum AnimFxControl::FxType type){ type_ = type; }

void AnimFxControl::setAction(enum AnimFxControl::Action action){
    action_ = action;
}

void AnimFxControl::setId(const std::string& id){ id_ = id; }


std::auto_ptr<AnimFx> AnimFxControl::getFx(){
    switch(type_){
        default:
            //return null for now (no anim fx made yet)
            return std::auto_ptr<AnimFx>(NULL);
//            return std::auto_ptr<AnimFX>(new X);
    }
}



inp::INFPacket AnimFxControl::savePacket(){
    inp::INFPacket pack;

	pack << static_cast<int>(type_);
	pack << static_cast<int>(action_);

    pack << id_;

    return pack;
}


bool AnimFxControl::loadPacket( inp::INFPacket& pack ){
    if( pack.empty() || pack.readDone() ){ return 0; }

    int atrib;

	pack >> atrib;
	if( !validFxType(atrib) ){ return 0; }
	type_ = static_cast<FxType>(atrib);
	if( pack.readDone() ){ return 0; }

	pack >> atrib;
	if( !validAction(atrib) ){ return 0; }
	action_ = static_cast<Action>(atrib);
	if( pack.readDone() ){ return 0; }

    pack >> id_;

    return true;
}
