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


#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

//******************************************************************************
//
//                              Animation
//
//  This class details the behaviour of an animated object and stores its
//  AnimFrames.
//
//  It stores aspects such as Fps, alignment, snap point (for items), and a
//  timer to automaticaly cycle through the frames.
//
//******************************************************************************

#include <vector>
#include <map>
#include <string>
#include "Timer.hpp"
#include "Tile.hpp"
#include "TSprite.hpp"
#include "AnimFrame.hpp"
#include "AnimFx.hpp"
#include "AnimFxControl.hpp"
#include "TSprite.hpp"
#include "INFPacket.hpp"
#include "dataPacks.hpp"


//  To allow the animator to access frames
class ItemAnimBox;
class ObjectAnimBox;
class CharacterAnimBox;
class CutSceneAnimBox;
class AnimFrameList;
class CharAnimation;
class ResourcePack;


class Animation : public Cacheable {
    public:
        static const int revision_ = 1;

        friend class ItemAnimBox;
        friend class ObjectAnimBox;
        friend class CharacterAnimBox;
        friend class CutSceneAnimBox;
        friend class AnimFrameList;
        friend class CharAnimation;

        enum AnimType{ ITEM, CHARACTER_PART, OBJECT };

        enum SnapPoint{ HEAD,      BODY,
                        LEFT_HAND, RIGHT_HAND,
                        LEFT_FOOT, RIGHT_FOOT };

        enum KeyPoint{ TL,   TOP, TR,
                       LEFT, MID, RIGHT,
                       BL,   BOT, BR };

        static bool validAnimType(int type);
        static bool validSnapPoint(int point);
        static bool validKeyPoint(int point);


        Animation();
        Animation(const Animation& src);
        Animation& operator=(const Animation& rhs);
        ~Animation();

        //  Skips one frame in animation
        void frameForward();
        //  Skip "numberOfFrames" number of frames
        void frameForward(unsigned numberOfFrames);
        //  Rewinds one frame backward in animation
        void frameBackwards();
        //  Rewinds "numberOfFrames" number of frames backwards
        void frameBackwards(unsigned numberOfFrames);


        //  Sets name of animation
        void setName(const std::string& newName);
        //  Sets animation type
        void setType(enum AnimType type);
        //  sets the snap point
        void setSnapPoint(enum SnapPoint snapPoint);
        //  sets origin of animation image
        void setOrigin(enum KeyPoint origin);
        //  sets aligenment of the animation to snap point
        void setAlignment(enum KeyPoint align);
        //  sets image of object being animated
        void setImage(const TSprite& image);
        //  Sets the object which it will snap to
        void setSnapSource(Tile* snapSource);
        //  Sets the frame number
        void setFrameProgression(unsigned frameNumber);
        //  Sets speed of animation (in fps)
        void setFPS(unsigned int FPS);


        //  Gets name of animation
        const std::string& getName()const;
        //  gets animation type
        enum AnimType getType()const;
        //  gets the snap point
        enum SnapPoint getSnapPoint()const;
        //  gets origin of animation image
        enum KeyPoint getOrigin()const;
        //  gets aligenment of the animation to snap point
        enum KeyPoint getAlignment()const;
        //  Gets the object which it will snap to
        Tile* getSnapSource()const;
        //  sets image of object being animated
        const TSprite& getImage()const;
        //  Returns the number of frame
        int getFrameCount()const;
        //  Returns the speed of the sprite (in FPS)
        int getFPS()const;
        //  Returns which frame the animation is up to
        int getFrameProgression();



        //  save animation to file
        bool save( const std::string& filename );

        //  load animation from file.
        bool load( const std::string& filename );

        //  load animation from a data pack
        bool load( const ResourcePack& pack, const std::string& filepath );


        //  Load animation file to use as tileset by resource cache
        bool cache_load( const std::string& filename ){ return load(packs::mainPack, filename); }


        //  Draws the animation to "dest" at base position (x,y)
        void draw(SDL_Surface *dest, int x, int y);



        //add a frame/fx event
        void addFrame(const AnimFrame& newFrame, const AnimFxControl& newFx = AnimFxControl());
        //removes frame/fx event
        void remove(size_t i);
        //clears out animation
        void clear();


        // returns a pointer to a frame
        // returns NULL if out of range
        AnimFrame* getFrame(size_t i);
        //returns a pointer to an Fx frame
        // returns NULL if out of range
        AnimFxControl* getFxControl(size_t i);



    private:
        //save animation to packet
        bool save( inp::INFPacket& pack );
        // load animation from packet
        bool load( inp::INFPacket& pack );

        void advanceFrames();

        std::string name_;
        enum AnimType type_;
        enum SnapPoint snapPoint_;
        enum KeyPoint origin_;
        enum KeyPoint alignment_;

        Tile *snapSource_;
        TSprite image_;

        SDL_Surface *drawn_;
        SDL_Surface *final_;

        Timer timer_;
        int frameDelay_;              //  The time gap between frame advances (ms)
        int lastFrameAdvance_;        //  The time the last frame was advanced
        size_t frameProgression_;     //  The frame the animation is up to

        typedef std::multimap<std::string, AnimFx*> ActiveFxList;
        typedef ActiveFxList::iterator ActiveFxIterator;

        std::vector<AnimFrame> frames_;
        std::vector<AnimFxControl> fxFrames_;
        ActiveFxList activeFx_;
};

struct CharAnimation{
    public:
        enum BodyParts{
            HEAD = 0,
            BODY = 1,
            LEFT_HAND = 2,
            RIGHT_HAND = 3,
            LEFT_FOOT = 4,
            RIGHT_FOOT = 5
        };

        CharAnimation();

        //save animation to file
        bool save( const std::string& filename );

        // load animation from file.
        bool load( const std::string& filename );

        //  Draws the animation to "dest" at base position (x,y)
        void draw(SDL_Surface *dest, int x, int y);

        std::vector<Animation> bodyPart;
};

#endif // ANIMATION_HPP_INCLUDED
