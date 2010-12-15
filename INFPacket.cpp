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


#include "INFPacket.hpp"
#include <iostream>

namespace inp{
    INFPacket::INFPacket() : readIndex(0) { data.reserve(maxSendSize); }

    INFPacket::~INFPacket(){ ; }

    size_t INFPacket::getLength()const{ return data.size(); }

    void INFPacket::clear(){ readIndex = 0; data.clear(); }

    NetCode peekTypeVar(const INFPacket& src){
        if( src.data.empty() ){ return ControlByte::NOTHING; }
        return src.data[src.readIndex];
    }




    INFPacket& operator<<(INFPacket& dest, const INFPacket& source){
        dest.data.insert( dest.data.end(), source.data.begin(), source.data.end() );
        return dest;
    }

   /* INFPacket& operator<<(INFPacket& dest, const bool val){
        //  Data type
        dest.data.push_back( VarTypeByte::B8.getVal() );
        //  Data
        dest.data.push_back( static_cast<Uint8>(val) );
        return dest;
    }*/

    INFPacket& operator<<(INFPacket& dest, const NetCode& code){
        dest.data.push_back( code.getVal() );
        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const Uint8 val){
        //  Data type
        dest.data.push_back( VarTypeByte::U8.getVal() );
        //  Data
        dest.data.push_back( val );
        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const Sint8 val){
        //  Data type
        dest.data.push_back( VarTypeByte::S8.getVal() );
        //  Data
        dest.data.push_back( val );
        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const Uint16 val){
        //  Data type
        dest.data.push_back( VarTypeByte::U16.getVal() );
        //  Data
        Uint8 tmp[2] = {0,0};
        SDLNet_Write16(static_cast<Uint16>(val), tmp);
        dest.data.push_back( tmp[0] );
        dest.data.push_back( tmp[1] );
        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const Sint16 val){
        //  Data type
        dest.data.push_back( VarTypeByte::S16.getVal() );
        //  Data
        Uint8 tmp[2] = {0,0};
        SDLNet_Write16(static_cast<Uint16>(val), tmp);
        dest.data.push_back( tmp[0] );
        dest.data.push_back( tmp[1] );
        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const Uint32 val){
        //  Data type
        dest.data.push_back( VarTypeByte::U32.getVal() );
        //  Data
        Uint8 tmp[4] = {0,0,0,0};
        SDLNet_Write32(static_cast<Uint32>(val), tmp);
        dest.data.push_back( tmp[0] );
        dest.data.push_back( tmp[1] );
        dest.data.push_back( tmp[2] );
        dest.data.push_back( tmp[3] );

        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const Sint32 val){
        //  Data type
        dest.data.push_back( VarTypeByte::S32.getVal() );
        //  Data
        Uint8 tmp[4] = {0,0,0,0};
        SDLNet_Write32(static_cast<Uint32>(val), tmp);
        dest.data.push_back( tmp[0] );
        dest.data.push_back( tmp[1] );
        dest.data.push_back( tmp[2] );
        dest.data.push_back( tmp[3] );

        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const std::string& str){
        //  Data type
        dest.data.push_back( VarTypeByte::STR.getVal() );

        //  String length
        Uint8 tmp[2] = {0,0};
        SDLNet_Write16(static_cast<Uint16>(str.length()), tmp);
        dest.data.push_back( tmp[0] );
        dest.data.push_back( tmp[1] );

        //  Data
        for( std::string::size_type i = 0; i < str.length(); ++i ){
            dest.data.push_back( static_cast<Uint8>(str[i]) );
        }
        return dest;
    }

    INFPacket& operator<<(INFPacket& dest, const float val){
        //  Data type
        dest.data.push_back( VarTypeByte::FPN.getVal() );

        Sint32 mantissa = static_cast<Sint32>(val);

        float quotient = val;

        if( val > 0 ){
            if( mantissa != 0 ){
                quotient -= mantissa;
            }
        } else {
            quotient *= -1;
            quotient += mantissa;
        }

        Uint32 encodeQuotient = floatToInt(quotient);

        //multiply by -1
        if( (mantissa == 0) && (val < 0) ){
            dest << static_cast<Uint8>(true);
        } else {
            dest << static_cast<Uint8>(false);
        }
        dest << mantissa;
        dest << encodeQuotient;

        return dest;
    }


