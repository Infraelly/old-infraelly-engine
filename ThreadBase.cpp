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


#include "ThreadBase.hpp"
#include "INFPacket.hpp"
#include "MutexLocker.hpp"
#include "Timer.hpp"
#include <queue>


class ThreadBase::ThreadBasePimpl_{
    public:
        friend class ThreadBase;

        //create mutex
        ThreadBasePimpl_() :
            stateAccess(SDL_CreateMutex()),
            signalAccess(SDL_CreateMutex()),
            userDataAccess(SDL_CreateMutex()),
            outDataAccess(SDL_CreateMutex()),
            refCount(0),
            canStart(SDL_CreateCond()),
            thread(NULL),
            thrdCreated(false),
            running(false),
            paused(false),
            sleeping(false),
            sleepTimeLeft(0),
            sleepBreak(250),
            frameDelay(1000/30),
            state(NEW)
        { ; }

        //destroy mutex
        ~ThreadBasePimpl_(){
            SDL_DestroyMutex(stateAccess);
            SDL_DestroyMutex(signalAccess);
            SDL_DestroyMutex(userDataAccess);
            SDL_DestroyMutex(outDataAccess);
            stateAccess = NULL;
            signalAccess = NULL;
            userDataAccess = NULL;
            outDataAccess = NULL;

            SDL_DestroyCond(canStart);
            canStart = NULL;
        }

    private:
        SDL_mutex *stateAccess;
        SDL_mutex *signalAccess;
        SDL_mutex *userDataAccess;
        SDL_mutex *outDataAccess;

        unsigned refCount;

        SDL_cond *canStart;
        SDL_Thread *thread;

        bool thrdCreated;
        bool running;
        bool paused;
        bool sleeping;
        int sleepTimeLeft;
        int sleepBreak;
        int frameDelay;
        enum ThreadBase::RunState state;

        std::queue<inp::INFPacket> controlSignals;
        std::queue<inp::INFPacket> userData;
        std::queue<inp::INFPacket> outData;
};


ThreadBase::ThreadBase() : pimpl_(new ThreadBasePimpl_)
{ ; }

ThreadBase::~ThreadBase(){ clean(); }

ThreadBase::ThreadBase(const ThreadBase& src){
    //lock mutex
    ScopedMutexLock(src.pimpl_->stateAccess);
    pimpl_ = src.pimpl_;
    ++pimpl_->refCount;
}

ThreadBase& ThreadBase::operator=(ThreadBase& rhs){
    if( &rhs != this ){
        //lock mutex
        ScopedMutexLock(rhs.pimpl_->stateAccess);
        clean();
        pimpl_ = rhs.pimpl_;
        ++pimpl_->refCount;
    }
    return *this;
}

void ThreadBase::clean(){
    //lock mutex
    MutexLocker lock(pimpl_->stateAccess);
    --pimpl_->refCount;
    if( pimpl_->refCount == 0 ){
        //tell lock to not unlock mutex upon destruction
        lock.clear();
        delete pimpl_;
        pimpl_ = NULL;
    }
    pimpl_ = NULL;
}


//  Creates thread
void ThreadBase::create(){
    ScopedMutexLock(pimpl_->stateAccess);
    if( !pimpl_->thrdCreated ){
        pimpl_->thrdCreated = true;
        pimpl_->thread = SDL_CreateThread( &ThreadBase::thrdFunc, static_cast<void*>(this) );
    }
}

//  Tells thread to start
void ThreadBase::start(){
    inp::INFPacket startPack;
    startPack << inp::DataTypeByte::THREAD_START;

    ScopedMutexLock(pimpl_->signalAccess);
    pimpl_->controlSignals.push(startPack);
}

//  Stop and exit thread
void ThreadBase::end(){
    inp::INFPacket endPack;
    endPack << inp::DataTypeByte::THREAD_END;

    ScopedMutexLock(pimpl_->signalAccess);
    pimpl_->controlSignals.push(endPack);
}

