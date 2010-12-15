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


#ifndef INFPACKET_HPP_INCLUDED
#define INFPACKET_HPP_INCLUDED


#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <cstddef>

#include <SDL/SDL_net.h>

#include "NetProtocol.hpp"


/**************************************************************

                            INFPacket

    A conveinient container for network ordered data,
    allowing the use of stream opperatos to stuff/extract
    data.

    NOTE: IO occurs in a FIFO fashion, so the order data is
          extracted, is the order it is inserted in.

          After data is extracted from the Packet, it is
          junked. It is not possible to restore data
          exctracted. For this reason, the

**************************************************************/


namespace inp{
    //  "Connection"s don't send INFPacket.data directly
    //  "Connection"s send slews of character arrays with headers and footers
    typedef std::vector< std::vector<Uint8> > PacketList;

    //  Maximum size of a packet
    const unsigned int maxSendSize = 256;

    class Connection;
    class INFPacket{
        public:
            friend class Connection;
            friend void splitForSend(const INFPacket& packet, PacketList& destList);

            INFPacket();
            ~INFPacket();

            //  Merging packets
            friend INFPacket& operator<<(INFPacket& dest, const INFPacket& source);
            //  Instertion of structuring data
            friend INFPacket& operator<<(INFPacket& dest, const NetCode& code);
            //  Insertion of regular data
            //friend INFPacket& operator<<(INFPacket& dest, const bool val);
            friend INFPacket& operator<<(INFPacket& dest, const Uint8 val);
            friend INFPacket& operator<<(INFPacket& dest, const Sint8 val);
            friend INFPacket& operator<<(INFPacket& dest, const Uint16 val);
            friend INFPacket& operator<<(INFPacket& dest, const Sint16 val);
            friend INFPacket& operator<<(INFPacket& dest, const Uint32 val);
            friend INFPacket& operator<<(INFPacket& dest, const Sint32 val);
            //VERY RUDIMENTARY
            friend INFPacket& operator<<(INFPacket& dest, const float val);

            friend INFPacket& operator<<(INFPacket& dest, const std::string& str);
            //  For debuging
            friend std::ostream& operator<<(std::ostream& o, const INFPacket& p);


            //  Check the data type of next bit of data stored
            friend NetCode peekTypeVar(const INFPacket& src);

            //  Extraction of a type code
            friend void operator>>(INFPacket& src, NetCode& dest);
            //  Extraction of data
            friend void operator>>(INFPacket& src, bool& dest);
            friend void operator>>(INFPacket& src, Uint8& dest);
            friend void operator>>(INFPacket& src, Sint8& dest);
            friend void operator>>(INFPacket& src, Uint16& dest);
            friend void operator>>(INFPacket& src, Sint16& dest);
            friend void operator>>(INFPacket& src, Uint32& dest);
            friend void operator>>(INFPacket& src, Sint32& dest);
            //VERY RUDIMENTARY
            friend void operator>>(INFPacket& src, float& dest);

            friend void operator>>(INFPacket& src, std::string& dest);


            //  Returns true if packet is empty
            bool empty()const;

            //  Returns true if there is nothing left to read
            bool readDone()const;

            //  Returns number of bytes stored by packet
            size_t getLength()const;

            //  Empties data contained within packet
            void clear();


            //when its absolutely nessceary to access the data buffer
            //  Handy for using INFPacket as a file format
            //returns a pointer to the buffer, use getLength to get size
            const Uint8* getBuf();

            // clears packet, then store data from a buffer into packets
            void readBuf(const Uint8* buf, size_t num);

            //clears packet then stores data from the file
            void readFile(std::ifstream& file, size_t num);



        private:
            size_t readIndex;
            std::vector<Uint8> data;
    };

    std::ostream& operator<<(std::ostream& o, const PacketList& pl);

    //for values between 0-1
    Uint32 floatToInt(float quotient);
    float intToFloat(Uint32 val);
}

#endif // INFPACKET_HPP_INCLUDED
