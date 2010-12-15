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


#include "LayerControl.hpp"

//  LayerControl.cpp

#include "globalFunc.hpp"

LayerControl::LayerControl(){
    init();
}

LayerControl::LayerControl(const std::string& group, bool checked){
    init();
    radioBar->setGroup(group);
    radioBar->setSelected(checked);
}

void LayerControl::init(){
    setOpaque( false );

    //  make container
    radioBar = new RadioBar;
    radioBar->setFocusable( false );

    //set colour
    radioBar->setBaseColor( gcn::Color(255,255,255) );
    radioBar->setBackgroundColor( gcn::Color(20,255,20) );


    //  make widgets
    number = new gcn::Label("0");
    name = new gcn::TextField("New Layer");
    view = new gcn::CheckBox( "", true );
    attributeView = new gcn::CheckBox( "", false );


    //  set sizes of contained widgets
    number->setWidth(20);
    name->setWidth(100);


    //  set focus able
    number->setFocusable( false );

    //  set positions of widgets
    number->setPosition(3,2);
    name->setPosition(number->getWidth()+number->getX()+6,2);
    view->setPosition(name->getWidth()+name->getX()+6,2);
    attributeView->setPosition(view->getWidth()+view->getX()+6,2);

    //  set text alignment
    number->setAlignment(gcn::Graphics::CENTER);


    //  add widgets to the container
    add(radioBar);
    add(number);
    add(name);
    add(view);
    add(attributeView);


    setHeight( 25 );
    setWidth(attributeView->getWidth()+attributeView->getX()+10);
    radioBar->setDimension( getDimension() );
}



LayerControl::~LayerControl(){
    delete number;
    delete name;
    delete view;
    delete attributeView;
}





int LayerControl::getNumber(){
    if( !isNumber(number->getCaption()) ){
        return 0;
    } else {
        return atoi( number->getCaption().c_str() );
    }
}

const std::string& LayerControl::getName(){
    return number->getCaption();
}

bool LayerControl::isView(){ return view->isSelected(); }

bool LayerControl::isViewAttribute(){ return attributeView->isSelected(); }

bool LayerControl::isSelected(){ return radioBar->isSelected(); }







void LayerControl::setNumber(int newNumber){
    number->setCaption( itos(newNumber) );
}

void LayerControl::setName(const std::string& newName){
    name->setText(newName);
}

void LayerControl::setView(bool value){
    view->setSelected(value);
}

void LayerControl::setViewAttribute(bool value){
    attributeView->setSelected(value);
}

void LayerControl::setSelected(bool value){
    radioBar->setSelected(value);
}
