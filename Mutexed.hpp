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


#ifndef MUTEXED_HPP_INCLUDED
#define MUTEXED_HPP_INCLUDED

#include "logOut.hpp"


/*******************************************************************************
                                    Mutexed

    Associates any object with an SDL_Mutex.

    Achieves the same effect as pair<T, SDL_mutex>

    Its a cheap way of protecting an object with a mutex.

*******************************************************************************/


template<typename T>
class Mutexed{
    public:
        Mutexed() :
            access(SDL_CreateMutex()),
            locked(false),
            data_(new T)
        {}

        virtual ~Mutexed(){
            SDL_DestroyMutex(access);
            access = NULL;
            delete data_;
            data_ = NULL;
        }

        /*void operator=(T& o)throw(){
            SDL_LockMutex(access);
            if( &o != data_ ){
                *data_ = o;
            }
            SDL_UnlockMutex(access);
        }*/


        void lock(){
            if(locked){ logs::logDebug("Double locked"); }
            SDL_LockMutex(access);
            locked = true;
        }

        void unlock(){
            if(!locked){ logs::logDebug("Double unlocked"); }
            locked = false;
            SDL_LockMutex(access);

        }

        T& data(){ return *data_; }



    protected:
        T *data_;
        SDL_mutex *access;
        bool locked;
};



template<typename T>
struct scopedLock{
    scopedLock(Mutexed<T> &o) : o_(o){
        o_.lock();
    }
    ~scopedLock(){
        o_.unlock();
    }

    private:
        Mutexed<T> &o_;
};



#endif // MUTEXED_HPP_INCLUDED
