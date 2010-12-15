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


#include "MakeTSpriteBox.hpp"
#include "globalFunc.hpp"
#include "NumberField.hpp"
#include "InfraellyScroller.hpp"
#include "TilesetIcon.hpp"
#include "TilesetList.hpp"
#include "TSpriteIcon.hpp"
#include "caches.hpp"


MakeTSpriteBox::MakeTSpriteBox() :
    filenameLbl(new gcn::Label("Filename: ")),
    filenameFld(new gcn::TextField("data/tsprites/")),
    //---------------------------
    numImagesLbl(new gcn::Label("Number of images: ")),
    numImagesFld(new NumberField("1")),
    perRowLbl(new gcn::Label("Images per row: ")),
    perRowFld(new NumberField("1")),
    xOffsetLbl(new gcn::Label("X-Offset (tiles): ")),
    xOffsetFld(new NumberField("0")),
    yOffsetLbl(new gcn::Label("Y-Offset (tiles): ")),
    yOffsetFld(new NumberField("0")),
    fpsLbl(new gcn::Label("FPS: ")),
    fpsFld(new NumberField("5")),
    //---------------------------
    spriteIcon(new TSpriteIcon),
    //---------------------------
    srcTsLbl(new gcn::Label("Source: ")),
    tilesetList(new TilesetList),
    tilesetDropDown(new gcn::DropDown(tilesetList)),
    //--------------------------
    tsIcon(new TilesetIcon),
    tsScroller(new InfraellyScroller(tsIcon))
{
    doneButton->setCaption("Save");

    //set caption of window
    setCaption("Make Sprite from Tileset");

    //add self as key listeners
    filenameFld->addKeyListener(this);
    tilesetDropDown->addSelectionListener(this);
    numImagesFld->addKeyListener(this);
    perRowFld->addKeyListener(this);
    xOffsetFld->addKeyListener(this);
    yOffsetFld->addKeyListener(this);
    fpsFld->addKeyListener(this);


    //set other properties
    spriteIcon->setTSprite(&sprite);


    // set sizes
    filenameFld->setWidth(100);
    numImagesFld->setWidth(100);
    perRowFld->setWidth(100);
    xOffsetFld->setWidth(100);
    yOffsetFld->setWidth(100);
    fpsFld->setWidth(100);
    spriteIcon->setSize(90, 140);
    tilesetDropDown->setWidth(200);
    tsScroller->setSize(300,200);


    //position stuff y+20
    filenameLbl->setPosition(10, 12);
    filenameFld->setPosition(110, 10);
    numImagesLbl->setPosition(10, 52);
    numImagesFld->setPosition(110, 50);
    perRowLbl->setPosition(10, 72);
    perRowFld->setPosition(110, 70);
    xOffsetLbl->setPosition(10, 92);
    xOffsetFld->setPosition(110, 90);
    yOffsetLbl->setPosition(10, 112);
    yOffsetFld->setPosition(110, 110);
    fpsLbl->setPosition(10, 132);
    fpsFld->setPosition(110, 130);
    spriteIcon->setPosition(220, 10);
    srcTsLbl->setPosition(10, 160);
    tilesetDropDown->setPosition(110, 160);
    tsScroller->setPosition(10, 180);


    //set colours
    tsScroller->setFrameSize(1);


    //add
    add(filenameLbl);
    add(filenameFld);
    add(numImagesLbl);
    add(numImagesFld);
    add(perRowLbl);
    add(perRowFld);
    add(xOffsetLbl);
    add(xOffsetFld);
    add(yOffsetLbl);
    add(yOffsetFld);
    add(fpsLbl);
    add(fpsFld);
    add(spriteIcon);
    add(srcTsLbl);
    add(tilesetDropDown);
    add(tsScroller);


    resizeToContent();
    setWidth(getWidth()+10);
    positionDefaultBtns();
}


MakeTSpriteBox::~MakeTSpriteBox(){
    // Delete                  //   Assign Null
    delete filenameLbl;        filenameLbl = NULL;
    delete filenameFld;        filenameFld = NULL;
    delete numImagesLbl;       numImagesLbl = NULL;
    delete numImagesFld;       numImagesFld = NULL;
    delete perRowLbl;          perRowLbl = NULL;
    delete perRowFld;          perRowFld = NULL;
    delete xOffsetLbl;         xOffsetLbl = NULL;
    delete xOffsetFld;         xOffsetFld = NULL;
    delete yOffsetLbl;         yOffsetLbl = NULL;
    delete yOffsetFld;         yOffsetFld = NULL;
    delete fpsLbl;             fpsLbl = NULL;
    delete fpsFld;             fpsFld = NULL;
    delete spriteIcon;         spriteIcon = NULL;
    delete srcTsLbl;           srcTsLbl = NULL;
    delete tilesetList;        tilesetList = NULL;
    delete tilesetDropDown;    tilesetDropDown = NULL;
    delete tsScroller;         tsScroller = NULL;
}


TSprite MakeTSpriteBox::getSprite()const{ return sprite; }


void MakeTSpriteBox::draw(gcn::Graphics* graphics){
    InfoForm::draw(graphics);
}

void MakeTSpriteBox::keyPressed(gcn::KeyEvent& keyEvent){
    if( (keyEvent.getSource() == numImagesFld)  ||
        (keyEvent.getSource() == perRowFld)     ||
        (keyEvent.getSource() == xOffsetFld)    ||
        (keyEvent.getSource() == yOffsetFld)    ||
        (keyEvent.getSource() == fpsFld)  ){
        updatePreview();
    }
}


void MakeTSpriteBox::valueChanged(const gcn::SelectionEvent& event){
    //if event from drop down
    if( event.getSource() == tilesetDropDown ){
        //change the tileset preview;
        tsIcon->setTileset( tilesetList->at( tilesetDropDown->getSelected() ) );
        //change tsprite preview
        updatePreview();
    }
}

void MakeTSpriteBox::updatePreview(){
   sprite.load( tilesetList->at( tilesetDropDown->getSelected() ),
                 atoi(numImagesFld->getText()),
                 atoi(perRowFld->getText()),
                 atoi(xOffsetFld->getText()),
                 atoi(yOffsetFld->getText()),
                 atoi(fpsFld->getText()) );
    spriteIcon->setTSprite(&sprite);
}

void MakeTSpriteBox::doneAction(){
    if( sprite.getSource() != NULL ){
        if( cache::tsprites.get( sprite.getFilename() ) == NULL ){
            //write to file
            if(sprite.saveXml(correctFilepath(filenameFld->getText()))){
                // add tsprite to cache
                TSprite *newTSpr = new TSprite(sprite);
                newTSpr->setFilename( sprite.getFilename() );
                cache::tsprites.addPointer( newTSpr );
                setVisible(false);
            } else {
                //"!@#" guiAlert();
                setVisible(true);
            }
        }
    }
}
