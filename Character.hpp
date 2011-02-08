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
#include "CharAnimation.hpp"
#include "globalFunc.hpp"

class Character{
    public:
        static const int revision_ = 1;

        //enumerations
        enum CharacterStates{ATTACKING, CASTING, EQUIPING, MOVING, STILL, TYPING};
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

        //  Inventory
        Inventory inventory;

        //  Stats
        Stats stats;

        //  Speech bubble
        SpeechBubble speechBubble;


        //  Standard ctor
        Character();
        ~Character();


        float getX()const{ return x; };
        float getY()const{ return y; };
        //  Returns the current X velocity
        float getXVel()const;
        //  Returns the current Y velocity
        float getYVel()const;
        //  Returns Character's
        enum Character::Jobs getClass()const;
        //  Returns Direct character is facing
        enum Directions getDirection()const;
        //  Returns Character's gender
        enum Character::Genders getGender()const;
        //  Returns the ActionState of teh character
        enum Character::CharacterStates getCharacterState()const{ return characterState; }
        //  Returns a string of the character's class
        std::string getClassStr()const;
        //  Returns character's name
        const std::string& getName()const;
        //returns the state
        enum CharacterStates getState()const;
        //  Returns true if state is locked
        bool stateLocked()const;
        //  Returns a pointer to the animation currently used
        CharAnimation getAnim()const;
        //  Retruns the images used for relevent body part
        const Tile& getTile( enum BodyParts part )const;


        void setX(float newX){ x = newX; };
        void setY(float newY){ y = newY; };
        void setXVel(float newXVel){ xVel = newXVel; }
        void setYVel(float newYVel){ yVel = newYVel; }
        void stop();
        //  Sets the Character's class
        void setClass(enum Character::Jobs newClass);
        //  Sets the Character's direction
        void setDirection(enum Directions newDirection);
        //  Sets the character's state
        void setState(enum Character::CharacterStates newState);
        //  Sets the character's gender
        void setGender(enum Character::Genders newGender);
        //  Sets the Character's name
        void setName(const std::string& newName);
        //  Setes lock state
        void lockState(bool v);
        //  Sets the Character's Animation
        void setAnim(const CharAnimation &newAnim);
        void setAnim(CharAnimation *newAnim);
        //  Sets the images used for relevent body part
        void setTile( enum BodyParts part, const Tile& newTile );

        //  Checks if its time to level up character
        void checkLevelUp();
        //  Returns how much exp is need to get to level "level"
        long getExpNeeded(unsigned level)const;


        //  Attacks
        void attack();
        //  Moves character in direction "directionToMove"
        void move(enum Directions directionToMove);
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
        enum Directions facing;                  //  Which direction Character is facing
        CharacterStates characterState;     //  Action State of Character
        bool stateIsLocked;                 //  Is action state editable
        CharAnimation anim_;                 //  Character's animation
        //  Images
        std::vector<Tile> bodyPartTiles_;

        float x;
        float y;
        float xVel;                         //  Horizontal velocity
        float yVel;                         //  Vertical velocity
        float xVelMax;                      //  Maximum horizontal velocity
        float yVelMax;                      //  Maximum vertical velocity


        //------------------
        //     "pivate" (dont save)
        long lastMoveTime;

        int nameLoc;                        //  The position to draw the name
        SDL_Rect healthBack;                //  The back of the health bar
        SDL_Rect healthFront;               //  The front of the health bar
        //------------------
};


std::string jobToString(enum Character::Jobs currentClass);
enum Character::Jobs stringToJob(std::string currentClass);





#endif

