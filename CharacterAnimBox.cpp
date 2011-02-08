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


#include "CharacterAnimBox.hpp"
#include <sstream>
#include "globalFunc.hpp"
#include "NumberField.hpp"
#include "AnimFrameList.hpp"
#include "InfraellyScroller.hpp"
#include "TSprite.hpp"
#include "caches.hpp"
#include "InfraellyImageLoader.hpp"
#include "TSpriteIcon.hpp"
#include "caches.hpp"
#include "Screen.hpp"
#include "InfraellyList.hpp"

#include <utility>


CharacterAnimBox::CharacterAnimBox() :
    mouseX_(0),
    mouseY_(0),
    animOriginX_(0),
    animOriginY_(0),
    editing_(false),
    drawAnim_(false),
    activePart_(HEAD),
    activeDir_(UP),
    defaultFrames_(6),
    //-------------
    filenameLbl(new gcn::Label("Filename: ")),
    filenameFld(new gcn::TextField("data/anim/char/")),
    nameLbl(new gcn::Label("Animation name: ")),
    nameFld(new gcn::TextField("")),
    //--------------
    frameLists(4),
    dirList( new InfraellyList ),
    dirDrpList( new gcn::DropDown(dirList) ),
    frameLstBox(new gcn::ListBox),
    frameScroller(new InfraellyScroller(frameLstBox)),
    listUpBtn(new gcn::Button("^")),
    listDownBtn(new gcn::Button("V")),
    listDelBtn(new gcn::Button("x")),
    listAddBtn(new gcn::Button("New Frame")),
    listDupeBtn(new gcn::Button("Dupe Frame")),
    //--------------
    bodyEditBtns(6),
    bodyToggleBtns(6),
    //--------------
    lockEdit(new gcn::CheckBox("Lock Part",false)),
    angleLbl(new gcn::Label("angle = ")),
    angleFld(new NumberField("")),
    zoomLbl(new gcn::Label("zoom = ")),
    zoomFld(new NumberField("1.00")),
    setRotoZoomBtn(new gcn::Button("Set")),
    //--------------
    animPVBox(new gcn::Container),
    rateLbl(new gcn::Label("Rate: ")),
    rateFld(new NumberField("30")),
    playBtn(new gcn::Button("Play")),
    saveBtn(new gcn::Button("Save")),
    loadBtn(new gcn::Button("Load")),
    //--------------
    infobarLbl(new gcn::Label(""))
{
    //set caption of window
    setCaption("Character Animator");

    //init dir list
    dirList->addElement("Up");
    dirList->addElement("Right");
    dirList->addElement("Down");
    dirList->addElement("Left");
    dirDrpList->setSelected(0);
    activeDir_ = UP;

    // set sizes
    filenameFld->setWidth(150);
    nameFld->setWidth(150);
    angleFld->setWidth(40);
    zoomFld->setWidth(40);
    rateFld->setWidth(40);
    setRotoZoomBtn->setWidth(170);
    frameScroller->setSize(200, 250);
    frameLstBox->setWidth(frameScroller->getWidth()-frameScroller->getScrollbarWidth());
    animPVBox->setSize(400, 250);
    listAddBtn->setWidth(animPVBox->getWidth()/2);
    listAddBtn->setHeight(18);
    listDupeBtn->setWidth( listAddBtn->getWidth() );
    listDupeBtn->setHeight(18);
    dirDrpList->setWidth(100);

    //position stuff
    filenameLbl->setPosition(20, 12);
    filenameFld->setPosition(100, 10);
    nameLbl->setPosition(10, 34);
    nameFld->setPosition(100, 32);
    //--------------
    frameScroller->setPosition(10, 60);
    listUpBtn->setPosition(215, 65);
    listDownBtn->setPosition(215, 100);
    listDelBtn->setPosition(215, 150);
    //--------------
    lockEdit->setPosition(300, 315);
    angleLbl->setPosition(250, 62);
    angleFld->setPosition(290, 60);
    zoomLbl->setPosition(340, 62);
    zoomFld->setPosition(380, 60);
    setRotoZoomBtn->setPosition(250, 90);
    //--------------
    saveBtn->setPosition(300, 20);
    loadBtn->setPosition(350, 20);
    //--------------
    listAddBtn->setPosition(10, 338);
    listDupeBtn->setPosition(10+listAddBtn->getWidth(), 338);
    animPVBox->setPosition(10, 360);
    rateLbl->setPosition(10, 622);
    rateFld->setPosition(40, 620);
    playBtn->setPosition(100, 620);
    dirDrpList->setPosition(260, 620);

    //set colours and frames
    frameScroller->setFrameSize(1);
    playBtn->setFrameSize(0);
    saveBtn->setFrameSize(0);
    loadBtn->setFrameSize(0);
    listUpBtn->setFrameSize(0);
    listDownBtn->setFrameSize(0);
    listDelBtn->setFrameSize(0);
    listAddBtn->setFrameSize(0);
    listAddBtn->setBaseColor(gcn::Color(225, 180, 255));
    listDupeBtn->setFrameSize(0);
    listDupeBtn->setBaseColor(gcn::Color(225, 180, 255));
    setRotoZoomBtn->setFrameSize(0);
    animPVBox->setFrameSize(1);
    animPVBox->setBackgroundColor(gcn::Color(0,0,0));
    animPVBox->setBaseColor(gcn::Color(0,0,0));
    animPVBox->setForegroundColor(gcn::Color(0,0,0));

    playBtn->setFocusable(0);
    saveBtn->setFocusable(0);
    loadBtn->setFocusable(0);
    listUpBtn->setFocusable(0);
    listDownBtn->setFocusable(0);
    listDelBtn->setFocusable(0);
    listAddBtn->setFocusable(0);
    listDupeBtn->setFocusable(0);


    // set the animation center
    animOriginX_ = animPVBox->getX() + (animPVBox->getWidth()*.5f);
    animOriginY_ = animPVBox->getY() + (animPVBox->getHeight()*.5f);


    //initialise body buttons
    for( int i = 0; i<6; ++i ){
        bodyToggleBtns[i] = new gcn::Button;
        bodyToggleBtns[i]->setSize(50,50);
        bodyToggleBtns[i]->setFrameSize(1);
        bodyToggleBtns[i]->setBaseColor(gcn::Color(200, 200, 255));
        bodyToggleBtns[i]->setFocusable(0);

        bodyEditBtns[i] = new TSpriteIcon;
        bodyEditBtns[i]->setFrameSize(1);
        bodyEditBtns[i]->setFocusable(0);
    }

    // make framelists
    for(size_t i = 0; i < frameLists.size(); ++i){
        for(size_t j = 0; j < 6; ++j){
            frameLists[i].push_back(new AnimFrameList);
        }
    }

    //position the toggle buttons
    bodyToggleBtns[HEAD]->setCaption("Head");
    bodyToggleBtns[HEAD]->setPosition(300, 130);
    bodyToggleBtns[BODY]->setCaption("Body");
    bodyToggleBtns[BODY]->setPosition(300, 195);
    bodyToggleBtns[LEFT_HAND]->setCaption("Left Hand");
    bodyToggleBtns[LEFT_HAND]->setPosition(240, 195);
    bodyToggleBtns[RIGHT_HAND]->setCaption("Right Hand");
    bodyToggleBtns[RIGHT_HAND]->setPosition(360, 195);
    bodyToggleBtns[LEFT_FOOT]->setCaption("Left Foot");
    bodyToggleBtns[LEFT_FOOT]->setPosition(260, 255);
    bodyToggleBtns[RIGHT_FOOT]->setCaption("Right Foot");
    bodyToggleBtns[RIGHT_FOOT]->setPosition(340, 255);


    //  load sprites for body parts
    bodyEditBtns[HEAD]->setTSprite( cache::tsprites.loadResource("tsprites/head1.xml") );
    bodyEditBtns[BODY]->setTSprite( cache::tsprites.loadResource("tsprites/bodyM1.xml") );
    bodyEditBtns[LEFT_HAND]->setTSprite( cache::tsprites.loadResource("tsprites/hand1.xml") );
    bodyEditBtns[RIGHT_HAND]->setTSprite( cache::tsprites.loadResource("tsprites/hand1.xml") );
    bodyEditBtns[LEFT_FOOT]->setTSprite( cache::tsprites.loadResource("tsprites/feet1.xml") );
    bodyEditBtns[RIGHT_FOOT]->setTSprite( cache::tsprites.loadResource("tsprites/feet1.xml") );

    //set up default frames
    defaultFrames_[HEAD].setPosition(0, 0);
    defaultFrames_[BODY].setPosition(0, 32);
    defaultFrames_[LEFT_HAND].setPosition(-14, 36);
    defaultFrames_[RIGHT_HAND].setPosition(34, 36);
    defaultFrames_[LEFT_FOOT].setPosition(-4, 64);
    defaultFrames_[RIGHT_FOOT].setPosition(16, 64);


    //populate framelist
    for(int i = 0; i < 4; ++i){
        activeDir_ = static_cast<enum Directions>(i);
        for( int j = 0; j < 6; ++j ){
            activePart_ = static_cast<enum BodyParts>(j);
            addListItem( defaultFrames_[activePart_] );
        }
    }
    activeDir_ = UP;
    activePart_ = HEAD;
    frameLstBox->setListModel( frameLists[activeDir_][activePart_] );

    // add event listeners
    dirDrpList->addSelectionListener(this);
    frameLstBox->addSelectionListener(this);

    //placed after the listeners to trigger the event,
    // which will then position the bodyEditButons
    frameLstBox->setSelected(0);

    //add
    add(filenameLbl);
    add(filenameFld);
    add(nameLbl);
    add(nameFld);
    //--------------
    add(frameScroller);
    add(listUpBtn);
    add(listDownBtn);
    add(listDelBtn);
    add(angleLbl);
    add(angleFld);
    add(zoomLbl);
    add(zoomFld);
    add(setRotoZoomBtn);
    //--------------
    add(lockEdit);
    add(animPVBox);
    //--------------
    add(rateLbl);
    add(rateFld);
    add(playBtn);
    add(saveBtn);
    add(loadBtn);
    add(listAddBtn);
    add(listDupeBtn);
    add(dirDrpList);
    //--------------
    add(infobarLbl);
    //--------------
    for( int i = 0; i<6; ++i ){
        add(bodyToggleBtns[i]);
        add(bodyEditBtns[i]);
    }


    resizeToContent();
    infobarLbl->setSize(getWidth(), 20);
    infobarLbl->setPosition(10,getHeight());
    resizeToContent();
}


