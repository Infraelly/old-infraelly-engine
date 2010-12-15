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


#ifndef NETCODE_HPP_INCLUDED
#define NETCODE_HPP_INCLUDED

#include <SDL/SDL.h>

/*******************************************************************************

                                NetCode

    Not really a class in its own right. It exists merely to allow an
    overloaded operator in the INFPacket class treat packet structuring
    bytes differently to ordinary data.


*******************************************************************************/

namespace inp{
    class INFPacket;
    class NetCode{
        public:
            NetCode() : code(255) {};
            NetCode(Uint8 val) : code(val) {};

            NetCode& operator=(const Uint8 val){
                code = val;
                return *this;
            }
            NetCode& operator=(const NetCode& rhs){
                code = rhs.code;
                return *this;
            }

            bool operator==(const NetCode& rhs)const{
                return (code == rhs.code);
            }
            bool operator!=(const NetCode& rhs)const{
                return (code != rhs.code);
            }
            bool operator==(const int rhs)const{
                return (code == rhs);
            }
            bool operator!=(const int rhs)const{
                return (code != rhs);
            }


            friend void operator>>(INFPacket& src, NetCode& dest);

            Uint8 getVal()const{ return code; }


        private:
            Uint8 code;
    };
}
inline bool operator==(const int lhs, const inp::NetCode& rhs){ return (lhs==rhs.getVal()); }
inline bool operator!=(const int lhs, const inp::NetCode& rhs){ return (lhs!=rhs.getVal()); }

#endif // NETCODE_HPP_INCLUDED
