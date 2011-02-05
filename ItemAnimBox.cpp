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


#include "ItemAnimBox.hpp"
#include <sstream>
#include "globalFunc.hpp"
#include "NumberField.hpp"
#include "KeyPointList.hpp"
#include "SnapPointList.hpp"
#include "AnimFrameList.hpp"
#include "InfraellyScroller.hpp"
#include "AnimPreviewIcon.hpp"


ItemAnimBox::ItemAnimBox() :
    keyPointLst(new KeyPointList),
    snapPointLst(new SnapPointList),
    frameLst(new AnimFrameList),
    //--------------
    filenameLbl(new gcn::Label("Filename: ")),
    filenameFld(new gcn::TextField("data/anim/item/")),
    nameLbl(new gcn::Label("Animation Name: ")),
    nameFld(new gcn::TextField("")),
    snapLbl(new gcn::Label("Snap To:")),
    snapDrpLst(new gcn::DropDown(snapPointLst)),
    originLbl(new gcn::Label("Origin: ")),
    originDrpLst(new gcn::DropDown(keyPointLst)),
    alignLbl(new gcn::Label("Align: ")),
    alignDrpLst(new gcn::DropDown(keyPointLst)),
    //--------------
    frameLstBox(new gcn::ListBox(frameLst)),
    frameScroller(new InfraellyScroller(frameLstBox)),
    listUpBtn(new gcn::Button("^")),
    listDownBtn(new gcn::Button("V")),
    listDelBtn(new gcn::Button("x")),
    listAddBtn(new gcn::Button("Add")),
    xLbl(new gcn::Label("x = ")),
    xFld(new NumberField("")),
    yLbl(new gcn::Label("y = ")),
    yFld(new NumberField("")),
    angleLbl(new gcn::Label("angle = ")),
    angleFld(new NumberField("")),
    zoomLbl(new gcn::Label("zoom = ")),
    zoomFld(new NumberField("1.00")),
    //--------------
    animPVBox(new AnimPreviewIcon),
    rateLbl(new gcn::Label("Rate: ")),
    rateFld(new NumberField("30")),
    playBtn(new gcn::Button("Play")),
    saveBtn(new gcn::Button("Save")),
    loadBtn(new gcn::Button("Load"))
{
    //set caption of window
    setCaption("Item Animator");

    //set up animation object
    TSprite dummyImage;
    dummyImage.loadXml("tsprites/animatorItem.xml");
    anim.setImage( dummyImage );
    animPVBox->setAnim(&anim);
    anim.setType(Animation::ITEM);

    // set sizes
    filenameFld->setWidth(150);
    nameFld->setWidth(150);
    xFld->setWidth(40);
    yFld->setWidth(40);
    angleFld->setWidth(40);
    zoomFld->setWidth(40);
    rateFld->setWidth(40);
    listAddBtn->setSize(50, 50);
    frameScroller->setSize(200, 100);
    frameLstBox->setWidth(frameScroller->getWidth()-frameScroller->getScrollbarWidth());
    animPVBox->setSize(242, 150);



    //position stuff
    filenameLbl->setPosition(20, 12);
    filenameFld->setPosition(100, 10);
    nameLbl->setPosition(10, 34);
    nameFld->setPosition(100, 32);
    snapLbl->setPosition(54, 54);
    snapDrpLst->setPosition(100, 54);
    originLbl->setPosition(64, 76);
    originDrpLst->setPosition(100, 76);
    alignLbl->setPosition(67, 98);
    alignDrpLst->setPosition(100, 98);
    //--------------
    frameScroller->setPosition(10, 130);
    listUpBtn->setPosition(215, 130);
    listDownBtn->setPosition(215, 150);
    listDelBtn->setPosition(215, 180);
    listAddBtn->setPosition(200, 240);
    xLbl->setPosition(30, 242);
    xFld->setPosition(60, 240);
    yLbl->setPosition(120, 242);
    yFld->setPosition(150, 240);
    angleLbl->setPosition(10, 272);
    angleFld->setPosition(60, 270);
    zoomLbl->setPosition(115, 272);
    zoomFld->setPosition(150, 270);
    //--------------
    animPVBox->setPosition(10, 310);
    rateLbl->setPosition(10, 472);
    rateFld->setPosition(40, 470);
    playBtn->setPosition(10, 500);
    saveBtn->setPosition(50, 500);
    loadBtn->setPosition(148, 500);


    //set colours
    frameScroller->setFrameSize(1);
    playBtn->setFrameSize(0);
    saveBtn->setFrameSize(0);
    loadBtn->setFrameSize(0);
    listUpBtn->setFrameSize(0);
    listDownBtn->setFrameSize(0);
    listDelBtn->setFrameSize(0);
    listAddBtn->setFrameSize(0);
    animPVBox->setFrameSize(1);


    //add
    add(filenameLbl);
    add(filenameFld);
    add(nameLbl);
    add(nameFld);
    add(snapLbl);
    add(snapDrpLst);
    add(originLbl);
    add(originDrpLst);
    add(alignLbl);
    add(alignDrpLst);
    //--------------
    add(frameScroller);
    add(listUpBtn);
    add(listDownBtn);
    add(listDelBtn);
    add(xLbl);
    add(xFld);
    add(yLbl);
    add(yFld);
    add(angleLbl);
    add(angleFld);
    add(zoomLbl);
    add(zoomFld);
    add(listAddBtn);

    //--------------
    add(animPVBox);
    add(rateLbl);
    add(rateFld);
    add(playBtn);
    add(saveBtn);
    add(loadBtn);

    resizeToContent();
}