CharacterAnimBox::~CharacterAnimBox(){
    //delete
    delete nameLbl;
    delete nameFld;
    delete filenameLbl;
    delete filenameFld;
    //--------------
    delete frameLstBox;
    delete dirList;
    delete dirDrpList;
    delete frameScroller;
    delete listUpBtn;
    delete listDownBtn;
    delete listDelBtn;
    delete listAddBtn;
    delete listDupeBtn;
    //--------------
    delete lockEdit;
    delete angleLbl;
    delete angleFld;
    delete zoomLbl;
    delete zoomFld;
    delete setRotoZoomBtn;
    //--------------
    delete animPVBox;
    delete rateLbl;
    delete rateFld;
    delete playBtn;
    delete saveBtn;
    delete loadBtn;
    //--------------
    delete infobarLbl;

    for( int i = 0; i<6; ++i ){
        delete bodyToggleBtns[i];
        delete bodyEditBtns[i];
    }
    for(size_t i = 0; i < frameLists.size(); ++i){
        for(size_t j = 0; j < frameLists[i].size(); ++j){
            delete frameLists[i][j];
        }
    }
}



void CharacterAnimBox::logic(){
    InfraellyWindow::logic();

    // recolour body part buttons
    for( int i = 0; i<6; ++i ){
        if( i == activePart_ ){
            bodyToggleBtns[i]->setBaseColor(gcn::Color(200, 255, 200));
        } else {
            bodyToggleBtns[i]->setBaseColor(gcn::Color(200, 200, 255));
        }
    }

     //stick body part to mouse
    if( editing_ ){
        TSpriteIcon *activePartIcon_ = NULL;

        switch(activePart_){
            case HEAD:
                activePartIcon_ = bodyEditBtns[HEAD];
                infobarLbl->setCaption("Head");
                break;

            case BODY:
                activePartIcon_ = bodyEditBtns[BODY];
                infobarLbl->setCaption("Body");
                break;

            case LEFT_HAND:
                activePartIcon_ = bodyEditBtns[LEFT_HAND];
                infobarLbl->setCaption("Left Hand");
                break;

            case RIGHT_HAND:
                activePartIcon_ = bodyEditBtns[RIGHT_HAND];
                infobarLbl->setCaption("Right Hand");
                break;

            case LEFT_FOOT:
                activePartIcon_ = bodyEditBtns[LEFT_FOOT];
                infobarLbl->setCaption("Left Foot");
                break;

            case RIGHT_FOOT:
                activePartIcon_ = bodyEditBtns[RIGHT_FOOT];
                infobarLbl->setCaption("Right Foot");
                break;
        }

        if( (mouseX_ > animPVBox->getX()) && (mouseX_ < animPVBox->getX()+animPVBox->getWidth()) &&
            (mouseY_ > animPVBox->getY()+16) && (mouseY_ < animPVBox->getY()+animPVBox->getHeight()+16)   ){
            activePartIcon_->setX(mouseX_-activePartIcon_->getWidth()*.5);
            activePartIcon_->setY(mouseY_-activePartIcon_->getHeight()*.5-16);

            //show xy co ords of part
            infobarLbl->setCaption( infobarLbl->getCaption() + ":     x: "
                                 + itos(activePartIcon_->getX()-animOriginX_) + "     y: "
                                 + itos(activePartIcon_->getY()-animOriginY_) );
        }
    }
}

