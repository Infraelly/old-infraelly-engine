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


#ifndef CHATBOX_HPP
#define CHATBOX_HPP

//  ChatBox.hpp


#include <string>

#include "guichan.hpp"

#include "InfraellyWindow.hpp"
#include "InfraellyScroller.hpp"

#include "InputCallBack.hpp"



class ChatBox : public InfraellyWindow, public gcn::KeyListener{
    public:
        enum outputFlags{
            DO_NOTHING =    0x0000 ,
            DO_CALLBACK =   0x1000,
            PUT_USERNAME =  0x0100
        };

        ChatBox();
        ~ChatBox();

        void push_back(const std::string& text, int flags);
        void push_back(const std::string& text);
        void clear();

        void focus();

        bool isTyping();

        void setUsername(const std::string& newUsername);
        const std::string& getUsername();

        gcn::Button *getSendButton()const;
        gcn::TextField *getInputField()const;
        gcn::TextBox *getChatArea()const;

        int getFlags()const;
        void setFlags(int flag);

        //call bac must be a function returning void, taking in a string as a param
        // null to clear function
        void setInputCallBack( InputCallBack *newCallBack );

        virtual void mouseClicked(gcn::MouseEvent& mouseEvent);
        virtual void keyPressed(gcn::KeyEvent& keyEvent);

    protected:
        gcn::Button *sendButton;
        gcn::TextField *inputField;
        gcn::TextBox *chatArea;
        InfraellyScroller *chatScroller;

        std::string username;
        bool typing;
        int lines;

        //functor is called when text is recieved
        InputCallBack *inputCallBack;

        // formatting flags
        int flags;
};





#endif
