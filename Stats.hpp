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


#ifndef STATS_HPP
#define STATS_HPP

#include <SDL/SDL.h>
#include "INFPacket.hpp"

//  Stats.hpp


class Stats{
    public:
        enum StatObjects{
            HP = 0,
            MP = 1,
            EXP = 2,
            STAM = 3,
            STR = 4,
            DEF = 5,
            SPD = 6,
            INT = 7,
            DEX = 8,
            CHAR = 9,
            RES = 10,
            CON = 11,
            LUCK = 12,
            LVL = 13,
            MAX_HP = 14,
            MAX_EXP = 15,
            MAX_MP = 16,
            MAX_STAM = 17
        };
        static bool validStat(int stat);

        //  Total number of StatObjects stored
        static const unsigned int size_ = 18;


        Stats();
        virtual ~Stats();


        // Returns the value denoted by "stat"
        const Sint32& getValue(const enum StatObjects& stat)const;


        // Sets the value of "stat" to "value"
        void setValue(const enum StatObjects& stat, const Sint32 value);
        // Adds the value denoted by "value" to the current value of "stat"
        void addValue(const enum StatObjects& stat, const Sint32 value);

        bool savePacket( inp::INFPacket& packet )const;
        bool loadPacket( inp::INFPacket& packet );


    private:
        Sint32 values_[size_];
};


#endif
