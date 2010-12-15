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


#ifndef CONNECTIONGROUP_HPP_INCLUDED
#define CONNECTIONGROUP_HPP_INCLUDED

#include <SDL/SDL.h>

#include <vector>
#include <string>

#include "Connection.hpp"


/*******************************************************************************

                            ConnectionGroup

    ConnectionGroup is a thread safe class for grouping Connections. As with
    Connections, each newly created ConnectionGroup is unique, all copies
    share data.

    When adding a Connection to a ConnectionGroup, the Connection group will
    exclusivley lock the Connection's mutex.

    Copying a ConnectionManager object is anticipated to be an extremly
    expensive opperation, so be all means, please avoid doing so.

*******************************************************************************/
namespace inp{
    class ConnectionGroup{
        public:
            static unsigned minSocks;

            ConnectionGroup();
            ConnectionGroup(const ConnectionGroup& src);

            //  Disconnects and frees all the Connection's memory
            ~ConnectionGroup();

            ConnectionGroup& operator=(ConnectionGroup& rhs);



            //  Returns the Connection stored at i, if ou were to iterate 'i'
            //      items from the start
            inp::Connection* conAt(unsigned i)const;

            //  Returns the Key stored at i, if ou were to iterate 'i'
            //      items from the start
            std::string keyAt(unsigned i)const;



            //  Returns true if there are no current connections
            bool empty()const;

            //  Returns true if the key exists
            bool exists(const std::string& key)const;

            //  returns pointer to connection with coresponding id to "key"
            //  returns NULL if no such connnection exists
            Connection *getConnection(const std::string& key)const;

            //  Returns the number of Connections being managed
            unsigned size()const;

            //  Returns the maximum number of Connections managed
            unsigned sizePeak()const;


            //  Add a Connection to manager.
            //  Takes the key to which the Connection can be accessed with,
            //  A pointer to the Connection
            bool addConnection(const std::string& key, inp::Connection* c);



            //  Disconnects the Connection with key "key"
            //  Erases entry after discconection
            //  Frees connection's memory
            void erase(const std::string& key);

            //  Same as erase but more intuitive
            //  (calls erase)
            void disconnect(const std::string& key);

            //  Removes connection, your resposible for deallocating it later
            void remove(const std::string& key);

            //  Removes all connections, your resposible for deallocating them later
            void removeAll(const std::string& key);



            //  Disconnects all connections
            //  Clears out all entries when done
            //  Sends shutdown message
            void disconnectAll();



            //  Checks all sockets for activity. Will wait for time out
            //  if timeout is -1, will wait for up to 49 days.
            //  returns true if there are active sockets
            bool checkSockets(Uint32 timeout);

            //  Checks all sockets for activity. Will wait for time out
            //  if timeout is -1, will wait for up to 49 days.
            //  assigns vector of connections with activity to activeList
            bool checkSockets(Uint32 timeout, std::vector<Connection*>& activeList);



            //  Sends 'packet' too connection with key "key"
            bool sendTo(const std::string& key, const inp::INFPacket& packet);

            //  Sends "packet" too all Connections
            void sendAll(const inp::INFPacket& packet);
            //  Sends "packet" too all Connections, except "bar"
            void sendAllBar(const inp::INFPacket& packet, const inp::Connection* bar);
            void sendAllBar(const inp::INFPacket& packet, const std::string& bar);



        private:
            void clean();

            //  This structure contains all the private data members
            struct ConnectionGroupImpl;

            //  Pointer to the private data member container
            ConnectionGroupImpl* pimpl_;
    };
}

#endif // CONNECTIONGROUP_HPP_INCLUDED
