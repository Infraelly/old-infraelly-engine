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


//  Console.cpp

#include "Console.hpp"
#include "fonts.hpp"
//isSpace
#include "globalFunc.hpp"

Console::Console() :
    lastCommand(""),
    inputCallBack(NULL)
{
    initialised = 0;
    init();
}

Console::Console(SDL_Surface *screen) :
    lastCommand(""),
    inputCallBack(NULL)
{
    initialised = 0;
    init();
    setTarget(screen);
    //reset the position of all UI objects
    resetPositions();
}

Console::~Console(){
    delete sendButton;
    delete inputField;
    delete textArea;
    delete textScroller;
}


void Console::init(){
    if( !initialised ){
        setTabbingEnabled(false);

        username = "";
        lines = 1;
        textW = 640;
        textH = 400;

        container->setOpaque(0);

        //initialise the form objects
        textArea = new gcn::TextBox("Welcome to the Infraelly Console");
        textScroller = new InfraellyScroller(textArea);
        inputField = new gcn::TextField;
        sendButton = new gcn::Button("Send");

        //add objects to self
        container->add(textScroller);
        container->add(inputField);
        container->add(sendButton);


        //set the console as the gui's top widget
        setTop(container);



        //CONFIGURE FORM OBJECTS
        // add 'this' as a mouse listener
        sendButton->addMouseListener(this);
        //add the input field as key listener
        inputField->addKeyListener(this);
        //make the text area non editable
        textArea->setEditable(0);
        //make the scroll bars (dis)appear automaticaly
        textScroller->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        textScroller->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        //not focus
        sendButton->setFocusable(0);
        textArea->setFocusable(0);
        //dont allow tabbing
        container->setTabOutEnabled(false);
        textArea->setTabOutEnabled(false);
        textScroller->setTabOutEnabled(false);
        inputField->setTabOutEnabled(false);
        sendButton->setTabOutEnabled(false);

        //set the colours of the objects
        // bases
        container->setBaseColor( gcn::Color(0, 0, 0) );             //the bg black
        inputField->setBaseColor( gcn::Color(0, 0, 200) );          //input d_black
        textArea->setForegroundColor( gcn::Color(255, 255, 255) );  //scoller (white)
        textScroller->setBaseColor( gcn::Color(0, 100, 200) );      //scoller (blue)
        sendButton->setBaseColor( gcn::Color(0, 0, 0) );            //the button (black)
        // background
        inputField->setBackgroundColor( gcn::Color(0, 0, 0) );      // black
        textScroller->setBackgroundColor( gcn::Color(0, 0, 0) );    //scoller (black)
        sendButton->setBackgroundColor( gcn::Color(0, 0, 0) );      //the button (black)
        //foreground (text color)
        inputField->setForegroundColor( gcn::Color(0, 255, 0) );    //input(green)
        textArea->setForegroundColor( gcn::Color(255, 255, 255) );          //text (white)
        textScroller->setForegroundColor( gcn::Color(255, 255, 255) );  //scroller (white)
        sendButton->setForegroundColor( gcn::Color(255, 0, 0) );        //the button (red)


        //give it a readable font
        textArea->setFont(font::guiTitleFont.at(font::stdGuiTitleFontSize));
        inputField->setFont(font::guiTitleFont.at(font::stdGuiTitleFontSize));
        sendButton->setFont(font::guiTitleFont.at(font::stdGuiTitleFontSize));

        //give the button no border
        sendButton->setFrameSize(0);
        //make the text area not opaque
        textArea->setOpaque(0);

        //set sizes
        //textArea
        //100 px hight
        textArea->setHeight(100);
        //500px wide
        textArea->setWidth(500);
        //textScroller
        // match textArea
        textScroller->setDimension( textArea->getDimension() );
        //input field
        // match the textArea in width, but leave space for the button
        inputField->setWidth( textArea->getWidth() - sendButton->getWidth() );
        inputField->setHeight( 20 );
        //button
        //make button same hight as input field
        sendButton->setHeight( inputField->getHeight() );


        //set the x-y
        resetPositions();

        inputField->requestFocus();

        // set initialised
        initialised = 1;
    }
}


void Console::resetPositions(){
    if ( initialised ){
        int w = 0;
        int h = 0;
        if( graphics->getTarget() != NULL ){
            w = graphics->getTarget()->w;
            h = graphics->getTarget()->h;
        }

        //container (size of screen)
        container->setWidth( w );
        container->setHeight( h );

        //input field (bottom)
        inputField->setPosition( 0, h-inputField->getHeight() );
        inputField->setWidth( w-sendButton->getWidth() );
        inputField->setHeight( 20 );
        //button
        sendButton->setX( inputField->getX()+inputField->getWidth() );
        sendButton->setY( inputField->getY() );

        //text area (top left, width of screen, height -input fields)
        textArea->setPosition(0,0);
        //textArea->setWidth( w );
        //textArea->setHeight( h-inputField->getHeight() );
        //set size for scroller
        textW = w;
        textH = h-inputField->getHeight();
        textScroller->setDimension( gcn::Rectangle(0, 0, textW, textH) );

        textArea->setCaretRow(lines);
        textArea->scrollToCaret();
    };
}




void Console::push_back(const std::string& text, int outputFlags){
    if( isSpace(text) ) { return; };
    ++lines;

    if( outputFlags & PUT_USERNAME ){
        if( username == "" ){
            textArea->addRow(text);
        } else {
            textArea->addRow(username+": "+text);
        }
    } else {
        textArea->addRow(text);
    }

    textArea->setCaretRow(lines);
    textArea->scrollToCaret();
    if( outputFlags & DO_CALLBACK ){
        if( inputCallBack != NULL ){ (*inputCallBack)( text ); }
    }
}


void Console::clear(){
    lines = 0;
    textArea->setText("");
}




gcn::Button *Console::getSendButton()const{ return sendButton; }

gcn::TextField *Console::getInputField()const{ return inputField; }

gcn::TextBox *Console::getTextArea()const{ return textArea; }




std::string Console::getLastInput()const{ return textArea->getTextRow(lines); }




void Console::setUsername(const std::string& newUsername){
    username = newUsername;
}
const std::string& Console::getUsername(){
    return username;
}



void Console::setInputCallBack( InputCallBack *newCallBack ){
    inputCallBack = newCallBack;
}




void Console::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    //click from the text area
    if( mouseEvent.getSource() == textArea ){
        inputField->requestFocus();
    };

    //if click was from send button
    if( mouseEvent.getSource() == sendButton ){
        // add the text from the text field into the text area
        push_back( inputField->getText(), DO_CALLBACK|PUT_USERNAME);
        //store command entered
        lastCommand = inputField->getText();
        //clear off the text field
        inputField->setText("");
        //--------------------------
        //          play sound
        //------------------------
        inputField->requestFocus();
    }
}


void Console::keyPressed(gcn::KeyEvent& keyEvent){
    //if( keyEvent.isConsumed() ){ return; };

    //if enter pressed
    if( keyEvent.getKey().getValue() == gcn::Key::ENTER ){
        // on the input field
        if ( keyEvent.getSource() == inputField ){
            // add the text from the text field into the text area
            push_back( inputField->getText(), DO_CALLBACK|PUT_USERNAME );
            //store command entered
            lastCommand = inputField->getText();
            //clear off the text field
            inputField->setText("");
            inputField->requestFocus();
        }
    } else
    if( keyEvent.getKey().getValue() == gcn::Key::UP ){
        if( lastCommand != "" ){
            inputField->setText(lastCommand);
            inputField->setCaretPosition( lastCommand.length() );
        }
    }
}
