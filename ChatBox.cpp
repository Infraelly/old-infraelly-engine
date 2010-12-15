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


//  ChatBox.cpp

#include "ChatBox.hpp"
#include <SDL/SDL.h>
//isSpace
#include "globalFunc.hpp"

using namespace std;


ChatBox::ChatBox() :
    sendButton(new gcn::Button("Send")),
    inputField(new gcn::TextField),
    chatArea(new gcn::TextBox("Welcome to Infraelly ^^")),
    chatScroller(new InfraellyScroller(chatArea)),
    username(""),
    typing(false),
    lines(1),
    inputCallBack(NULL),
    flags(NULL)
{
    //set caption of window
    setCaption("Chat");

    //initialise the form objects
    // add 'this' as a mouse listener
    sendButton->addMouseListener(this);
    //add the input field as key listener
    inputField->addKeyListener(this);
    //make the chat area non editable
    chatArea->setEditable(0);
    //make the scroll bars (dis)appear automaticaly
    chatScroller->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
    chatScroller->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
    //button not focus
    sendButton->setFocusable(0);
    //dont allow tabbing
    setTabOutEnabled(false);
    chatArea->setTabOutEnabled(false);
    chatScroller->setTabOutEnabled(false);
    inputField->setTabOutEnabled(false);
    sendButton->setTabOutEnabled(false);

    //set the colours of the objects
    // bases
    setBaseColor( gcn::Color(255, 210, 119) );                //the window (l_orang)
    inputField->setBaseColor( gcn::Color(253, 204, 197) );    //input (pale orange)
    chatScroller->setBaseColor( gcn::Color(248, 244, 186) );  //scoller (pale yel)
    sendButton->setBaseColor( gcn::Color(247, 203, 170) );    //the button (l_pink/l_orange)
    // background
    inputField->setBackgroundColor( gcn::Color(253, 204, 197) );    //input (pale orange)
    chatScroller->setBackgroundColor( gcn::Color(253, 233, 206) );  //scoller (pale yel)
    //give the button no border
    sendButton->setFrameSize(0);
    //make the Chat area not opaqu
    chatArea->setOpaque(0);

    //set sizes
    //chatArea
    //100 px hight
    chatArea->setHeight(100);
    //500px wide
    chatArea->setWidth(500);
    //chatScroller
    // match chatArea
    chatScroller->setDimension( chatArea->getDimension() );
    //input field
    // match the chatArea in width, but leave space for the button
    inputField->setWidth( chatArea->getWidth() - sendButton->getWidth() );
    inputField->setHeight( 20 );
    //button
    //make button same hight as input field
    sendButton->setHeight( inputField->getHeight() );

    //set the co-ords of teh form objects
    chatScroller->setPosition( 0, 0 );                      //top left
    inputField->setPosition(0, chatArea->getHeight());      //bottom of chat scroller
    sendButton->setX( inputField->getWidth() );             //the right of the input field
    sendButton->setY( inputField->getY() );                 //same as input field

    //add objects to self
    add(chatScroller);
    add(inputField);
    add(sendButton);

    //resize window
    resizeToContent();
}

ChatBox::~ChatBox(){
    delete sendButton;
    delete inputField;
    delete chatScroller;
    delete chatArea;
}

void ChatBox::push_back(const std::string& text, int flags__){
    typing = 0;
    chatArea->requestFocus();
    SDL_EnableKeyRepeat(0, 0);
    if( isSpace(text) ) { return; }
    ++lines;

    std::string output = "";
    if( flags__ & PUT_USERNAME ){
        if( username != "" ){
            output = username + ": " + text;
        }
    } else {
        output = text;
    }

    chatArea->addRow(output);
    chatArea->setCaretRow(lines);
    chatArea->scrollToCaret();
    if( flags__ & DO_CALLBACK ){
        if( inputCallBack != NULL ){ (*inputCallBack)( text ); }
    }
}

void ChatBox::push_back(const std::string& text){ push_back(text, flags); }


void ChatBox::clear(){
    lines = 0;
    chatArea->setText("");
}



void ChatBox::focus(){
    if( !typing ){
        SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
        requestFocus();
        inputField->requestFocus();
        if( inputField->isFocused() ){
            typing = 1;
        }
    } else {
        // add the text from the text field into the chat area
        push_back( inputField->getText(), DO_CALLBACK|PUT_USERNAME );
        //clear off the text field
        inputField->setText("");
    }
}


bool ChatBox::isTyping(){
    if( inputField->isFocused() ){
        return 1;
    } else {
        return 0;
    }
}


void ChatBox::setUsername(const std::string& newUsername){
    username = newUsername;
}
const std::string& ChatBox::getUsername(){
    return username;
}


gcn::Button *ChatBox::getSendButton()const{ return sendButton; }

gcn::TextField *ChatBox::getInputField()const{ return inputField; }

gcn::TextBox *ChatBox::getChatArea()const{ return chatArea; }




int ChatBox::getFlags()const{ return flags; }

void ChatBox::setFlags(int flag){ flags = flag; }




void ChatBox::setInputCallBack( InputCallBack *newCallBack ){
    inputCallBack = newCallBack;
}





void ChatBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; }

    InfraellyWindow::mouseClicked(mouseEvent);

    //click from the chat area
    if( mouseEvent.getSource() == chatArea ){
        chatArea->requestFocus();
        typing = 0;
    }

    //click from the input field
    if( mouseEvent.getSource() == inputField ){
        typing = 1;
    }

    //if click was from send button
    if( mouseEvent.getSource() == sendButton ){
        // add the text from the text field into the chat area
        push_back( inputField->getText(), DO_CALLBACK|PUT_USERNAME );
        //clear off the text field
        inputField->setText("");
        //--------------------------
        //          play sound
        //------------------------
    }
}


void ChatBox::keyPressed(gcn::KeyEvent& keyEvent){
    //if( keyEvent.isConsumed() ){ return; }

    //if enter pressed
    if( keyEvent.getKey().getValue() == gcn::Key::ENTER ){
        // on the input field
        if ( keyEvent.getSource() == inputField ){
            // add the text from the text field into the chat area
            push_back( inputField->getText(), DO_CALLBACK|PUT_USERNAME );
            //clear off the text field
            inputField->setText("");
        }
    }
}
