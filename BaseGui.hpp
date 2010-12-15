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


#ifndef BASEGUI_HPP_INCLUDED
#define BASEGUI_HPP_INCLUDED


//  BaseGui.hpp

#include <SDL/SDL.h>

#include "guichan.hpp"
#include "guichan/sdl.hpp"

#include "Notification.hpp"
#include "AlertBox.hpp"


/***************************************************************

                    Base Gui Class
    All Infraelly Gui's should inherrit from this class

***************************************************************/



class BaseGui : public gcn::Gui,
                public gcn::MouseListener,
                public gcn::KeyListener{
    public:
        BaseGui();
        BaseGui(SDL_Surface *screen);
        virtual ~BaseGui();



        //  Children should overide this function
        //  to allow runtime dynamicaly resizeable
        virtual void resetPositions();



        //  Displays an alert box containing the message "message"
        void alert(const std::string& message);
        //  Displays a box containing the message "message".
        //  Remember to call unotify to get rid of box
        void notify(const std::string& message);
        //  Hides the notification
        void unNotify();



        //  Returns a pointer to the container in use
        const gcn::Container *getContainer()const;
        //  Returns the inbuilt container
        const gcn::Container *getBaseContainer()const;
        //  Sets the container to use
        void setContainer(gcn::Container *newContainer);


        //  Adds widget to container
        void add(gcn::Widget *wgt);


        //  This sets the SDL_Suface the gui should be drawing to
        void setTarget(SDL_Surface *screen);
        //  This is used to hand the gui input
        void pushInput(SDL_Event event);


        //mouse listener
        void mouseClicked(gcn::MouseEvent& mouseEvent){};




    protected:
        void init();                    //  Initialises object

        gcn::SDLInput *input;           //  Interfaces SDL input to guichan input
        gcn::SDLGraphics *graphics;     //  Interfaces SDL with guichan (gfx)

        gcn::Container *container;      //  Holds widgets, let's user use gui more naturaly
        gcn::Container *customContainer;//  Holds pointer to custom container if one is used
        bool baseContainer;             //  Whether the base container is in use

        Notification *notification;     //  A standard Box with a message inside
        AlertBox *alertBox;             //  A box with an ok button.
};







#endif // BASEGUI_HPP_INCLUDED
