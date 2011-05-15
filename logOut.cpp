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
#include <iostream>
#include <fstream>

#include "logOut.hpp"

namespace logs{
    static bool logs_inited = 0;

    std::ofstream of_out;
    std::ofstream of_err;
    std::ofstream of_dbg;

    Logger logOut(std::cout, of_out);
    Logger logErr(std::cerr, of_err);
    Logger logDbg(std::cerr, of_dbg);


    bool logsInit(const std::string& out, const std::string& err, const std::string &dbg){
        if( logs_inited ){
            logsQuit();
            return logsInit(out, err, dbg);
        } else {
            of_out.open(out.c_str());
            of_err.open(err.c_str());
            of_dbg.open(dbg.c_str());
            if( !of_out.is_open() || !of_err.is_open() || !of_dbg.is_open() ){
                std::cerr << "Cannot open log files: ";
                if(!of_out.is_open() ){ std::cerr << out; }
                if(!of_err.is_open() ){ std::cerr << ", " << err; }
                if(!of_dbg.is_open() ){ std::cerr << ", " << dbg; }
                std::cerr << std::endl;
            }
            if( !of_out.is_open() && !of_err.is_open() && !of_dbg.is_open() ){
                logs_inited = false;
            } else {
                logs_inited = true;
                std::cout << "Logs inited: \n";
            }
        }
        #ifndef DEBUG
            logDbg.deactivate();
        #endif
        return logs_inited;
    }

    void logsQuit(){
        if( logs_inited ){
            logOut.flush();
            logErr.flush();
            logDbg.flush();
            of_out << std::flush;
            of_err << std::flush;
            of_dbg << std::flush;
            of_out.close();
            of_err.close();
            of_dbg.close();
            logs_inited = false;
        }
    }
}
