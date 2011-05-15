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
#include <fstream>
#include <string>


/************************************************************************
still in the making

                                LogOut

    logOut outputs to the standard output device: cout
    logErr outputs to the standard error device: cerr

    logDebug differs from the logOut and logErr because it only outputs
    if DEBUG is defined. It always prefixes the file and line number,
    and always outputs to the standard error device: cerr.

************************************************************************/

namespace logs{
    class Logger{
        public:
            Logger(std::ostream& stream, std::ofstream& f_stream):
                std_stream_(stream), file_stream_(f_stream), active_(true){ ; }

            inline bool isActive()const{ return active_; }
            inline void activate(){ active_ = true; }
            inline void deactivate(){ active_ = false; }
            inline void setActive(bool b){ active_ = b; }
            inline void toggle(){ active_ = !active_; }

            inline Logger& operator<<(const std::string& message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(const char* message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(char message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(short message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(int message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(long message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(float message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(double message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }


            template<typename T>
            Logger& operator<<(T& message){
                if(active_)data_ << message;
                std_stream_ << message;
                return *this;
            }

            inline Logger& operator<<(Logger& src){
                if(active_)data_ << src.data_.str();
                std_stream_ << src.data_.str();
                return *this;
            }

            inline Logger& operator<<(std::ostream& o){
                if(active_)data_ << o;
                std_stream_ << o;
                return *this;
            }

            friend std::ostream& operator<<(std::ostream& o, Logger& l);

            void flush(){
                if( active_ ){
                    file_stream_ << data_.str() << std::flush;
                }
                std_stream_ << data_.str() << std::flush;
                data_.str("");
            }

        private:
            std::ostream& std_stream_;
            std::ofstream& file_stream_;
            std::stringstream data_;
            bool active_;
    };

    inline std::ostream& operator<<(std::ostream& o, Logger& l){
        if(l.active_)o << l.data_.str();
        return o;
    }

    extern Logger logOut;
    extern Logger logErr;
    extern Logger logDbg;

    bool logsInit(const std::string& out = "logOut.txt", const std::string& err = "logErr.txt", const std::string &dbg = "logDbg.txt");
    void logsQuit();

    inline void logsSetActive(bool v){
        logOut.setActive(v);
        logErr.setActive(v);
        logDbg.setActive(v);
    }

    inline void logsActivate(){
        logOut.activate();
        logErr.activate();
        logDbg.activate();
    }

    inline void logsDeactivate(){
        logOut.deactivate();
        logErr.deactivate();
        logDbg.deactivate();
    }

    inline void logsFlush(){
        logOut.flush();
        logErr.flush();
        logDbg.flush();
    }

    /*extern bool logs_inited;
    extern std::ofstream of_out;
    extern std::ofstream of_err;
    extern std::ofstream of_dbg;


    template<typename T>
    void logOut(std::ostream& stream, const T& message){
        if( GameConfig::logging && logs_inited ){
            #ifdef DEBUG
                of_out << __FILE__ << ", " << __LINE__ << ": ";
            #endif
            of_out << message << std::endl;
        }
        #ifdef DEBUG
           stream << __FILE__ << ", " << __LINE__ << ": ";
        #endif
        stream << message << std::endl;
    }


    template<typename T>
    void logOut(const T& message){
        if( GameConfig::logging && logs_inited ){
            #ifdef DEBUG
                of_out << __FILE__ << ", " << __LINE__ << ": ";
            #endif
            of_out << message << std::endl;
        }
        #ifdef DEBUG
            std::cout << __FILE__ << ", " << __LINE__ << ": ";
        #endif
        std::cout << message << std::endl;
    }

    template<typename T>
    void logErr(const T& message){
        if( GameConfig::logging && logs_inited ){
            #ifdef DEBUG
                of_err << __FILE__ << ", " << __LINE__ << ": ";
            #endif
            of_err << message << std::endl;
        }
        #ifdef DEBUG
            std::cerr << __FILE__ << ", " << __LINE__ << ": ";
        #endif
        std::cerr << message << std::endl;
    }

    template<typename T>
    void logDebug(const T& message){
        #ifdef DEBUG
            if( GameConfig::logging && logs_inited ){
                of_dbg << __FILE__ << ", " << __LINE__ << ": " <<  message << std::endl;
            }
            std::cerr << __FILE__ << ", " << __LINE__ << ": " << message << std::endl;
        #endif
    }*/
};


#endif // DEBUGOUT_H_INCLUDED
