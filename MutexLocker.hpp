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

#include "SDL/SDL_thread.h"


/*******************************************************************************
                                    MutexLocker

    Locks a mutex. unlocks mutex when lock object goes out of scope.

*******************************************************************************/

//conveinience macro
#define ScopedMutexLock(x) MutexLocker safeLockedMutex(x);

class MutexLocker{
    public:
        MutexLocker(SDL_mutex* m) : mutex_(m) {
            if( SDL_mutexP(mutex_) == -1 ){
                std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mutexP(mutex): " << SDL_GetError() << std::endl;
            }
        }

        //  Copying transfers respoinsiblity
        MutexLocker(MutexLocker& rhs){
            mutex_ = rhs.mutex_;
            rhs.mutex_ = NULL;
        }

        //  Asigning transfers respoinsiblity
        MutexLocker& operator=(MutexLocker& rhs){
            if( &rhs != this ){
                mutex_ = rhs.mutex_;
                rhs.mutex_ = NULL;
            }
            return *this;
        }

        //  Stops managing the mutex
        void clear(){ mutex_ = NULL; }

        //  Unlocks mutex then stops managing it
        void release(){
            if( mutex_ != NULL ){
                if( SDL_mutexV(mutex_) == -1 ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mutexV(mutex): " << SDL_GetError() << std::endl;
                }
                mutex_= NULL;
            }
        }

        ~MutexLocker(){
            if( mutex_ != NULL ){
                if( SDL_mutexV(mutex_) == -1 ){
                    std::cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mutexV(mutex): " << SDL_GetError() << std::endl;
                }
            }
        }

    private:
        SDL_mutex* mutex_;
};


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
            if(locked){ std::cout << "Double locked"; }
            SDL_LockMutex(access);
            locked = true;
        }

        void unlock(){
            if(!locked){ std::cout << "Double unlocked"; }
            locked = false;
            SDL_LockMutex(access);

        }

        T& data(){ return *data_; }


    protected:
        T *data_;
        SDL_mutex *access;
        bool locked;
};


#endif // MUTEXED_HPP_INCLUDED
