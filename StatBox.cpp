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


//  StatBox.cpp

#include "StatBox.hpp"
#include "globalFunc.hpp"

using namespace std;


unsigned short StatBox::alpha = 255;

StatBox::StatBox(){ init(); }

StatBox::StatBox(Character *newSource){
    init();
    characterSource = newSource;
    statsSource = &newSource->stats;
    setCaption("Stats - " + characterSource->getName());
}

StatBox::StatBox(Stats *newSource){
    init();
    statsSource = newSource;
}

StatBox::~StatBox(){
    for( int i = 0; i < 14; ++i ){
        delete labels[i][0];
        delete labels[i][1];
    }
}

void StatBox::init(){
    setCaption("Stat Box");
    setBaseColor( gcn::Color(170, 200, 245, alpha) );
    setBackgroundColor( gcn::Color(216, 239, 255, alpha) );
    setForegroundColor( gcn::Color(0 ,0 ,0 , alpha) );

    characterSource = NULL;
    statsSource = NULL;

    labels[0][0] = new gcn::Label("Level: ");
    labels[1][0] = new gcn::Label("HP: ");
    labels[2][0] = new gcn::Label("MP: ");
    labels[3][0] = new gcn::Label("Exp: ");
    labels[4][0] = new gcn::Label("Stam: ");
    labels[5][0] = new gcn::Label("Str: ");
    labels[6][0] = new gcn::Label("Def: ");
    labels[7][0] = new gcn::Label("Spd: ");
    labels[8][0] = new gcn::Label("Int: ");
    labels[9][0] = new gcn::Label("Dex: ");
    labels[10][0] = new gcn::Label("Char: ");
    labels[11][0] = new gcn::Label("Res: ");
    labels[12][0] = new gcn::Label("Con: ");
    labels[13][0] = new gcn::Label("Luck: ");

    labels[0][1] = new gcn::Label("0");
    labels[1][1] = new gcn::Label("0/0");
    labels[2][1] = new gcn::Label("0/0");
    labels[3][1] = new gcn::Label("0/0");
    labels[4][1] = new gcn::Label("0/0");

    for( unsigned int i = 5; i < 14; ++i ){
        labels[i][1] = new gcn::Label("0");
    }

    int tmpY = 10;
    for( unsigned int i = 0; i < 14; ++i ){
        labels[i][0]->setX(10);
        labels[i][1]->setX( labels[i][0]->getWidth() + labels[i][0]->getX() );

        labels[i][0]->setY(tmpY);
        labels[i][1]->setY(tmpY);

        tmpY += labels[i][0]->getHeight();
    }

    for( unsigned int i = 0; i < 14; ++i ){
        add(labels[i][0]);
        add(labels[i][1]);
    }

    resizeToContent();
    if( getWidth() < 100 ){ setWidth( 100 ); } else { setWidth(getWidth()+10); }
    //setHeight(getHeight()+10);
}


Character *StatBox::getCharacterSource()const{ return characterSource; }

Stats *StatBox::getStatsSource()const{ return statsSource; }

void StatBox::setCharacterSource(Character *newSource){
    characterSource = newSource;
    setCaption("Stats - " + characterSource->getName());
}

void StatBox::setStatsSource(Stats *newSource){
    statsSource = newSource;
}

void StatBox::logic(){
    if( characterSource != NULL ){
        setCaption("Stats - " + characterSource->getName());
    }

    //if theres a source for values
    if( statsSource != NULL ){
        //update values
        labels[0][1]->setCaption( itos(statsSource->getValue(Stats::LVL)) );
        labels[1][1]->setCaption( itos(statsSource->getValue(Stats::HP)) + "/" + itos(statsSource->getValue(Stats::MAX_HP)) );
        labels[2][1]->setCaption( itos(statsSource->getValue(Stats::MP)) + "/" + itos(statsSource->getValue(Stats::MAX_MP)) );
        labels[3][1]->setCaption( itos(statsSource->getValue(Stats::EXP)) + "/" + itos(statsSource->getValue(Stats::MAX_EXP)) );
        labels[4][1]->setCaption( itos(statsSource->getValue(Stats::STAM)) + "/" + itos(statsSource->getValue(Stats::MAX_STAM)) );
        labels[5][1]->setCaption( itos(statsSource->getValue(Stats::STR)) );
        labels[6][1]->setCaption( itos(statsSource->getValue(Stats::DEF)) );
        labels[7][1]->setCaption( itos(statsSource->getValue(Stats::SPD)) );
        labels[8][1]->setCaption( itos(statsSource->getValue(Stats::INT)) );
        labels[9][1]->setCaption( itos(statsSource->getValue(Stats::DEX)) );
        labels[10][1]->setCaption( itos(statsSource->getValue(Stats::CHAR)) );
        labels[11][1]->setCaption( itos(statsSource->getValue(Stats::RES)) );
        labels[12][1]->setCaption( itos(statsSource->getValue(Stats::CON)) );
        labels[13][1]->setCaption( itos(statsSource->getValue(Stats::LUCK)) );

        //resize labels
        for( unsigned int i = 0; i < 14; ++i ){
            labels[i][0]->adjustSize();
            labels[i][1]->adjustSize();
        }
    }
}
