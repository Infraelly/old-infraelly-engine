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


#ifndef CONSOLE_HPP
#define CONSOLE_HPP

//  ServerConsole.hpp

#include <string>

#include "guichan.hpp"

#include "BaseGui.hpp"
#include "InfraellyScroller.hpp"

#include "InputCallBack.hpp"


class Console : public BaseGui{
    public:
        enum outputFlags{
            DO_NOTHING = 0,
            DO_CALLBACK = 0x01,
            PUT_USERNAME = 0x10
        };

        Console();
        Console(SDL_Surface *screen);
        virtual ~Console();


        void push_back(const std::string& text, int outputFlags = DO_NOTHING);
        void clear();

        gcn::Button *getSendButton()const;
        gcn::TextField *getInputField()const;
        gcn::TextBox *getTextArea()const;

        std::string getLastInput()const;

        void setUsername(const std::string& newUsername);
        const std::string& getUsername();

        //call bac must be a function returning void, taking in a string as a param
        // null to clear function
        void setInputCallBack( InputCallBack *newCallBack );


        virtual void mouseClicked(gcn::MouseEvent& mouseEvent);
        virtual void keyPressed(gcn::KeyEvent& keyEvent);

        virtual void resetPositions();



    protected:
        void init();

        bool initialised;
        int lines;
        int textW;
        int textH;
        std::string username;
        std::string lastCommand;

         //console area
        gcn::Button *sendButton;
        gcn::TextField *inputField;
        gcn::TextBox *textArea;
        InfraellyScroller *textScroller;

        //function is called when text is recieved
        InputCallBack *inputCallBack;
};


#endif
