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


#ifndef DEBUGOUT_H_INCLUDED
#define DEBUGOUT_H_INCLUDED

#include <iostream>
#include <sstream>

/************************************************************************
still in the making

                                LogOut

    Function templates will output the file and line number, only if
    DEBUG is defined.

    logOut outputs to the standard output device: cout
    logErr outputs to the standard error device: cerr

    logDebug differs from the logOut and logErr because it only outputs
    if DEBUG is defined. It always prefixes the file and line number,
    and always outputs to the standard error device: cerr.

************************************************************************/

namespace logs{
    template<typename T>
    void logOut(std::ostream& stream, const T& message){
        #ifdef DEBUG
            std::cout << __FILE__ << ", " << __LINE__ << ": ";
        #endif
        std::cout << message << std::endl;
    }


    template<typename T>
    void logOut(const T& message){
        #ifdef DEBUG
            std::cout << __FILE__ << ", " << __LINE__ << ": ";
        #endif
        std::cout << message << std::endl;
    }

    template<typename T>
    void logErr(const T& message){
        #ifdef DEBUG
            std::cerr << __FILE__ << ", " << __LINE__ << ": ";
        #endif
        std::cerr << message << std::endl;
    }

    template<typename T>
    void logDebug(const T& message){
        #ifdef DEBUG
            std::cerr << __FILE__ << ", " << __LINE__ << ": " << message << std::endl;
        #endif
    }
}


#endif // DEBUGOUT_H_INCLUDED
