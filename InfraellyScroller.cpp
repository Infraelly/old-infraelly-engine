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


// InfraellyScroller.cpp

#include "InfraellyScroller.hpp"

#include <cmath>


InfraellyScroller::InfraellyScroller(){}

InfraellyScroller::InfraellyScroller(Widget *content){
    setContent(content);
}

InfraellyScroller::InfraellyScroller(Widget *content, ScrollPolicy hPolicy, ScrollPolicy vPolicy){
    mHPolicy = hPolicy;
    mVPolicy = vPolicy;

    setContent(content);
}



void InfraellyScroller::logic(){
    if( getContent() != NULL ){
        if( getContent()->getWidth() > getWidth() ){
            if( getHorizontalScrollAmount() > 0 ){
                float horPerc = (float) getHorizontalScrollAmount()/getHorizontalMaxScroll();
                getContent()->setX(   -int( ceil( (getContent()->getWidth() - getWidth())*horPerc ) )   );
            } else {
                getContent()->setX( 0 );
            }
        }

        if( getContent()->getHeight() > getHeight() ){
            if( getVerticalScrollAmount() > 0 ){
                float verPerc = (float) getVerticalScrollAmount()/getVerticalMaxScroll();
                getContent()->setY(   -int( ceil( (getContent()->getHeight() - getHeight())*verPerc) )   );
            } else {
                getContent()->setY( 0 );
            }
        }

        getContent()->logic();
    };

    checkPolicies();

    setVerticalScrollAmount(getVerticalScrollAmount());
    setHorizontalScrollAmount(getHorizontalScrollAmount());
}

