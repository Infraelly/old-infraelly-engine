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


#ifndef MUTEXLOCKER_HPP_INCLUDED
#define MUTEXLOCKER_HPP_INCLUDED

#include <iostream>

#include <SDL/SDL.h>


/*******************************************************************************
in teh making
                                MutexLocker

    A simple class that will lock the mutex passed to it, and release it
    upon destruction. Supposed to have been the auto_ptr of mutexes.

    Turned out quite useless.

*******************************************************************************/

class MutexLocker{
    public:
        MutexLocker(SDL_mutex* m) : mutex(m) {
            if( SDL_mutexP(mutex) == -1 ){
                std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mutexP(mutex): " << SDL_GetError() << std::endl;
            }
        }

        //  Copying transfers respoinsiblity
        MutexLocker(MutexLocker& rhs){
            mutex = rhs.mutex;
            rhs.mutex = NULL;
        }

        //  Asigning transfers respoinsiblity
        MutexLocker& operator=(MutexLocker& rhs){
            if( &rhs != this ){
                mutex = rhs.mutex;
                rhs.mutex = NULL;
            }
            return *this;
        }


        //  Stops managing the mutex
        void clear(){ mutex = NULL; }

        //  Unlocks mutex then stops managing it
        void release(){
            if( mutex != NULL ){
                if( SDL_mutexV(mutex) == -1 ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mutexV(mutex): " << SDL_GetError() << std::endl;
                }
                mutex = NULL;
            }
        }


        ~MutexLocker(){
            if( mutex != NULL ){
                if( SDL_mutexV(mutex) == -1 ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mutexV(mutex): " << SDL_GetError() << std::endl;
                }
            }
        }


    private:
        SDL_mutex* mutex;
};

//conveinience macro
#define SafeLock(x) MutexLocker safeLockedMutex(x);


#endif // MUTEXLOCKER_HPP_INCLUDED