//  Pauses thread
void ThreadBase::pause(){
    inp::INFPacket pausePack;
    pausePack << inp::DataTypeByte::THREAD_PAUSE;

    ScopedMutexLock(pimpl_->signalAccess);
    pimpl_->controlSignals.push(pausePack);
}

//  Resume thread from pause
void ThreadBase::resume(){
    inp::INFPacket resumePack;
    resumePack << inp::DataTypeByte::THREAD_RESUME;

    ScopedMutexLock(pimpl_->signalAccess);
    pimpl_->controlSignals.push(resumePack);
}

//  Tells thread to sleep for a total of "ms" milliseconds.
//  While the thread is 'sleeping', it will 'wake' up at reglar intevals
//  ("sleepBreak") in order to check for system commands such as end();
//  ie. this will cause the thread to sleep for a total of ms, sleepBreak
//  miliseconds at a time.
void ThreadBase::sleep(int ms, int sleepBreak){
    inp::INFPacket sleepPack;
    sleepPack << inp::DataTypeByte::THREAD_SLEEP << ms << sleepBreak;

    ScopedMutexLock(pimpl_->signalAccess);
    pimpl_->controlSignals.push(sleepPack);
}

//  Waits until the thread has atucaly finished and exited
void ThreadBase::waitFinish(){
    end();
    SDL_WaitThread( pimpl_->thread, NULL );
}


//  Returns the ID of the thread
Uint32 ThreadBase::getId()const{
    ScopedMutexLock(pimpl_->stateAccess);
    if( pimpl_->thrdCreated ){
        return SDL_GetThreadID(pimpl_->thread);
    }
    return 0;
}



//  Returns the state of the thread
enum ThreadBase::RunState ThreadBase::getRunState(){
    ScopedMutexLock(pimpl_->stateAccess);
    return pimpl_->state;
}


//  Sets the target number of times work will be called per seccond
void ThreadBase::setRate(int rate){
    if( rate < 1 ){ rate = 30; }
    ScopedMutexLock(pimpl_->stateAccess);
    pimpl_->frameDelay = (1000/rate);
}


//  Gets the target number of times work will be called per second
int ThreadBase::getRate()const{
    ScopedMutexLock(pimpl_->stateAccess);
    return (int)(1000/pimpl_->frameDelay);
}




//  Allows a thread to send data out of the thread
//  to retriev the data sent out, use getOutData();
void ThreadBase::sendOut(const inp::INFPacket& data){
    ScopedMutexLock(pimpl_->outDataAccess);
    pimpl_->outData.push(data);
}

//  Gets data sent from sendOut()
//  NOTE: Returns false if no data.
//          If there is data, returns true and assignes to "data"
bool ThreadBase::getOutData(inp::INFPacket& data){
    ScopedMutexLock(pimpl_->outDataAccess);
    if( !pimpl_->outData.empty() ){
        data = pimpl_->outData.front();
        pimpl_->outData.pop();
        return true;
    }
    return false;
}

bool ThreadBase::outDataReady(){
    ScopedMutexLock(pimpl_->outDataAccess);
    return !pimpl_->outData.empty();
}




//  Sends an INFPacket to thread. User can recieve this by calling
//  handleUserData();
void ThreadBase::sendToThread(const inp::INFPacket& data){
    ScopedMutexLock(pimpl_->userDataAccess);
    pimpl_->userData.push(data);
}




