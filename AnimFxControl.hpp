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


#ifndef ANIMFXCONTROL_HPP_INCLUDED
#define ANIMFXCONTROL_HPP_INCLUDED

//******************************************************************************
//
//                              AnimFxControl
//
//  Allows AnimFxes to be turned on and off by the Animation class.
//
//******************************************************************************

#include <string>
#include <memory>
#include "INFPacket.hpp"


class AnimFx;

class AnimFxControl{
    public:
        enum FxType{ FLASH, PARTICLE, TINT, FADE };
        enum Action{ NOTHING, START, STOP };

        static bool validFxType(int fx);
        static bool validAction(int act);


        AnimFxControl();
        ~AnimFxControl();

        enum FxType getType()const;
        enum Action getAction()const;
        std::string getId()const;

        void setType(enum FxType type);
        void setAction(enum Action action);
        void setId(const std::string& );

        //returns correct animFx(child) object according to type
        std::auto_ptr<AnimFx> getFx();



        //Returns a INFPacket containing the tile
        inp::INFPacket savePacket()const;

        //  Loads data from a packet
        bool loadPacket( inp::INFPacket& pack );


    private:
        // type of fx
        enum FxType type_;
        //  action to perform on AnimFx object
        enum Action action_;
        //  id to give the AnimFx object
        std::string id_;
};


#endif // ANIMFXCONTROL_HPP_INCLUDED
