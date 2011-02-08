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


#ifndef CHARANIMATION_HPP_INCLUDED
#define CHARANIMATION_HPP_INCLUDED

#include "Animation.hpp"
#include "INFPacket.hpp"
#include "globalFunc.hpp"
#include "Tile.hpp"

class Character;

class CharAnimation : public Cacheable {
    public:
        static const int revision_ = 1;

        CharAnimation();

        void clear();

        void setTile( enum BodyParts part, const Tile& newTile );

        //save animation to file
        bool save( inp::INFPacket& pack)const;
        bool save( const std::string& filename )const;

        // load animation from file.
        bool load( inp::INFPacket& pack );
        bool load( const std::string& filename );

        //  load animation from a data pack
        bool load( const ResourcePack& pack, const std::string& filepath );

        //  Load animation file to use as tileset by resource cache
        bool cache_load( const std::string& filename ){ return load(packs::mainPack, filename); }

        //  Draws the animation to "dest" at base position (x,y)
        void draw(SDL_Surface *dest, enum Directions facing, int x, int y);
        //  Draws frame of anim
        void draw(SDL_Surface *dest, enum Directions facing, int x, int y, int frameNumber);

        std::vector< std::vector<Animation> > anims;

        private:
            Tile face_;
            TSprite hair_;
};

#endif // CHARANIMATION_HPP_INCLUDED
