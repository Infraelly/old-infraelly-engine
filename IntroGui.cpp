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


//  IntroGui.cpp

#include "IntroGui.hpp"


#include <cstdlib>  //exit()
#include <iostream> //cerr

#include "guichan.hpp"  //buttons

#include "fonts.hpp"
#include "colours.hpp"
#include "Screen.hpp"   //reset positions needs screen size

#include "Connection.hpp"

//centering
#include "globalFunc.hpp"

#include "GameContext.hpp"
#include "CreditContext.hpp"
#include "GameConfig.hpp"




IntroGui::IntroGui(){
    init();
}

IntroGui::IntroGui(SDL_Surface *screen){
    init();
    setTarget(screen);
}

IntroGui::~IntroGui(){
    delete startButton;
    delete helpButton;
    delete creditsButton;
    delete exitButton;
}



void IntroGui::init(){
    //Menu butons
    startButton = new gcn::Button("Start");
    helpButton = new gcn::Button("Help");
    creditsButton = new gcn::Button("Credits");
    exitButton = new gcn::Button("Exit");

    //mouse listners
    startButton->addMouseListener(this);
    helpButton->addMouseListener(this);
    creditsButton->addMouseListener(this);
    exitButton->addMouseListener(this);


    //keyboard listeners
    startButton->addKeyListener(this);
    helpButton->addKeyListener(this);
    creditsButton->addKeyListener(this);
    exitButton->addKeyListener(this);


    //position the container
    container->setX(0);
    container->setY(0);
    container->setWidth(Screen::getWidth());
    container->setHeight(Screen::getHeight());


    //put buttons into vector
    menu.push_back(startButton);
    menu.push_back(helpButton);
    menu.push_back(creditsButton);
    menu.push_back(exitButton);

    //get total hieght of stuff
    int height = 0;
    for(unsigned int i = 0; i < menu.size(); ++i ){
        height += menu.at(i)->getHeight();
    }
    //button positions + properties
    int startY = Screen::getHeight()-height;
    for(unsigned int i = 0; i < menu.size(); ++i ){
        //set font
        menu.at(i)->setFont(font::guiTitleFont.at(35));
        menu.at(i)->adjustSize();
        //set colour
        menu.at(i)->setBaseColor(gcn::Color(255, 200, 200, 128));
        menu.at(i)->setBackgroundColor(gcn::Color(255, 200, 200, 128));
        menu.at(i)->setForegroundColor(colour::guiBlack);
        //set not cousable
        menu.at(i)->setFocusable(false);
        //get rid of frame
        menu.at(i)->setFrameSize(0);

        //center
        int newX = centerX(menu.at(i)->getX(), Screen::getWidth());
        menu.at(i)->setX(newX);
        //set standard width
        menu.at(i)->setWidth(150);

        //set y
        menu.at(i)->setY(startY);
        startY += menu.at(i)->getHeight();
        //add button
        add(menu.at(i));
    }

    //login box
    loginBox = new LoginBox();
    add(loginBox);

    //mouse listening
    loginBox->doneButton->addMouseListener(this);
    loginBox->cancelButton->addMouseListener(this);

    //set position of login box
    loginBox->setX( centerX(loginBox->getWidth(), container->getWidth()) );
    loginBox->setY( centerY(loginBox->getHeight(), container->getHeight()) );
}



void IntroGui::resetPositions(){
    //position the container
    container->setX(0);
    container->setY(0);
    container->setWidth(Screen::getWidth());
    container->setHeight(Screen::getHeight());

    //get total hieght of stuff
    int height = 0;
    for(unsigned int i = 0; i < menu.size(); ++i ){
        height += menu.at(i)->getHeight();
    }

    //button positions + properties
    int startY = Screen::getHeight()-height;
    for(unsigned int i = 0; i < menu.size(); ++i ){
        int width = menu.at(i)->getWidth();
        if(width > Screen::getWidth()){
            menu.at(i)->setX((width - Screen::getWidth())/2);
        } else {
            menu.at(i)->setX((Screen::getWidth() - width)/2);
        }
        menu.at(i)->setY(startY);
        startY += menu.at(i)->getHeight();
    }

    //set position of login box
    loginBox->setX( centerX(loginBox->getWidth(), container->getWidth()) );
    loginBox->setY( centerY(loginBox->getHeight(), container->getHeight()) );
}


void IntroGui::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    //click from the map image ( worldImagePlaceHolder )
    if( mouseEvent.getSource() == startButton ){
        //display the login box
        loginBox->setVisible(true);
    } else
    if( mouseEvent.getSource() == helpButton ){
        //help
    } else
    if( mouseEvent.getSource() == creditsButton ){
        //run creadit loop
        CreditContext();
    } else
    if( mouseEvent.getSource() == exitButton ){
        //exit
        exit(EXIT_SUCCESS);
    } else

    //  Login Box
    if( mouseEvent.getSource() == loginBox->doneButton ){
        // proccess login details and connect to server
        if( loginBox->isInfoReady() ){
            // hide login box
            loginBox->infoTaken();
            loginBox->setVisible(false);

            //  start up the connection
            //  The connection to the server
            inp::Connection svrCon;
            svrCon.setId( loginBox->getUsername() );
            //attempt connect
            if( svrCon.init(GameConfig::serverIp, GameConfig::serverPort) == -1 ){
                alert("Unable to contact server (server is offline?) ");
            } else {
                inp::INFPacket packet;
                //verify version
                //send version
                packet << inp::DataTypeByte::INP_VER << inp::inp_ver;
                svrCon.send(packet);
                //wait response
                int rVal = svrCon.recvWait(packet, 30000);
                switch ( rVal ){
                    case -1:
                        svrCon.disconnect();
                        alert("Server droped connection");
                        return;

                    case 0:
                        svrCon.disconnect();
                        alert("Connection timeout");
                        return;

                    default:
                        inp::NetCode code;
                        packet >> code;
                        if( code == inp::DataTypeByte::CONNECT_SUCCESS ){
                            break;
                        } else
                        if( code == inp::DataTypeByte::CONNECT_DENY ){
                            std::string msg;
                            packet >> msg;
                            alert("Connection failed: "+msg);
                            return;
                        } else {
                            alert("Delegation failure. (Please update client?)");
                            return;
                        }
                }

                //log in
                packet.clear();
                packet << inp::DataTypeByte::USERNAME << svrCon.getId();
                //--------------------------
                // hash ps?
                //--------------------------
                packet << inp::DataTypeByte::PASS << "unsecure";
                svrCon.send(packet);
                //--------------------------
                // wait for confirmation
                //--------------------------
                GameContext( static_cast<inp::Connection*>(&svrCon) );
            }
        } else {
            // info not ready, but button was pushed: invalid usr/pass
            alert("Username and Password may only contain alpha-numeric characters");
        }
    }
}
