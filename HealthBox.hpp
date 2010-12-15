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


#ifndef HEALTHBOX_HPP
#define HEALTHBOX_HPP

//  HealthBox.hpp

#include <string>

#include "guichan.hpp"

#include "InfraellyWindow.hpp"
#include "ProgressBar.hpp"
#include "Character.hpp"
#include "Stats.hpp"


class HealthBox : public InfraellyWindow{
    public:
        HealthBox();
        HealthBox(Character *source);
        HealthBox(Stats *source);
        ~HealthBox();

        Character *getCharacterSource()const;
        Stats *getStatsSource()const;

        void setCharacterSource(Character *source);
        void setStatsSource(Stats *source);

        virtual void logic();


    protected:
        static unsigned short alpha;

        gcn::Label *hpBarText;
        gcn::Label *mpBarText;
        gcn::Label *stamBarText;
        gcn::Label *expBarText;

        ProgressBar *hpBarAmount;
        ProgressBar *mpBarAmount;
        ProgressBar *stamBarAmount;
        ProgressBar *expBarAmount;

        Character *characterSource;
        Stats *statsSource;

        void init();
};


#endif
