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


#ifndef NETPROTOCOL_HPP_INCLUDED
#define NETPROTOCOL_HPP_INCLUDED

#include <string>
#include <SDL/SDL.h>
#include "NetCode.hpp"

/*******************************************************************************

                                NetProtocol

    This file contains values used to control the transfer data over IP/TCP.


*******************************************************************************/


namespace inp{
    //debugger jump here is most likely due to a constuctor compiled without
    // debbuging info requiring string construction.

    const std::string inp_ver = "0.18";

    //  Packet Structure Control
    namespace ControlByte{
        //  Nothingness
        const NetCode NOTHING = 0;

        //  Start of packet
        const NetCode START = 1;

        //  Should go at end of all data
        const NetCode END = 2;

        //  The data about to be recived is a part of the previous packet
        const NetCode CONTINUE = 3;
        //  There is another segment of data about to be recived
        const NetCode WAIT_MORE = 4;
    }


    //  To know how to handle data
    namespace VarTypeByte{
        const NetCode B8 = 10;      //  bool                  (1b)
        const NetCode U8 = 11;      //  unsigend character    (1b)
        const NetCode S8 = 12;      //  signed character      (1b)
        const NetCode U16 = 13;     //  unsigned short        (2b)
        const NetCode S16 = 14;     //  signed short          (2b)
        const NetCode U32 = 15;     //  unsigned int          (4b)
        const NetCode S32 = 16;     //  signed int            (4b)
        const NetCode FPN = 17;     //  floating point number (4b)x2
        const NetCode STR = 18;     //  string
        const NetCode BIN = 19;     //  Binary data.
        const NetCode OTHER = 20;   //Unkown(most likely not a NetCode but data)
    }


    //  Server/client communications
    //  The "TRY"s are from the client
    //  The non-"TRY"s are affirmative responses from the server
    namespace DataTypeByte{
        //  Server is Shutting Down
        const NetCode SHUTDOWN = 30;

        /************************************
                        Ping
        *************************************/
        //  Client obtain ping
        //  Send ping request (client>server)
        const NetCode PING = 31;
        //  Reply to ping (server>client)
        const NetCode PONG = 32;

        //  Server Obtain Ping
        //  Server needs ping (server>client)
        const NetCode PING_REQ = 33;
        //  Client's ping (client>server)
        const NetCode PING_TIME = 34;

        /************************************
                Version Concurrency
        *************************************/
        //  Request the version of the INP
        const NetCode REQ_INP_VER = 40;
        //  Version of INP
        const NetCode INP_VER = 41;
        //  Request server version
        const NetCode REQ_SERVER_VER = 42;
        //  Server version
        const NetCode SERVER_VER = 43;
        //  Request client version
        const NetCode REQ_CLIENT_VER = 44;
        //  Version of client player is using
        const NetCode CLIENT_VER = 45;

        /************************************
                Connection Control
        *************************************/
        //  "disconnections and other errors, [are] determined by a failed read/write attempt"
        //  thus we just preiodicaly send this
        const NetCode CHECK_ALIVE = 50;
        //  Error message from sever (for compatibility)
        const NetCode SERVER_MSG = 51;
        //  Successfully Connected
        const NetCode CONNECT_SUCCESS = 52;
        //  Server denies connection request
        const NetCode CONNECT_DENY = 53;
        //  Sever kicks player
        const NetCode KICK = 54;



        /************************************

                Client Communication

        *************************************/
        //  New user has joined (client make new remote player)
        const NetCode USER_JOIN = 60;
        //  User has left (client remove a remote player from draw list)
        const NetCode USER_LEFT = 61;
        //  Username (login and remote player identification)
        const NetCode USERNAME = 62;
        //  Password
        const NetCode PASS = 63;
        //  Chat message
        const NetCode CHAT = 64;
        //  Make a gesture (infraelly emoticon)
        const NetCode GESTURE = 65;
        //  Character is typing
        const NetCode IS_TYPING = 66;

        /*******************************
                Player Attributes
        *******************************/
        //  The name of a player
        const NetCode CHAR_NAME = 70;
        //  The map the player is on
        const NetCode CHAR_MAP = 71;
        //  X Co-ordinate of player
        const NetCode CHAR_X = 72;
        //  X Co-ordinate of player
        const NetCode CHAR_Y = 73;
        //  Player's class
        const NetCode CHAR_CLASS = 74;
        //  Player's Gender
        const NetCode CHAR_GENDER = 75;
        //  Player's direction
        const NetCode CHAR_DIR = 76;
        //  Player's State (STILL, ATTACKING, MOVING, EQUIPING etc)
        const NetCode CHAR_STATE = 77;
        //  A stat of the player (hp, mp, str etc)
        const NetCode CHAR_STAT_TYPE = 78;
        //  A stat of the player (hp, mp, str etc)
        const NetCode CHAR_STAT_VAL = 79;


        /****************************
                Player actions
        ****************************/
        //  Request to perform an action (client -> server)
        const NetCode TRY_MOVE = 90;
        const NetCode TRY_STOP = 91;
        //Battle
        const NetCode TRY_ATTACK = 92;
        const NetCode TRY_DEFEND = 93;
        //Items
        const NetCode TRY_GAIN_ITEM = 94;
        const NetCode TRY_DROP_ITEM = 95;
        const NetCode TRY_USE_ITEM = 96;
        //Spell
        const NetCode TRY_GAIN_SPELL = 97;
        const NetCode TRY_CAST = 98;


        //  Perform an action (server -> client)
        const NetCode CHAR_MOVE = 99;
        const NetCode CHAR_STOP = 100;
        //Battle
        const NetCode CHAR_ATTACK = 101;
        const NetCode CHAR_DEFEND = 102;
        //Items
        const NetCode CHAR_GAIN_ITEM = 103;
        const NetCode CHAR_DROP_ITEM = 104;
        const NetCode CHAR_USE_ITEM = 105;
        //Spells
        const NetCode CHAR_GAIN_SPELL = 106;
        const NetCode CHAR_CAST = 107;




        /******************************
                Generic Attribute
        ******************************/
        //  For sending data as string,value pairs.
        //  ATTRIBUTE << descripting(string) << ATTRIBUTE_VALUE << value(T)
        const NetCode ATTRBUTE = 198;
        const NetCode ATTRIBUTE_VALUE = 199;


        //  Thread Control
        const NetCode THREAD_START = 200;
        const NetCode THREAD_PAUSE = 201;
        const NetCode THREAD_RESUME = 202;
        const NetCode THREAD_END = 203;
        const NetCode THREAD_SLEEP = 204;
        //  Send some text to server console from thread
        const NetCode THREAD_SEND_CONSOLE = 205;


        //  Default value of netcode
        const NetCode EMPTY = 255;
    }
}



#endif // NETPROTOCOL_HPP_INCLUDED