void CharacterAnimBox::mouseMoved(gcn::MouseEvent& mouseEvent){
    mouseX_ = mouseEvent.getX();
    mouseY_ = mouseEvent.getY();
}

void CharacterAnimBox::valueChanged(const gcn::SelectionEvent& event){
    if( event.getSource() == dirDrpList ){
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );
    }
    if( event.getSource() == dirDrpList || event.getSource() == frameLstBox ){
        activeDir_ = static_cast<enum Directions>( dirDrpList->getSelected() );

        for( size_t i = 0; i < 6; ++i ){
            if( !frameLists[activeDir_][i]->empty() ){
                AnimFrame frame = frameLists[activeDir_][i]->at( frameLstBox->getSelected() );
                bodyEditBtns[i]->setPosition(frame.getX()+animOriginX_, frame.getY()+animOriginY_);
            }
        }
    }
}


void CharacterAnimBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    bool fromBodyPart = false;
    for( int i = 0; i < 6; ++i ){
        if( mouseEvent.getSource() == bodyEditBtns[i] ){
            fromBodyPart = true;
            break;
        }
    }

    if( fromBodyPart ){
        mouseEvent.consume();

        if( editing_ ){
            editListItem();
            editing_ = false;
        } else {
            int selected = frameLstBox->getSelected();
            frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
            frameLstBox->setSelected( selected );

            if( mouseEvent.getSource() == bodyEditBtns[HEAD] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == HEAD ){
                        moveToTop(bodyEditBtns[HEAD]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = HEAD;
                    moveToTop(bodyEditBtns[HEAD]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[BODY] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == BODY ){
                        moveToTop(bodyEditBtns[BODY]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = BODY;
                    moveToTop(bodyEditBtns[BODY]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[LEFT_HAND] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == LEFT_HAND ){
                        moveToTop(bodyEditBtns[LEFT_HAND]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = LEFT_HAND;
                    moveToTop(bodyEditBtns[LEFT_HAND]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[RIGHT_HAND] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == RIGHT_HAND ){
                        moveToTop(bodyEditBtns[RIGHT_HAND]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = RIGHT_HAND;
                    moveToTop(bodyEditBtns[RIGHT_HAND]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[LEFT_FOOT] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == LEFT_FOOT ){
                        moveToTop(bodyEditBtns[LEFT_FOOT]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = LEFT_FOOT;
                    moveToTop(bodyEditBtns[LEFT_FOOT]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[RIGHT_FOOT] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == RIGHT_FOOT ){
                        moveToTop(bodyEditBtns[RIGHT_FOOT]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = RIGHT_FOOT;
                    moveToTop(bodyEditBtns[RIGHT_FOOT]);
                    editing_ = true;
                }
            }
        }
    } else



    //  head
    if( mouseEvent.getSource() == bodyToggleBtns[HEAD] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[HEAD]);
        activePart_ = HEAD;
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );
        lockEdit->setSelected(false);
    } else
    //  Body
    if( mouseEvent.getSource() == bodyToggleBtns[BODY] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[BODY]);
        activePart_ = BODY;
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );

        lockEdit->setSelected(false);
    } else
    //  Left arm
    if( mouseEvent.getSource() == bodyToggleBtns[LEFT_HAND] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[LEFT_HAND]);
        activePart_ = LEFT_HAND;
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );
        lockEdit->setSelected(false);
    } else
    //  right hand
    if( mouseEvent.getSource() == bodyToggleBtns[RIGHT_HAND] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[RIGHT_HAND]);
        activePart_ = RIGHT_HAND;
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );
        lockEdit->setSelected(false);
    } else
    //  LEFT FOOT
    if( mouseEvent.getSource() == bodyToggleBtns[LEFT_FOOT] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[LEFT_FOOT]);
        activePart_ = LEFT_FOOT;
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );
        lockEdit->setSelected(false);
    } else
    //  RIGHT FOOT
    if( mouseEvent.getSource() == bodyToggleBtns[RIGHT_FOOT] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[RIGHT_FOOT]);
        activePart_ = RIGHT_FOOT;
        int selected = frameLstBox->getSelected();
        frameLstBox->setListModel( frameLists[activeDir_][activePart_] );
        frameLstBox->setSelected( selected );
        lockEdit->setSelected(false);
    } else



    //New Frame button
    if( mouseEvent.getSource() == listAddBtn ){
        mouseEvent.consume();
        BodyParts trueActivepart = activePart_;
        Directions trueDir = activeDir_;
        //only want the event to be fired once not 4x6 times
        frameLstBox->removeSelectionListener(this);
        for( int i = 0; i < 4; ++i ){
            activeDir_ = static_cast<Directions>(i);
            for( int j = 0; j < 6; ++j ){
                activePart_ = static_cast<enum BodyParts>(j);
                addListItem( defaultFrames_[activePart_] );
            }
        }
        activePart_ = trueActivepart;
        activeDir_ = trueDir;
        frameLstBox->addSelectionListener(this);
        frameLstBox->setSelected( frameLists[activeDir_][activePart_]->getNumberOfElements()-1 );
    } else
    // dupe frame button
    if( mouseEvent.getSource() == listDupeBtn ){
        mouseEvent.consume();
        BodyParts trueActivepart = activePart_;
        Directions trueDir = activeDir_;
        //only want the event to be fired once not 4x6 times
        frameLstBox->removeSelectionListener(this);
        for( int i = 0; i < 4; ++i ){
            activeDir_ = static_cast<Directions>(i);
            for( int j = 0; j < 6; ++j ){
                activePart_ = static_cast<BodyParts>(j);
                if( activeDir_ == trueDir ){
                    addListItem();
                } else {
                    addListItem( defaultFrames_[activePart_] );
                }
            }
        }
        activePart_ = trueActivepart;
        activeDir_ = trueDir;
        frameLstBox->addSelectionListener(this);
        frameLstBox->setSelected( frameLists[activeDir_][activePart_]->getNumberOfElements()-1 );
    } else
    //List up button
    if( mouseEvent.getSource() == listUpBtn ){
        mouseEvent.consume();
        if( frameLists[activeDir_][activePart_]->getNumberOfElements() > 1 ){
            if( frameLstBox->getSelected() != 0 ){
                frameLists[activeDir_][activePart_]->swap( frameLstBox->getSelected(),
                                frameLstBox->getSelected()-1 );
                frameLstBox->setSelected(frameLstBox->getSelected()-1);
            }
        }
    } else
    //  List down button
    if( mouseEvent.getSource() == listDownBtn ){
        mouseEvent.consume();
        if( frameLists[activeDir_][activePart_]->getNumberOfElements() > 1 ){
            if( frameLstBox->getSelected() < frameLists[activeDir_][activePart_]->getNumberOfElements()-1 ){
                frameLists[activeDir_][activePart_]->swap( frameLstBox->getSelected(),
                                frameLstBox->getSelected()+1 );
                frameLstBox->setSelected(frameLstBox->getSelected()+1);
            }
        }
    } else
    //  List's delete button
    if( mouseEvent.getSource() == listDelBtn ){
        mouseEvent.consume();
        BodyParts trueActivepart = activePart_;
        Directions trueDir = activeDir_;
        for( int i = 0; i < 4; ++i ){
            activeDir_ = static_cast<Directions>(i);
            for( int j = 0; j < 6; ++j ){
                activePart_ = static_cast<BodyParts>(j);
                if( frameLists[activeDir_][activePart_]->getNumberOfElements() > 1 ){
                    int selected = frameLstBox->getSelected();
                    if( (selected >= 0) && (selected < frameLists[activeDir_][activePart_]->getNumberOfElements()) ){
                        frameLists[activeDir_][activePart_]->removeElementAt( selected );
                        if( selected > frameLists[activeDir_][activePart_]->getNumberOfElements()-1 ){
                            //adjusted selected
                            frameLstBox->setSelected(frameLists[activeDir_][activePart_]->getNumberOfElements()-1);
                            //adjust scroller
                            frameLstBox->logic();
                            frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                                            frameScroller->getVerticalMaxScroll());
                        }
                    }
                }
            }
            activePart_ = trueActivepart;
            activeDir_ = trueDir;
        }
    } else
    //  set rotozoom
    if( mouseEvent.getSource() == setRotoZoomBtn ){
        mouseEvent.consume();
        if( frameLists[activeDir_][activePart_]->getNumberOfElements() > 0 ){
            int selected = frameLstBox->getSelected();
            if( (selected >= 0) && (selected < frameLists[activeDir_][activePart_]->getNumberOfElements()) ){
                frameLists[activeDir_][activePart_]->at( selected ).setAngle( atoi(angleFld->getText()) );
                frameLists[activeDir_][activePart_]->at( selected ).setZoom( atoi(zoomFld->getText()) );
            }
        }
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
    //  play
    if( mouseEvent.getSource() == playBtn ){
        mouseEvent.consume();
        if( !drawAnim_ ){
            if( !(save("breath.tmp", 1) && load("breath.tmp", 1)) ){
                setCaption("Character Animator - Preview Fail");
            } else {
                for( int i = 0; i < 6; ++i ){
                    bodyEditBtns[i]->setVisible(0);
                }
                drawAnim_ = true;
                playBtn->setCaption("Stop");
            }
        } else {
            for( int i = 0; i < 6; ++i ){
                bodyEditBtns[i]->setVisible(1);
            }
            drawAnim_ = false;
            playBtn->setCaption("Play");
        }
    }
}


