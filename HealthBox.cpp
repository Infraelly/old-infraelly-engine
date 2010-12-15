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


//  HealthBox.cpp

#include "HealthBox.hpp"
#include "fonts.hpp"

using namespace std;


unsigned short HealthBox::alpha = 255;

HealthBox::HealthBox(){ init(); }

HealthBox::HealthBox(Character *newSource){
    init();
    characterSource = newSource;
    statsSource = &newSource->stats;
    setCaption("Health - " + characterSource->getName());
}

HealthBox::HealthBox(Stats *newSource){
    init();
    statsSource = newSource;
}

HealthBox::~HealthBox(){
    delete hpBarText;
    delete mpBarText;
    delete stamBarText;
    delete expBarText;

    delete hpBarAmount;
    delete mpBarAmount;
    delete stamBarAmount;
    delete expBarAmount;
}

void HealthBox::init(){
    setCaption("Health Box");
    setBaseColor( gcn::Color(182, 227, 174, alpha) );
    setBackgroundColor( gcn::Color(207, 235, 202, alpha) );
    setForegroundColor( gcn::Color(0 ,0 ,0 , alpha) );

    characterSource = NULL;
    statsSource = NULL;


    //init objcts
    hpBarText = new gcn::Label("HP: ");
    mpBarText = new gcn::Label("MP: ");
    stamBarText = new gcn::Label("Stam: ");
    expBarText = new gcn::Label("Exp: ");

    hpBarAmount = new ProgressBar("", 0, 0);
    mpBarAmount = new ProgressBar("", 0, 0);
    stamBarAmount = new ProgressBar("", 0, 0);
    expBarAmount = new ProgressBar("", 0, 0);


    //setfont
    hpBarAmount->setFont( font::guiFont.at(font::stdGuiFontSize) );
    mpBarAmount->setFont( font::guiFont.at(font::stdGuiFontSize) );
    stamBarAmount->setFont( font::guiFont.at(font::stdGuiFontSize) );
    expBarAmount->setFont( font::guiFont.at(font::stdGuiFontSize) );


    //set co-ords (X)
    hpBarText->setX(0);
    mpBarText->setX(0);
    stamBarText->setX(0);
    expBarText->setX(0);

    hpBarAmount->setX( stamBarText->getWidth() + stamBarText->getX() +2);
    mpBarAmount->setX( stamBarText->getWidth() + stamBarText->getX() +2);
    stamBarAmount->setX( stamBarText->getWidth() + stamBarText->getX() +2);
    expBarAmount->setX( stamBarText->getWidth() + stamBarText->getX() +2);


    //set co-ords (Y)
    hpBarAmount->setY(3);
    mpBarAmount->setY( hpBarAmount->getHeight() + hpBarAmount->getY() +3);
    stamBarAmount->setY( mpBarAmount->getHeight() + mpBarAmount->getY() +3);
    expBarAmount->setY( stamBarAmount->getHeight() + stamBarAmount->getY() +3);

    hpBarText->setY(2);
    mpBarText->setY( mpBarAmount->getY() +2);
    stamBarText->setY( stamBarAmount->getY() +2);
    expBarText->setY( expBarAmount->getY() +2);


    //set colours
    hpBarText->setForegroundColor( gcn::Color( 0, 0, 0) );
    mpBarText->setForegroundColor( gcn::Color( 0, 0, 0) );
    stamBarText->setForegroundColor( gcn::Color( 0, 0, 0) );
    expBarText->setForegroundColor( gcn::Color( 0, 0, 0) );

    hpBarAmount->setBaseColor( gcn::Color( 249, 225, 225) );
    hpBarAmount->setBackgroundColor( gcn::Color( 111, 240, 120) );
    hpBarAmount->setForegroundColor( gcn::Color( 0, 0, 0) );

    mpBarAmount->setBaseColor( gcn::Color( 228, 249, 252) );
    mpBarAmount->setBackgroundColor( gcn::Color( 122, 179, 249) );
    mpBarAmount->setForegroundColor( gcn::Color( 0, 0, 0) );

    stamBarAmount->setBaseColor( gcn::Color( 252, 228, 251) );
    stamBarAmount->setBackgroundColor( gcn::Color( 194, 122, 245) );
    stamBarAmount->setForegroundColor( gcn::Color( 0, 0, 0) );

    expBarAmount->setBaseColor( gcn::Color( 252, 247, 228) );
    expBarAmount->setBackgroundColor( gcn::Color( 250, 222, 108) );
    expBarAmount->setForegroundColor( gcn::Color( 0, 0, 0) );


    //set size
    hpBarAmount->setWidth(100);
    hpBarAmount->setHeight(12);

    mpBarAmount->setWidth(100);
    mpBarAmount->setHeight(12);

    stamBarAmount->setWidth(100);
    stamBarAmount->setHeight(12);

    expBarAmount->setWidth(100);
    expBarAmount->setHeight(12);


    //add contents to window
    add(hpBarText);
    add(mpBarText);
    add(stamBarText);
    add(expBarText);

    add(hpBarAmount);
    add(mpBarAmount);
    add(stamBarAmount);
    add(expBarAmount);


    //resize to contents
    resizeToContent();
}


Character *HealthBox::getCharacterSource()const{ return characterSource; }

Stats *HealthBox::getStatsSource()const{ return statsSource; }

void HealthBox::setCharacterSource(Character *newSource){
    characterSource = newSource;
    setCaption("Health - " + characterSource->getName());
}

void HealthBox::setStatsSource(Stats *newSource){
    statsSource = newSource;
}

void HealthBox::logic(){
    if( characterSource != NULL ){
        setCaption("Health - " + characterSource->getName());
    }

    //if theres a source for values
    if( statsSource != NULL ){
        //update values
        hpBarAmount->setProgress( statsSource->getValue(Stats::HP) );
        hpBarAmount->setTotal( statsSource->getValue(Stats::MAX_HP) );
        mpBarAmount->setProgress( statsSource->getValue(Stats::MP) );
        mpBarAmount->setTotal( statsSource->getValue(Stats::MAX_MP) );
        stamBarAmount->setProgress( statsSource->getValue(Stats::STAM) );
        stamBarAmount->setTotal( statsSource->getValue(Stats::MAX_STAM) );
        expBarAmount->setProgress( statsSource->getValue(Stats::EXP) );
        expBarAmount->setTotal( statsSource->getValue(Stats::MAX_EXP) );
    }
}
