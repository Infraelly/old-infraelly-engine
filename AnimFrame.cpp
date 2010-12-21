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


//  AnimFrame.hpp

#include "AnimFrame.hpp"


AnimFrame::AnimFrame() :
    x_(0),
    y_(0),
    angle_(0),
    zoom_(0)
{ ; }

AnimFrame::AnimFrame(int x, int y, int angle, float zoom) :
    x_(x),
    y_(y),
    angle_(angle),
    zoom_(zoom)
{ ; }

AnimFrame::~AnimFrame(){ ; }


int AnimFrame::getX()const{ return x_; }

int AnimFrame::getY()const{ return y_; }

int AnimFrame::getAngle()const{ return angle_; }

float AnimFrame::getZoom()const{ return zoom_; }


void AnimFrame::setX(int x){ x_ = x; }

void AnimFrame::setY(int y){ y_ = y; }

void AnimFrame::setAngle(int angle){
    angle_ = angle;
    //strip sign
    if( angle_ < 0 ){
        angle_ *= -1;
    }
    //wrap value around
    angle_ %= 360;
    //restore sign
    if( angle < 0 ){
        angle_ *= -1;
    }
}

void AnimFrame::setZoom(float zoom){ zoom_ = zoom; }




inp::INFPacket AnimFrame::savePacket()const{
    inp::INFPacket pack;

    pack << x_;
    pack << y_;
    pack << angle_;
    pack << zoom_;

    return pack;
}


bool AnimFrame::loadPacket( inp::INFPacket& pack ){
    if( pack.empty() || pack.readDone() ){ return 0; }

    //warp file
    pack >> x_;
    if( pack.readDone() ){ return 0; }

    pack >> y_;
    if( pack.readDone() ){ return 0; }

    pack >> angle_;
    if( pack.readDone() ){ return 0; }

    pack >> zoom_;

    return true;
}
