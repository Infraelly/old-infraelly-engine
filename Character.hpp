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


#ifndef CHARACTER_HPP
#define CHARACTER_HPP

//  Character.hpp


#include <string>

#include "Tile.hpp"
#include "Item.hpp"
#include "Stats.hpp"
#include "SpeechBubble.hpp"
#include "Inventory.hpp"


class Character{
    public:
        static const int revision_ = 1;

        //enumerations
        enum Directions{UP, LEFT, DOWN, RIGHT};
        enum CharacterStates{ATTACKING, CASTING, EQUIPING, MOVING, STILL};
        enum Genders{MALE, FEMALE, ASEXUAL};
        enum Jobs{
            WANDERER,
            ELF, LIGHT_ELF, DARK_ELF,
            SPRITE, PIXIE, IMP,
                    NYMPH, HOB,
            EFT, TRITON, WYVERN,
            NPC
        };

        //check if value is a valid enumertaion value
        static bool validDirection(int dir);
        static bool validState(int state);
        static bool validGender(int gen);
        static bool validJob(int job);


        //  Images
        Tile head;          //  Image of head
        Tile face;          //  Image of face
        Tile body;          //  Image of body
        Tile hands;         //  Image of hands
        Tile feet;          //  Image of feet

        //  Inventory
        Inventory inventory;

        //  Stats
        Stats stats;

        //  Speech bubble
        SpeechBubble speechBubble;

        //  Standard ctor
        Character();
        ~Character();


        //  Returns Character's
        const enum Character::Jobs& getClass()const;
        //  Returns Direct character is facing
        const enum Character::Directions& getDirection()const;
        //  Returns Character's gender
        const enum Character::Genders& getGender()const;
        //  Returns the ActionState of teh character
        const enum Character::CharacterStates& getCharacterState()const{ return characterState; }
        //  Returns a string of the character's class
        const std::string getClassStr()const;
        //  Returns character's name
        const std::string& getName()const;
        //returns the state
        enum CharacterStates getState()const;
        //  Returns true if state is locked
        bool stateLocked()const;
        float getX()const{ return x; };
        float getY()const{ return y; };
        //  Returns the current X velocity
        float getXVel()const;
        //  Returns the current Y velocity
        float getYVel()const;


        void setX(float newX){ x = newX; };
        void setY(float newY){ y = newY; };
        void setXVel(float newXVel){ xVel = newXVel; }
        void setYVel(float newYVel){ yVel = newYVel; }
        void stop();
        //  Sets the Character's class
        void setClass(const enum Character::Jobs& newClass);
        //  Sets the Character's direction
        void setDirection(const enum Character::Directions& newDirection);
        //  Sets the character's state
        void setState(const enum Character::CharacterStates& newState);
        //  Sets the character's gender
        void setGender(const enum Character::Genders& newGender);
        //  Sets the Character's name
        void setName(const std::string& newName);


        //  Checks if its time to level up character
        void checkLevelUp();
        //  Returns how much exp is need to get to level "level"
        long getExpNeeded(unsigned level);


        //  Attacks
        void attack();
        //  Moves character in direction "directionToMove"
        void move(const enum Character::Directions& directionToMove);
        //  Uses item "item"
        void use(Item& item);


        //  This is what does the actualy x/y value changing
        //  the other version only edits the velocity
        //  this is for the server
        void move();


        //  Draws to "dest" at character's x-y
        void draw(SDL_Surface *dest);
        // Draws to "dest" at (x,y)
        void draw(SDL_Surface *dest, int x, int y);



        //save load
        //!@# make player data in sql
        bool save( const std::string& filename );
        // load
        bool load( const std::string& filename );



    private:
        std::string name;                   //  Character's name
        enum Jobs characterClass;           //  Character's class
        Genders gender;                     //  Character's gender
        Directions facing;                  //  Which direction Character is facing
        CharacterStates characterState;     //  Action State of Character
        bool stateIsLocked;                 //  Is action state editable

        float x;
        float y;
        float xVel;                         //  Horizontal velocity
        float yVel;                         //  Vertical velocity
        float xVelMax;                      //  Maximum horizontal velocity
        float yVelMax;                      //  Maximum vertical velocity


        //------------------
        //     "pivate" (dont save)
        Uint32 lastMoveTime;                  //  The time the character drawn
        int frameProgression;               //  The animtion frame number currently to be used
        int hand1AnimOffsetY;               //  The Y-Offset of hand1 for current animation frame
        int hand2AnimOffsetY;               //  The Y-Offset of hand2 for current animation frame
        int foot1AnimOffsetX;               //  The X-Offset of foot1 for current animation frame
        int foot2AnimOffsetX;               //  The X-Offset of foot2 for current animation frame

        int nameLoc;                        //  The position to draw the name
        SDL_Rect healthBack;                //  The back of the health bar
        SDL_Rect healthFront;               //  The front of the health bar
        //------------------
};


std::string jobToString(enum Character::Jobs currentClass);
enum Character::Jobs stringToJob(std::string currentClass);





#endif

