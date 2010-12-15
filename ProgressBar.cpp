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


//  ProgressBar.cpp

#include "ProgressBar.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include "fonts.hpp"
#include "guichan/contrib/sdl/sdltruetypefont.hpp"
//center
#include "globalFunc.hpp"

ProgressBar::ProgressBar(){
    setFont( font::guiFont.at(12) );
    setBaseColor( gcn::Color( 255, 0, 0) );
    setBackgroundColor( gcn::Color( 0, 255, 0) );
    setForegroundColor( gcn::Color( 0, 0, 0) );
    setWidth(100);
    setHeight(17);
    prefix = "";
    mode = PERCENTAGE;
    showText = 1;
    progress = 1.0;
    total = 1.0;
}

ProgressBar::ProgressBar(const std::string& newPrefix, float newProgress, float totalAmount){
    setFont( font::guiFont.at(12) );
    setBaseColor( gcn::Color( 255, 0, 0) );
    setBackgroundColor( gcn::Color( 0, 255, 0) );
    setForegroundColor( gcn::Color( 0, 0, 0) );
    setWidth(100);
    setHeight(17);
    prefix = newPrefix;
    mode = PERCENTAGE;
    showText = 1;
    progress = newProgress;
    total = totalAmount;
}

ProgressBar::~ProgressBar(){}



enum ProgressBar::ProgressMode ProgressBar::getProgressMode()const{ return mode; }

bool ProgressBar::isShowText()const{ return showText; }

std::string ProgressBar::getPrefix()const{ return prefix; }

float ProgressBar::getProgress()const{ return progress; }

float ProgressBar::getTotal()const{ return total; }




void ProgressBar::setProgressMode(enum ProgressBar::ProgressMode newMode){ mode = newMode; }

void ProgressBar::setShowText(bool newShowText){
    showText = newShowText;
}

void ProgressBar::setPrefix(const std::string& newPrefix){
    prefix = newPrefix;
}

void ProgressBar::setProgress(float newProgress){
    progress = newProgress;
    if( progress < 0 ){ progress = 0; };
    if( total < progress ){
        total = progress;
    };
}

void ProgressBar::setTotal(float newTotal){
    total = newTotal;
    if( total < 0 ){ total = 0; };
    if ( progress > total ){
        progress = total;
    };
}




void ProgressBar::draw(gcn::Graphics* graphics){
    //draw a border
    graphics->setColor( gcn::Color( 0,0,0) );
    graphics->drawRectangle(  gcn::Rectangle( -1, -1,
                                              mDimension.width+2, mDimension.height+2 )
                            );


    //setcolour to base
    graphics->setColor( mBaseColor );
    //draw backbit
    graphics->fillRectangle( gcn::Rectangle(0, 0, getWidth(), getHeight()) );


    //if theres a need to draw the front bar
    if( (progress > 0) && (total > 0) ){
        //set color to background
        graphics->setColor( mBackgroundColor );
        // drawing front bar
        graphics->fillRectangle( gcn::Rectangle(0, 0,
                                                int(  ceil( mDimension.width*((float)progress/total) )  ),
                                                mDimension.height )
                                );
    }

    //check if we shud draw the text
    if( showText ){
        std::string text = "";
        //set color foregound colour
        graphics->setColor( mForegroundColor );
        //check if we shud draw the prefix
        if( prefix != "" ){ text = prefix; };

        //check which mode to use (fraction or percentage or none)
        switch( mode ){
            case FRACTION:{
                //generate the fraction
                std::ostringstream fractionString;
                fractionString << progress << "/" << total;
                text += fractionString.str();
                break;
            }
            case PERCENTAGE:{
                //make a percentage
                std::ostringstream percentageString;
                percentageString << (float)progress*100/total << "%";
                text += percentageString.str();
                break;
            }
            case NONE:
                break;
        }

        //draw centered sting
        graphics->drawText( text,
                            centerX( mCurrentFont->getWidth(text), getWidth() ),
                            centerY( mCurrentFont->getHeight(), getHeight() ) );
    }
}