// Handles control signals (end, sleep, resuem)
void ThreadBase::handleSignal(){
    ScopedMutexLock(pimpl_->signalAccess);
    if( (!pimpl_->controlSignals.empty()) ){
        inp::INFPacket recvPack;
        recvPack = pimpl_->controlSignals.front();
        pimpl_->controlSignals.pop();

        inp::NetCode code;
        recvPack >> code;

        ScopedMutexLock(pimpl_->stateAccess);
        if( code == inp::DataTypeByte::THREAD_START ){
            pimpl_->state = RUNNING;
        } else
        if( code == inp::DataTypeByte::THREAD_END ){
            if( pimpl_->running ){
                pimpl_->running = false;
                pimpl_->paused = false;
                pimpl_->sleeping = false;
                pimpl_->sleepTimeLeft = 0;
                pimpl_->state = TERMINATED;
            }
        } else
        if( code == inp::DataTypeByte::THREAD_PAUSE ){
            if( pimpl_->running && (!pimpl_->sleeping) ){
                pimpl_->paused = true;
                pimpl_->state = PAUSED;
            }
        } else
        if( code == inp::DataTypeByte::THREAD_RESUME ){
            if( pimpl_->running ){
                pimpl_->paused = false;
                pimpl_->sleeping = false;
                pimpl_->state = RUNNING;
            }
        } else
        if( code == inp::DataTypeByte::THREAD_SLEEP ){
          if( pimpl_->running ){
                pimpl_->paused = false;
                pimpl_->state = SLEEPING;
                recvPack >> pimpl_->sleepTimeLeft;
                recvPack >> pimpl_->sleepBreak;
                if( pimpl_->sleepBreak < 1 ){
                    pimpl_->sleepBreak = 150;
                }
          }
        }
    }
}



//  This is the function that will be used as a thread
int ThreadBase::thrdFunc(void *void_self){
    ThreadBase *self;
    self = static_cast<ThreadBase*>(void_self);

    SDL_LockMutex(self->pimpl_->stateAccess);
    self->pimpl_->running = true;
    self->pimpl_->state = NEW;

    //wait for start signal
    while( self->pimpl_->state == NEW ){
        SDL_UnlockMutex(self->pimpl_->stateAccess);
        self->handleSignal();
        SDL_Delay(200);
        SDL_LockMutex(self->pimpl_->stateAccess);
    }

    //main run loop
    Timer runtimer;
    while( self->pimpl_->running ){
        if( (!self->pimpl_->paused) && (!self->pimpl_->sleeping) ){
            self->pimpl_->state = RUNNING;

            runtimer.clear();
            runtimer.start();

            //handle user sent data
            SDL_LockMutex(self->pimpl_->userDataAccess);
            while( !self->pimpl_->userData.empty() ){
                self->handleUserData( self->pimpl_->userData.front() );
                self->pimpl_->userData.pop();
            }
            SDL_UnlockMutex(self->pimpl_->userDataAccess);

            //do work
            if( !self->work() ){ return 0; }

            //handle control signal
            self->handleSignal();

            runtimer.stop();
            //cap framerate
            if( runtimer.getTime() < self->pimpl_->frameDelay ){
                if( !self->pimpl_->sleeping ){
                    self->pimpl_->sleeping = true;
                    self->pimpl_->sleepTimeLeft = self->pimpl_->frameDelay-runtimer.getTime();
                }
            }
        }

        // pause
        while( self->pimpl_->paused ){
            SDL_UnlockMutex(self->pimpl_->stateAccess);
            SDL_Delay(200);
            self->handleSignal();
            SDL_LockMutex(self->pimpl_->stateAccess);
        }

        //sleep
        if( self->pimpl_->sleeping ){
            while( (self->pimpl_->sleepTimeLeft > self->pimpl_->sleepBreak)  &&
                    self->pimpl_->sleeping  ){
                self->pimpl_->sleepTimeLeft -= self->pimpl_->sleepBreak;
                int delay = self->pimpl_->sleepBreak;
                SDL_UnlockMutex(self->pimpl_->stateAccess);
                SDL_Delay(delay);
                self->handleSignal();
                SDL_LockMutex(self->pimpl_->stateAccess);
            }
            if( self->pimpl_->sleepTimeLeft > 0 ){
                SDL_Delay(self->pimpl_->sleepTimeLeft);
                self->pimpl_->sleepTimeLeft = 0;
            }
            self->pimpl_->sleeping = false;
        }

        //mandatory signal read time
        /*SDL_UnlockMutex(self->pimpl_->stateAccess);
        SDL_Delay(50);
        self->handleSignal();
        SDL_LockMutex(self->pimpl_->stateAccess);*/
    }
    return 0;
}