ItemAnimBox::~ItemAnimBox(){
    //delete
    delete keyPointLst;
    delete snapPointLst;
    delete frameLst;
    //--------------
    delete nameLbl;
    delete nameFld;
    delete filenameLbl;
    delete filenameFld;
    delete snapLbl;
    delete snapDrpLst;
    delete originLbl;
    delete originDrpLst;
    delete alignLbl;
    delete alignDrpLst;
    //--------------
    delete frameLstBox;
    delete frameScroller;
    delete listUpBtn;
    delete listDownBtn;
    delete listDelBtn;
    delete listAddBtn;
    delete xLbl;
    delete xFld;
    delete yLbl;
    delete yFld;
    delete angleLbl;
    delete angleFld;
    delete zoomLbl;
    delete zoomFld;
    //--------------
    delete animPVBox;
    delete rateLbl;
    delete rateFld;
    delete playBtn;
    delete saveBtn;
    delete loadBtn;
}



bool ItemAnimBox::save(const std::string &filename, bool discrete){
    //clear out the old animation
    anim.clear();

    if( !discrete ){
        anim.setName( correctFilepath(filename) );
    }
    anim.setSnapPoint( snapPointLst->at(snapDrpLst->getSelected()) );
    anim.setOrigin( keyPointLst->at(originDrpLst->getSelected()) );
    anim.setAlignment( keyPointLst->at(alignDrpLst->getSelected()) );
    anim.setFPS( atoi(rateFld->getText()) );

    //fill up with new animation frames
    for( int i = 0; i < frameLst->getNumberOfElements(); ++i ){
        anim.addFrame( frameLst->at(i) );
    }

    //save the animation to file.
    if(  anim.save( correctFilepath(filename) )  ){
        if( !discrete ){
            setCaption("Item Animator - Save Success");
        }
        return true;
    } else {
        if( !discrete ){
            setCaption("Item Animator - Save Fail");
        }
        return false;
    }
}


