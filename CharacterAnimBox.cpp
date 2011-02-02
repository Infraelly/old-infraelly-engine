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
    activePart_(CharAnimation::HEAD),
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
    animOriginX_ = (animPVBox->getWidth()*.5f);
    animOriginY_ = animPVBox->getY() + (animPVBox->getHeight()*.5f);


    //initialise body buttons
    for( int i = 0; i<6; ++i ){
        bodyToggleBtns[i] = new gcn::Button;
        bodyToggleBtns[i]->setSize(50,50);
        bodyToggleBtns[i]->setFrameSize(1);
        bodyToggleBtns[i]->setBaseColor(gcn::Color(200, 200, 255));
        bodyToggleBtns[i]->setFocusable(0);

        bodyEditBtns[i] = new TSpriteIcon;
        bodyEditBtns[i]->setFrameSize(0);
        bodyEditBtns[i]->setFocusable(0);
    }

    // make framelists
    for(size_t i = 0; i < frameLists.size(); ++i){
        for(size_t j = 0; j < 6; ++j){
            frameLists[i].push_back(new AnimFrameList);
        }
    }

    //position the toggle buttons
    bodyToggleBtns[CharAnimation::HEAD]->setCaption("Head");
    bodyToggleBtns[CharAnimation::HEAD]->setPosition(300, 130);
    bodyToggleBtns[CharAnimation::BODY]->setCaption("Body");
    bodyToggleBtns[CharAnimation::BODY]->setPosition(300, 130);
    bodyToggleBtns[CharAnimation::LEFT_HAND]->setCaption("Left Hand");
    bodyToggleBtns[CharAnimation::LEFT_HAND]->setPosition(240, 195);
    bodyToggleBtns[CharAnimation::RIGHT_HAND]->setCaption("Right Hand");
    bodyToggleBtns[CharAnimation::RIGHT_HAND]->setPosition(360, 195);
    bodyToggleBtns[CharAnimation::LEFT_FOOT]->setCaption("Left Foot");
    bodyToggleBtns[CharAnimation::LEFT_FOOT]->setPosition(260, 255);
    bodyToggleBtns[CharAnimation::RIGHT_FOOT]->setCaption("Right Foot");
    bodyToggleBtns[CharAnimation::RIGHT_FOOT]->setPosition(340, 255);


    //  load sprites for body parts
    bodyEditBtns[CharAnimation::HEAD]->setTSprite( cache::tsprites.loadResource("tsprites/head1.xml") );
    bodyEditBtns[CharAnimation::BODY]->setTSprite( cache::tsprites.loadResource("tsprites/bodyM1.xml") );
    bodyEditBtns[CharAnimation::LEFT_HAND]->setTSprite( cache::tsprites.loadResource("tsprites/hand1.xml") );
    bodyEditBtns[CharAnimation::RIGHT_HAND]->setTSprite( cache::tsprites.loadResource("tsprites/hand1.xml") );
    bodyEditBtns[CharAnimation::LEFT_FOOT]->setTSprite( cache::tsprites.loadResource("tsprites/feet1.xml") );
    bodyEditBtns[CharAnimation::RIGHT_FOOT]->setTSprite( cache::tsprites.loadResource("tsprites/feet1.xml") );

    //set up default frames
    defaultFrames_[CharAnimation::HEAD].setPosition(-16, -48);
    defaultFrames_[CharAnimation::BODY].setPosition(-16, -16);
    defaultFrames_[CharAnimation::LEFT_HAND].setPosition(-30, -12);
    defaultFrames_[CharAnimation::RIGHT_HAND].setPosition(16, -12);
    defaultFrames_[CharAnimation::LEFT_FOOT].setPosition(-20, 14);
    defaultFrames_[CharAnimation::RIGHT_FOOT].setPosition(2, 14);


    //populate framelist
    for(int i = 0; i < 4; ++i){
        activeDir_ = static_cast<enum Directions>(i);
        for( int j = 0; j < 6; ++j ){
            activePart_ = static_cast<CharAnimation::BodyParts>(j);
            addListItem( defaultFrames_[activePart_] );
        }
    }
    activeDir_ = UP;
    activePart_ = CharAnimation::HEAD;
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
    //delete                    //NULL
    delete nameLbl;            nameLbl = NULL;
    delete nameFld;            nameFld = NULL;
    delete filenameLbl;        filenameLbl = NULL;
    delete filenameFld;        filenameFld = NULL;
    //--------------
    delete frameLstBox;        frameLstBox = NULL;
    delete dirList;            dirList = NULL;
    delete dirDrpList;         dirDrpList = NULL;
    delete frameScroller;      frameScroller = NULL;
    delete listUpBtn;          listUpBtn = NULL;
    delete listDownBtn;        listDownBtn = NULL;
    delete listDelBtn;         listDelBtn = NULL;
    delete listAddBtn;         listAddBtn = NULL;
    delete listDupeBtn;        listDupeBtn = NULL;
    //--------------
    delete lockEdit;           lockEdit = NULL;
    delete angleLbl;           angleLbl = NULL;
    delete angleFld;           angleFld = NULL;
    delete zoomLbl;            zoomLbl = NULL;
    delete zoomFld;            zoomFld = NULL;
    delete setRotoZoomBtn;     setRotoZoomBtn = NULL;
    delete zoomFld;            zoomFld = NULL;
    //--------------
    delete animPVBox;          animPVBox = NULL;
    delete rateLbl;            rateLbl = NULL;
    delete rateFld;            rateFld = NULL;
    delete playBtn;            playBtn = NULL;
    delete saveBtn;            saveBtn = NULL;
    delete loadBtn;            loadBtn = NULL;
    //--------------
    delete infobarLbl;         infobarLbl = NULL;

    for( int i = 0; i<6; ++i ){
        delete bodyToggleBtns[i];
        delete bodyEditBtns[i];
        bodyToggleBtns[i] = NULL;
        bodyEditBtns[i] = NULL;

    }
    for(size_t i = 0; i < frameLists.size(); ++i){
        for(size_t j = 0; j < frameLists[i].size(); ++j){
            delete frameLists[i][j];
            frameLists[i][j] = NULL;
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
            case CharAnimation::HEAD:
                activePartIcon_ = bodyEditBtns[CharAnimation::HEAD];
                infobarLbl->setCaption("Head");
                break;

            case CharAnimation::BODY:
                activePartIcon_ = bodyEditBtns[CharAnimation::BODY];
                infobarLbl->setCaption("Body");
                break;

            case CharAnimation::LEFT_HAND:
                activePartIcon_ = bodyEditBtns[CharAnimation::LEFT_HAND];
                infobarLbl->setCaption("Left Hand");
                break;

            case CharAnimation::RIGHT_HAND:
                activePartIcon_ = bodyEditBtns[CharAnimation::RIGHT_HAND];
                infobarLbl->setCaption("Right Hand");
                break;

            case CharAnimation::LEFT_FOOT:
                activePartIcon_ = bodyEditBtns[CharAnimation::LEFT_FOOT];
                infobarLbl->setCaption("Left Foot");
                break;

            case CharAnimation::RIGHT_FOOT:
                activePartIcon_ = bodyEditBtns[CharAnimation::RIGHT_FOOT];
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
            if( mouseEvent.getSource() == bodyEditBtns[CharAnimation::HEAD] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == CharAnimation::HEAD ){
                        moveToTop(bodyEditBtns[CharAnimation::HEAD]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = CharAnimation::HEAD;
                    moveToTop(bodyEditBtns[CharAnimation::HEAD]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[CharAnimation::BODY] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == CharAnimation::BODY ){
                        moveToTop(bodyEditBtns[CharAnimation::BODY]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = CharAnimation::BODY;
                    moveToTop(bodyEditBtns[CharAnimation::BODY]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[CharAnimation::LEFT_HAND] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == CharAnimation::LEFT_HAND ){
                        moveToTop(bodyEditBtns[CharAnimation::LEFT_HAND]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = CharAnimation::LEFT_HAND;
                    moveToTop(bodyEditBtns[CharAnimation::LEFT_HAND]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[CharAnimation::RIGHT_HAND] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == CharAnimation::RIGHT_HAND ){
                        moveToTop(bodyEditBtns[CharAnimation::RIGHT_HAND]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = CharAnimation::RIGHT_HAND;
                    moveToTop(bodyEditBtns[CharAnimation::RIGHT_HAND]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[CharAnimation::LEFT_FOOT] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == CharAnimation::LEFT_FOOT ){
                        moveToTop(bodyEditBtns[CharAnimation::LEFT_FOOT]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = CharAnimation::LEFT_FOOT;
                    moveToTop(bodyEditBtns[CharAnimation::LEFT_FOOT]);
                    editing_ = true;
                }
            }
            if( mouseEvent.getSource() == bodyEditBtns[CharAnimation::RIGHT_FOOT] ){
                if( lockEdit->isSelected() ){
                    if( activePart_ == CharAnimation::RIGHT_FOOT ){
                        moveToTop(bodyEditBtns[CharAnimation::RIGHT_FOOT]);
                        editing_ = true;
                    }
                } else {
                    activePart_ = CharAnimation::RIGHT_FOOT;
                    moveToTop(bodyEditBtns[CharAnimation::RIGHT_FOOT]);
                    editing_ = true;
                }
            }
        }
    } else



    //  head
    if( mouseEvent.getSource() == bodyToggleBtns[CharAnimation::HEAD] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[CharAnimation::HEAD]);
        activePart_ = CharAnimation::HEAD;
        lockEdit->setSelected(false);
    } else
    //  Body
    if( mouseEvent.getSource() == bodyToggleBtns[CharAnimation::BODY] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[CharAnimation::BODY]);
        activePart_ = CharAnimation::BODY;
        lockEdit->setSelected(false);
    } else
    //  Left arm
    if( mouseEvent.getSource() == bodyToggleBtns[CharAnimation::LEFT_HAND] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[CharAnimation::LEFT_HAND]);
        activePart_ = CharAnimation::LEFT_HAND;
        lockEdit->setSelected(false);
    } else
    //  right hand
    if( mouseEvent.getSource() == bodyToggleBtns[CharAnimation::RIGHT_HAND] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[CharAnimation::RIGHT_HAND]);
        activePart_ = CharAnimation::RIGHT_HAND;
        lockEdit->setSelected(false);
    } else
    //  LEFT FOOT
    if( mouseEvent.getSource() == bodyToggleBtns[CharAnimation::LEFT_FOOT] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[CharAnimation::LEFT_FOOT]);
        activePart_ = CharAnimation::LEFT_FOOT;
        lockEdit->setSelected(false);
    } else
    //  RIGHT FOOT
    if( mouseEvent.getSource() == bodyToggleBtns[CharAnimation::RIGHT_FOOT] ){
        mouseEvent.consume();
        moveToTop(bodyEditBtns[CharAnimation::RIGHT_FOOT]);
        activePart_ = CharAnimation::RIGHT_FOOT;
        lockEdit->setSelected(false);
    } else



    //New Frame button
    if( mouseEvent.getSource() == listAddBtn ){
        mouseEvent.consume();
        CharAnimation::BodyParts trueActivepart = activePart_;
        Directions trueDir = activeDir_;
        //only want the event to be fired once not 4x6 times
        frameLstBox->removeSelectionListener(this);
        for( int i = 0; i < 4; ++i ){
            activeDir_ = static_cast<Directions>(i);
            for( int j = 0; j < 6; ++j ){
                activePart_ = static_cast<CharAnimation::BodyParts>(j);
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
        CharAnimation::BodyParts trueActivepart = activePart_;
        Directions trueDir = activeDir_;
        //only want the event to be fired once not 4x6 times
        frameLstBox->removeSelectionListener(this);
        for( int i = 0; i < 4; ++i ){
            activeDir_ = static_cast<Directions>(i);
            for( int j = 0; j < 6; ++j ){
                activePart_ = static_cast<CharAnimation::BodyParts>(j);
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
        CharAnimation::BodyParts trueActivepart = activePart_;
        Directions trueDir = activeDir_;
        for( int i = 0; i < 4; ++i ){
            activeDir_ = static_cast<Directions>(i);
            for( int j = 0; j < 6; ++j ){
                activePart_ = static_cast<CharAnimation::BodyParts>(j);
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


    //fill up with loaded animation frames
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
            frameLstBox->setSelected(frameLst->getNumberOfElements()-1);
            //run logic to have it resize itself =]
            frameLstBox->logic();
            //scroll to bottom
            frameScroller->setScrollAmount( frameScroller->getHorizontalScrollAmount(),
                                            frameScroller->getVerticalMaxScroll());
        }
    }

    rateFld->setText( itos(anim_.anims[0][0].getFPS()) );

    return true;
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


void CharacterAnimBox::draw(gcn::Graphics *graphics){
    InfraellyWindow::draw(graphics);

    if( drawAnim_ ){
        anim_.draw( Screen::getSurface(), activeDir_, getX()+animOriginX_,
                    getY()+getTitleBarHeight()+animOriginY_ );
    }
}
