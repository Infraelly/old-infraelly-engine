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


#ifndef CONNECTION_HPP_INCLUDED
#define CONNECTION_HPP_INCLUDED

#include <iosfwd>
#include <string>

//auto_ptr
#include <memory>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "INFPacket.hpp"

/*******************************************************************************
                        Connection

    This class is a thread safe container to manage information about a socket
    connection: the TCPSocket, SDLNet_SocketSet, IPaddress, username of the
    player, duration of connection and whether the socket is still
    active(connected).

    This class encapsulates fucntions for safe sending and recieving of network
    data.

    Additionaly, it will also manage data (de)encryption, (de)compression for
    data send and recieved. (not yet)

    CONNNECTION EXPECTS:
    | DATA TYPE | VAR TYPE | DATA TYPE | VAR TYPE | ...

*******************************************************************************/



namespace inp{
    class Connection{
        public:
            //friend class ::PlayerConnection;
            Connection();
            explicit Connection(TCPsocket& userSocket);
            explicit Connection(SDLNet_SocketSet& group, TCPsocket& userSocket);
            Connection(const Connection& src);
            Connection& operator=(Connection& rhs);
            virtual ~Connection();


            //These are the best ways to use the Class
            int init(IPaddress *ip);
            int init(const std::string& host, int port = 1337);
            int initServer(int port = 1337);


            int connect();
            int connect(IPaddress *ip);
            int connect(const std::string &host, int port);
            void disconnect();


            //  Checks if there are any sockets ready to be read in the same
            //      SocketSet as this one.
            //  Returns the number of sockects ready, or -1 on error.
            //  -2 is returned if Socket is not in a SocketSet exists.
            int checkSet(int timeout = 200);
            //  Check Set MUST be called first
            //  if checkSetFirst is true, checkSet will automaticly be called first
            //  Checks if the socket is ready to be read.
            //  Returns non 0 on success and -1 on failure
            int checkSocket(bool checkSetFirst = true);


            //  For servers
            //  Accepts incomming connections.
            //  Returns the newly connected Connection, or NULL on error
            std::auto_ptr<Connection> accept();

            //  returns -1 on error (disconnection or corrupt packet)
            int send(const inp::INFPacket& data);

            //  They return -1 on error (disconnection or corrupt packet)
            //  The data is still stored into dest in an error
            //  0 if nothing to recieve
            int recv(inp::INFPacket& dest);
            //  Same as recv(INFPacket&), except will wait for activity until timeout
            //  (calls recv(INFPacket&) )
            int recvWait(inp::INFPacket& dest, Uint32 ms);


            //  Returns the SocketSet the socket is in
            SDLNet_SocketSet getSocketSet()const;
            //  Returns the underlying socket
            TCPsocket getSocket()const;
            //  Returns the peer the socket is connected to
            IPaddress getPeer()const;
            //if a connection is ever inactive, it probably means an error has
            //  occured and it probbaly shud be disconected.
            bool isActive()const;
            //  Returns the time the connection was established
            Uint32 getConnectTime()const;
            //  Returns the time the connection has been established for
            Uint32 getTimeConnected()const;


            //  Adds socket to the given socket set
            //  If it was already part of a group, it will be removed from the
            //     old group and aded to the new one.
            int setSocketSet(const SDLNet_SocketSet& group);
            //  Sets the underlying socket.
            //  This will NOT disconnect the old one first
            void setSocket(const TCPsocket& userSocket);
            //  Sets the Connection's peer.
            //  This is for making new outbound connections
            void setPeer(IPaddress* peer);
            //  Sets the active state of the Connection
            void setActive(bool value);


            //  Makes a SocketSet and adds the socket to it
            //  This will remove the socket from any set it was apart of prior
            void makeSet(int max = 1);
            //  Removes socket from set and if internal socket set was used,
            //      free memory. If an external socket set was provided, this will
            //      only remove the socket from the set.
            void cleanSet();

            //  Give the Connection an id
            void setId(const std::string& newId);
            //  Returns the Connection's id
            std::string getId();




        protected:
            virtual void clean();

            //recieve a single byte, ms = timout
            int recvOne(Uint8* data, Uint32 ms = 5000, bool lockMutex = true);

            //  This structure contains all the private data members
            struct ConnectionImpl;

            //  Pointer to the private data member container
            ConnectionImpl* pimpl_;
    };

    //generates a queue of 'packets' that are the right
    //  size to be sent
    void splitForSend(const INFPacket& src, PacketList& destList);
}



#endif // CONNECTION_HPP_INCLUDED
