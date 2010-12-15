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


#include "LoginBox.hpp"
// string validation
#include "globalFunc.hpp"


LoginBox::LoginBox() :
    usernameLabel(new gcn::Label("Username: ")),
    passwordLabel(new gcn::Label("Password: ")),
    usernameTextField(new gcn::TextField("")),
    passwordTextField(new PasswordField(""))
    //passwordField(new PasswordField)
{
    setCaption("Login");
    setBaseColor(gcn::Color(204,235,150));

    usernameTextField->setWidth(100);
    passwordTextField->setWidth(100);

    usernameLabel->setPosition(10,10);
    usernameTextField->setPosition(usernameLabel->getWidth()+usernameLabel->getX(),10);

    passwordLabel->setPosition(10, usernameLabel->getY()+usernameLabel->getHeight()+5);
    passwordTextField->setPosition( passwordLabel->getX()+passwordLabel->getWidth(),
                                    passwordLabel->getY() );

    doneButton->addMouseListener(this);

    add(usernameLabel);
    add(usernameTextField);
    add(passwordLabel);
    add(passwordTextField);

    resizeToContent();
    setWidth(getWidth()+10);

    InfoForm::positionDefaultBtns();
}

LoginBox::~LoginBox(){
    //  delete                  //null
    delete usernameLabel;       usernameLabel = NULL;
    delete passwordLabel;       passwordLabel = NULL;
    delete usernameTextField;   usernameTextField = NULL;
    delete passwordTextField;   passwordTextField = NULL;
}


std::string LoginBox::getUsername(){
    return usernameTextField->getText();
}

std::string LoginBox::getPassword(){
    return passwordTextField->getText();
}


void LoginBox::setUsername(const std::string& newText){
    usernameTextField->setText(newText);
}

void LoginBox::setPassword(const std::string& newText){
    passwordTextField->setText(newText);
}

void LoginBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.getSource() == doneButton ){
        if( isAlphNum(usernameTextField->getText()) &&
            isAlphNum(passwordTextField->getText()) )
        {
            infoReady = true;
        } else {
            infoReady = false;
        }
    } else
    if( mouseEvent.getSource() == cancelButton ){
        setVisible(false);
    }
}