void CharacterAnimBox::addListItem(){
    int x = bodyEditBtns[activePart_]->getX()-animOriginX_;
    int y = bodyEditBtns[activePart_]->getY()-animOriginY_;
    int angle = atoi( angleFld->getText() );
    float zoom = atof( zoomFld->getText() );

    AnimFrame newFrame;
    newFrame.setX( x );
    newFrame.setY( y );
    newFrame.setAngle( angle );
    newFrame.setZoom( zoom );

    addListItem(newFrame);
}

void CharacterAnimBox::addListItem(const AnimFrame& newFrame){
    std::ostringstream name;
    name << "x: " << newFrame.getX()        << "         "
         << "y: " << newFrame.getY()        << "         "
         << "a: " << newFrame.getAngle()    << "         "
         << "z: " << newFrame.getZoom();

    frameLists[activeDir_][activePart_]->addElement(name.str(), newFrame);
    frameLstBox->setSelected(frameLists[activeDir_][activePart_]->getNumberOfElements()-1);
    //run logic to have it resize itself =]
    frameLstBox->logic();
    //scroll to bottom
    frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                    frameScroller->getVerticalMaxScroll());
}


void CharacterAnimBox::editListItem(){
    if( frameLists[activeDir_][activePart_]->empty() ){
        addListItem();
        return;
    }

    int x = bodyEditBtns[activePart_]->getX()-animOriginX_;
    int y = bodyEditBtns[activePart_]->getY()-animOriginY_;
    int angle = atoi( angleFld->getText() );
    float zoom = atof( zoomFld->getText() );

    AnimFrame newFrame;
    newFrame.setX( x );
    newFrame.setY( y );
    newFrame.setAngle( angle );
    newFrame.setZoom( zoom );

    std::ostringstream name;
    name << "x: " << x << "         y: " << y << "         a: " << angle << "         z: " << zoom;

    std::pair<std::string, AnimFrame> newListItem(name.str(), newFrame);

    frameLists[activeDir_][activePart_]->getPair(frameLstBox->getSelected()) = newListItem;
    //run logic to have it resize itself =]
    frameLstBox->logic();
    //scroll to bottom
    frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                    frameScroller->getVerticalMaxScroll());
}