    std::ostream& operator<<(std::ostream& o, const INFPacket& p){
        o << "Infraelly Packet Dump" << std::endl;
        o << "Index: " << p.readIndex << std::endl;
        for( unsigned int i = 0; i < p.data.size(); ++i ){
            o << static_cast<unsigned int>(p.data[i]) << " ";
        }
        o << std::endl;
        o << std::endl;
        return o;
    }



    /*--------------------------------------------------------*/
    /*                  Extraction opertators                  */
    /*--------------------------------------------------------*/



    void operator>>(INFPacket& src, NetCode& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get code
        dest.code = src.data[src.readIndex];
        ++src.readIndex;
    }



    void operator>>(INFPacket& src, bool& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint8 tmp = static_cast<bool>(src.data[src.readIndex]);
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = tmp;
            }
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16 ){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint16 tmp = static_cast<bool>( SDLNet_Read16( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<bool>( tmp );
            }
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32 ){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32 ){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 tmp = static_cast<bool>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<bool>( tmp );
            }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            if( (mantissa < 0) || !polarity ){
                dest = false;
            } else {
                dest = static_cast<bool>( mantissa );
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
    }



    void operator>>(INFPacket& src, Uint8& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint8 tmp = static_cast<Sint8>(src.data[src.readIndex]);
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = tmp;
            }
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint8>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint16 tmp = static_cast<Sint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<Uint8>( tmp );
            }
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint8>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 tmp = static_cast<Sint32>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<Uint8>( tmp );
            }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            if( (mantissa < 0) || !polarity ){
                dest = 0;
            } else {
                dest = static_cast<Uint8>( mantissa );
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
    }



    void operator>>(INFPacket& src, Sint8& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            if( dest < 0 ){ dest = 0; }
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint8>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint8>( SDLNet_Read16( &src.data[src.readIndex] ) );
            if( dest < 0 ){ dest = 0; }
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint8>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint8>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( dest < 0 ){ dest = 0; }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint8>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            dest = static_cast<Sint16>( mantissa );
            if( (mantissa == 0) && !polarity ){
                dest *= -1;
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
    }



    void operator>>(INFPacket& src, Uint16& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint8 tmp = static_cast<Sint8>(src.data[src.readIndex]);
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = tmp;
            }
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint16 tmp = static_cast<Sint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<Uint16>( tmp );
            }
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint16>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 tmp = static_cast<Sint32>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<Uint16>( tmp );
            }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            if( (mantissa < 0) || !polarity ){
                dest = 0;
            } else {
                dest = static_cast<Uint16>( mantissa );
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
   }



    void operator>>(INFPacket& src, Sint16& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint8>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
            if( dest < 0 ){ dest = 0; }
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint16>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( dest < 0 ){ dest = 0; }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint16>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            dest = static_cast<Sint16>( mantissa );
            if( (mantissa == 0) && !polarity ){
                dest *= -1;
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
     }



    void operator>>(INFPacket& src, Uint32& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint8 tmp = static_cast<Sint8>(src.data[src.readIndex]);
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = tmp;
            }
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint32>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint16 tmp = static_cast<Sint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<Uint32>( tmp );
            }
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint32>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 tmp = static_cast<Sint32>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( tmp < 0 ){
                dest = 0;
            } else {
                dest = static_cast<Uint32>( tmp );
            }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            if( (mantissa < 0) || !polarity ){
                dest = 0;
            } else {
                dest = static_cast<Uint32>( mantissa );
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
    }



    void operator>>(INFPacket& src, Sint32& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<bool>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint32>( src.data[src.readIndex] );
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint32>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint32>(SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Uint32>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( dest < 0 ){ dest = 0; }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<Sint32>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            Sint32 mantissa;
            float quotient;
            bool polarity;
            src >> polarity;
            src >> mantissa;
            src >> quotient;
            dest = static_cast<Sint32>( mantissa );
            if( (mantissa == 0) && !polarity ){
                dest *= -1;
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
    }


    void operator>>(INFPacket& src, float& dest){
        //check if theres enough to read from the packet
        if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
            dest = 0;
            return;
        }
        //get type
        NetCode typeCode = src.data[src.readIndex++];
        //Extract data according to its true data type
        //  then cast it to the requested form
        if( typeCode == VarTypeByte::B8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<float>(src.data[src.readIndex]);
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = src.data[src.readIndex];
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::S8){
            //check if theres enough to read from the packet
            if( (src.readIndex+1 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<float>( src.data[src.readIndex] );
            ++src.readIndex;
        } else
        if( typeCode == VarTypeByte::U16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<float>( SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::S16){
            //check if theres enough to read from the packet
            if( (src.readIndex+2 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<float>(SDLNet_Read16( &src.data[src.readIndex] ) );
            src.readIndex += 2;
        } else
        if( typeCode == VarTypeByte::U32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<float>( SDLNet_Read32( &src.data[src.readIndex] ) );
            if( dest < 0 ){ dest = 0; }
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::S32){
            //check if theres enough to read from the packet
            if( (src.readIndex+4 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            dest = static_cast<float>( SDLNet_Read32( &src.data[src.readIndex] ) );
            src.readIndex += 4;
        } else
        if( typeCode == VarTypeByte::FPN ){
            if( (src.readIndex+12 > src.data.size()) || src.data.empty() ){
                src.clear();
                dest = 0;
                return;
            }
            bool polarity = true;
            Sint32 mantissa = 0;
            float quotient = 0;
            src >> polarity;
            src >> mantissa;
            src >> quotient;

            quotient = intToFloat(quotient);

            dest = mantissa;
            if( mantissa > 0 ){
                 dest += quotient;
            } else {
                dest -= quotient;
            }
            if( (mantissa == 0) && !polarity ){
                dest *= -1;
            }
        } else {
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            dest = 0;
            return;
        }
        return;
    }



    void operator>>(INFPacket& src, std::string& dest){
        dest = "";
        //check if theres enough to read from the packet
        if( src.readIndex+3 > src.data.size() ){ return; }
        //get type
        if( src.data[src.readIndex] != VarTypeByte::STR   ){
            std::cerr << __LINE__ << ": Packet data extraction type mismatch" << std::endl;
            std::cerr << src;
            src.clear();
            return;
        }
        ++src.readIndex;

        //get length
        int len = static_cast<Uint16>( SDLNet_Read16( &src.data[src.readIndex] ) );
        src.readIndex += 2;

        //check if theres enough to read from the packet
        if( src.readIndex+len > src.data.size() ){ return; }

        //  String
        for( std::string::size_type i = 0; i < len; ++i ){
            dest += static_cast<Uint8>( src.data[src.readIndex] );
            ++src.readIndex;
        }
    }


    bool INFPacket::empty()const{ return data.empty(); }

    bool INFPacket::readDone()const{ return (readIndex == data.size()); }


    const Uint8* INFPacket::getBuf(){
        if( data.empty() ){ return NULL; }
        return reinterpret_cast<Uint8*>( &data[0] );
    }

    void INFPacket::readBuf(const unsigned char* buf, size_t num){
        data.clear();
        data.resize(num);
        memmove(&data[0], buf, num);
    }

    void INFPacket::readFile(std::ifstream& file, size_t num){
        data.clear();
        data.resize(num);
        file.read( (char*)&data[0], num );
    }




    std::ostream& operator<<(std::ostream& o, const PacketList& pl){
        o << "Infraelly PacketList Dump" << std::endl;
        for( unsigned int i = 0; i < pl.size(); ++i ){
            o << "Packet: " << i << std::endl;
            for( unsigned int j = 0; j < pl.at(i).size(); ++j ){
                o << (int)pl.at(i).at(j) << " ";
            }
            o << std::endl;
        }
        o << std::endl;
        return o;
    }



    const float floatMult = 512;

    Uint32 floatToInt(float val){
        Uint32 result = static_cast<Uint32>(val * (floatMult - 1) + 0.5f);
        if( result > (floatMult-1) ){
            result = floatMult-1;
        }
        return result;
    }

    float intToFloat(Uint32 val){
        return val/(floatMult);
    }

}
