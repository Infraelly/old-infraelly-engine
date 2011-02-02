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


#ifndef ANIMFRAME_HPP_INCLUDED
#define ANIMFRAME_HPP_INCLUDED

//******************************************************************************
//
//                              AnimFrame
//
//  AnimFrame details a single stage(or frame) of an object's transformation
//  during an animation.
//
//  In a single frame, an object can be translated along the x and y axis, and
//  rotated.
//
//  In animations, the transformation values used are all reletive to the
//  objects current orientation. The exception is Cutscenes, where the x-y
//  values are absolute screen positions; the rotations are still reletive.
//
//  NOTE: Angles are in degrees.
//        Anlges range -359~359 (inc. 0). Angles larger or smaller
//        will be wrapped around to meet these requirements.
//
//******************************************************************************

#include "INFPacket.hpp"

class AnimFrame{
    public:
        AnimFrame();
        AnimFrame(int x, int y, int angle = 0, float _zoom = 1.00);
        ~AnimFrame();

        int getX()const;
        int getY()const;
        int getAngle()const;
        float getZoom()const;

        void setX(int x);
        void setY(int y);
        void setPosition(int x, int y){ setX(x); setY(y); }
        void setAngle(int angle);
        void setZoom(float factor);


        //Returns a INFPacket containing the tile
        inp::INFPacket savePacket()const;

        //  Loads data from a packet
        bool loadPacket( inp::INFPacket& pack );


    private:
        int x_;
        int y_;
        int angle_;
        float zoom_;
};


#endif // ANIMFRAME_HPP_INCLUDED