bool CharacterAnimBox::save(const std::string &filename, bool discrete){
    std::string fixedFn = correctFilepath(filename);

    //clear out the old animation
    anim_.clear();

    for( int i = 0; i < anim_.anims.size(); i++ ){
        for( int j = 0; j < anim_.anims[i].size(); ++j ){
            Animation &anim = anim_.anims[i][j];

            if( !discrete ){
                anim.setName( fixedFn );
            }

            anim.setFPS( atoi(rateFld->getText()) );

            //fill up with new animation frames
            for( int k = 0; k < frameLists[i][j]->getNumberOfElements(); ++k ){
                anim.addFrame( frameLists[i][j]->at(k) );
            }
        }
    }

    //save the animation to file.
    if(  anim_.save( fixedFn )  ){
        if( !discrete ){
            setCaption("Character Animator - Save Success");
        }
        return true;
    } else {
        if( !discrete ){
            setCaption("Character Animator - Save Fail");
        }
        return false;
    }

    return true;
}

bool CharacterAnimBox::load(const std::string &filename, bool discrete){
    std::string fixedFn = correctFilepath(filename);

    //clear out the old animation
    anim_.clear();

    //load the animation to file.
    if( anim_.load(fixedFn) ){
        if( !discrete ){
            setCaption("Character Animator - Load Success");
        }
    } else {
        if( !discrete ){
            setCaption("Character Animator - Load Fail");
        }
        return 0;
    }

    //display load anim name
    if( !discrete ){
        filenameFld->setText( fixedFn );
    }


    //fill up with loaded animations frames
    for( int i = 0; i < anim_.anims.size(); ++i ){
        for( int j = 0; j < anim_.anims[i].size(); ++j ){
            AnimFrameList* frameLst = frameLists[i][j];
            Animation &anim = anim_.anims[i][j];

            frameLst->clear();
            for( int k = 0; k < anim.getFrameCount(); ++k ){
                std::ostringstream name;
                AnimFrame *frame = anim.getFrame(k);

                name << "x: " << frame->getX() << "         y: " << frame->getY()
                     << "         a: " << frame->getAngle() << "         z: " << frame->getZoom();

                frameLst->addElement(name.str(), *frame);
            }
        }
    }

    // refresh frame list box
    frameLstBox->setListModel( frameLists[activeDir_][HEAD] );
    frameLstBox->setSelected( 0 );
    //run logic to have it resize itself =]
    frameLstBox->logic();
    //scroll to bottom
    frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                            frameScroller->getVerticalMaxScroll());

    rateFld->setText( itos(anim_.anims[0][0].getFPS()) );

    return true;
}

void CharacterAnimBox::draw(gcn::Graphics *graphics){
    InfraellyWindow::draw(graphics);

    if( drawAnim_ ){
        anim_.draw( Screen::getSurface(), activeDir_, getX()+animOriginX_,
                    getY()+getTitleBarHeight()+animOriginY_ );
    }

    gcn::Rectangle d = animPVBox->getDimension();
    d.y += getTitleBarHeight();
    d.x += animPVBox->getFrameSize();
    graphics->pushClipArea( d );
    graphics->setColor( gcn::Color( 60, 60, 60 ) );
    //draw vertical line
    graphics->drawLine(  d.width/2, 0,
                         d.width/2, d.height  );
    //draw horizontal line
    graphics->drawLine(  0,         d.height/2,
                         d.width,   d.height/2 );
    graphics->popClipArea();
}
