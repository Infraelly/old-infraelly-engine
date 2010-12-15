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


#ifndef THREADBASE_HPP_INCLUDED
#define THREADBASE_HPP_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>


//******************************************************************************
//
//                                 ThreadBase
//
//  ThreadBase is a wrapper around SDL_Thread.
//
//  It allows the controled creating, halting, pausing, resuming, and
//  destroying or threads.
//
//  It also allows data to be sent to and from the the thread in the form of
//  INFPackets.
//
//  To send data INTO a thread, use the sendToThread().
//  To recive data from sendToThread(), override the handleUserData()
//  function. This function will automatically be called everytime
//  data is sent to the thread using sendToThread().
//
//  To send data OUT of a thread to the outside world, use sendOut()
//  To recieve data sent to the outside of the thread use getOutData(),
//  and outDataReady() to find if there is data sent out.
//
//  Override the work() function to perform what ever actions you will
//  the thread to do. work() should not contain any loops as this will impede
//  ThreadBase's ability to recieve signals from start(), sendToThread(), etc.
//  work() is called repetetivly by ThreadBase anyway. When work returns false
//  the thread will exit.
//
//  A thread can be exited by work() returning false, or end()
//
//  sleep(ms, break) will sleep in blocks of "break" miliseconds, until a total
//  sleep-time of "ms" is reached. During this break time, signals and data
//  from the outside world can be recived
//
//  end() and resume() both work while pause()ed and sleep()ed, so long as
//  sleep()'s break time is high enough.
//
//  The rate is the target number of times work() and signals will be recieved
//  in a seccond. It is the equivelent of setting the frames per seccond
//
//******************************************************************************


namespace inp{ class INFPacket; }

class ThreadBase{
    public:
        enum RunState{
            //  Thread has not yet been started/created
            NEW,

            //  Thread is executing code
            RUNNING,

            //  Thread is paused
            PAUSED,

            //  Thread has been told to sleep for an arbitary amount of time
            SLEEPING,

            //  Thread has exited
            TERMINATED
        };


        ThreadBase();
        virtual ~ThreadBase();

        ThreadBase(const ThreadBase& src);
        ThreadBase& operator=(ThreadBase& rhs);


        //  Creates thread and waits for a start signal
        void create();
        //  Tells a thread to start
        void start();
        //  Aks thread to Stop and exit thread
        //  (works even if thread is sleeping)
        void end();
        //  Pauses thread
        void pause();
        //  Resume thread from pause or sleep
        void resume();
        //  Tells thread to sleep for a total of "ms" milliseconds.
        //  While the thread is 'sleeping', it will 'wake' up at reglar intevals
        //  ("sleepBreak") in order to check for system commands such as end();
        //  ie. this will cause the thread to sleep for a total of ms, sleepBreak
        //  miliseconds at a time.
        //  OVERIDES pause
        void sleep(int ms, int sleepBreak = 250);
        //  Waits until the thread has atucaly finished and exited
        void waitFinish();


        //  Returns the ID of the thread
        Uint32 getId()const;


        //  Returns the state of the thread
        enum RunState getRunState();

        //  Sets the target number of times work will be called per seccond
        void setRate(int rate);

        //  Gets the target number of times work will be called per second
        int getRate()const;


        //  This is the function that will be used as a thread
        static int thrdFunc(void *self);



        //  Allows a thread to send data out of the thread.
        //  to retriev the data sent out, use getOutData();
        void sendOut(const inp::INFPacket& data);

        //  Gets data sent from sendOut()
        //  NOTE: Returns false if no data.
        //          If there is data, returns true and assignes to "data"
        bool getOutData(inp::INFPacket& data);

        //  Returns true if there is data from sendOut();
        bool outDataReady();



        //  Sends an INFPacket to thread. User can recieve this by calling
        //  handleUserData();
        void sendToThread(const inp::INFPacket& data);


        //  This function is called every time the thread recievs data from
        //  send();
        //  Overide it to perform what ever manipulations you wish to the
        //  data with send;
        virtual void handleUserData(inp::INFPacket &data) = 0;


        //  ThrdFunc will repetetivly call this function until it returns false
        //  Returning false will cause the thread to exit
        //  Overide this function to perform the actions you wish the thread
        //      to execute.
        //  NOTE: Must not contain an infinite/gameLoop type structure, or else
        //     commands such as end(),send(),sleep() will not work.
        virtual bool work() = 0;



    private:
        void clean();
        void handleSignal();

        class ThreadBasePimpl_;
        ThreadBasePimpl_ *pimpl_;
};



#endif // THREADBASE_HPP_INCLUDED