bool ItemAnimBox::load(const std::string &filename, bool discrete){
    std::string fixedFn = correctFilepath(filename);
    //clear out the old animation
    anim.clear();

    //load the animation to file.
    if( anim.load(fixedFn) ){
        if( !discrete ){
            setCaption("Item Animator - Load Success");
        }
    } else {
        if( !discrete ){
            setCaption("Item Animator - Load Fail");
        }
        return 0;
    }

    //display load anim name
    if( !discrete ){
        filenameFld->setText( fixedFn );
    }

    //display loaded snap point
    for( int i = 0; i < snapPointLst->getNumberOfElements(); ++i ){
        if( anim.getSnapPoint() == snapPointLst->at(i) ){
            snapDrpLst->setSelected(i);
        }
    }

    //display loaded orign/align
    for( int i = 0; i < keyPointLst->getNumberOfElements(); ++i ){
        if( anim.getOrigin() == keyPointLst->at(i) ){
            originDrpLst->setSelected(i);
        }
        if( anim.getAlignment() == keyPointLst->at(i) ){
            alignDrpLst->setSelected(i);
        }
    }

    //fill up with loaded animation frames
    frameLst->clear();

    for( int i = 0; i < anim.getFrameCount(); ++i ){
        std::ostringstream name;
        AnimFrame *frame = anim.getFrame(i);

        name << "x: " << frame->getX() << "         y: " << frame->getY()
             << "         a: " << frame->getAngle() << "         z: " << frame->getZoom();

        frameLst->addElement(name.str(), *frame);
    }
    frameLstBox->setSelected(frameLst->getNumberOfElements()-1);
        //run logic to have it resize itself =]
        frameLstBox->logic();
        //scroll to bottom
        frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                        frameScroller->getVerticalMaxScroll());

    rateFld->setText( itos(anim.getFPS()) );

    return true;
}



void ItemAnimBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    //List up button
    if( mouseEvent.getSource() == listUpBtn ){
        mouseEvent.consume();
        if( frameLst->getNumberOfElements() > 1 ){
            if( frameLstBox->getSelected() != 0 ){
                frameLst->swap( frameLstBox->getSelected(),
                                frameLstBox->getSelected()-1 );
                frameLstBox->setSelected(frameLstBox->getSelected()-1);
            }
        }
    } else
    //  List down button
    if( mouseEvent.getSource() == listDownBtn ){
        mouseEvent.consume();
        if( frameLst->getNumberOfElements() > 1 ){
            if( frameLstBox->getSelected() < frameLst->getNumberOfElements()-1 ){
                frameLst->swap( frameLstBox->getSelected(),
                                frameLstBox->getSelected()+1 );
                frameLstBox->setSelected(frameLstBox->getSelected()+1);
            }
        }
    } else
    //  List's delete button
    if( mouseEvent.getSource() == listDelBtn ){
        mouseEvent.consume();
        if( frameLst->getNumberOfElements() > 0 ){
            int selected = frameLstBox->getSelected();
            if( (selected >= 0) && (selected < frameLst->getNumberOfElements()) ){
                frameLst->removeElementAt( selected );
                if( selected > frameLst->getNumberOfElements()-1 ){
                    //adjusted selected
                    frameLstBox->setSelected(frameLst->getNumberOfElements()-1);
                    //adjust scroller
                    frameLstBox->logic();
                    frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                                    frameScroller->getVerticalMaxScroll());
                }
            }
        }
    } else
    //  List add button
    if( mouseEvent.getSource() == listAddBtn ){
        mouseEvent.consume();
        int x = atoi( xFld->getText() );
        int y = atoi( yFld->getText() );
        int angle = atoi( angleFld->getText() );
        float zoom = atof( zoomFld->getText() );

        AnimFrame newFrame;
        newFrame.setX( x );
        newFrame.setY( y );
        newFrame.setAngle( angle );
        newFrame.setZoom( zoom );

        std::ostringstream name;
        name << "x: " << x << "         y: " << y << "         a: " << angle << "         z: " << zoom;

        frameLst->addElement(name.str(), newFrame);
        frameLstBox->setSelected(frameLst->getNumberOfElements()-1);
        //run logic to have it resize itself =]
        frameLstBox->logic();
        //scroll to bottom
        frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                        frameScroller->getVerticalMaxScroll());

        xFld->setText("");
        yFld->setText("");
    } else
    //  Save Button
    if( mouseEvent.getSource() == saveBtn ){
        mouseEvent.consume();
        save( filenameFld->getText() );
    } else
    //  Load Button
    if( mouseEvent.getSource() == loadBtn ){
        mouseEvent.consume();
        load( filenameFld->getText() );
    } else
    //  Play Button
    if( mouseEvent.getSource() == playBtn ){
        mouseEvent.consume();
        if( !(save("breath.tmp", 1) && load("breath.tmp", 1)) ){
            setCaption("Item Animator - Preview Fail");
        }
    }
}
