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


//  Character.cpp

#include "Character.hpp"

#include <iostream>
#include "colours.hpp"
#include "fonts.hpp"
#include "globalFunc.hpp"
#include "Tileset.hpp"
#include "caches.hpp"
#include "CharAnimation.hpp"


using namespace std;

bool Character::validDirection(int dir){
    return ( (dir==UP) || (dir==LEFT) || (dir==DOWN) || (dir==RIGHT) );
}
bool Character::validState(int state){
    return ( (state==ATTACKING) || (state==CASTING) ||
             (state==EQUIPING)  || (state==MOVING)  ||
             (state==STILL)     || (state==TYPING)  );
}
bool Character::validGender(int gen){
    return ( (gen==MALE) || (gen==FEMALE) || (gen==ASEXUAL) );
}
bool Character::validJob(int job){
    return ( (job==WANDERER) ||
             (job==ELF)      || (job==LIGHT_ELF) || (job==DARK_ELF) ||
             (job==SPRITE)   || (job==PIXIE)     || (job==IMP)      ||
                                (job==NYMPH)     || (job==HOB)      ||
             (job==EFT)      || (job==TRITON)    || (job==WYVERN)   ||
             (job==NPC) );
}

std::string jobToString(enum Character::Jobs currentClass){
    switch(currentClass){
        case Character::WANDERER: return "Wanderer";
        case Character::ELF: return "Elf";
        case Character::LIGHT_ELF: return "Light Elf";
        case Character::DARK_ELF: return "Dark Elf";
        case Character::SPRITE: return "Sprite";
        case Character::PIXIE: return "Pixie";
        case Character::IMP: return "Imp";
        case Character::NYMPH: return "Nymph";
        case Character::HOB: return "Hob";
        case Character::EFT: return "Eft";
        case Character::TRITON: return "Triton";
        case Character::WYVERN: return "Wyvern";
        case Character::NPC: return "NPC";
        default: return "UNKOWN";
    }
    return "UNKOWN";
}
enum Character::Jobs stringToJob(std::string currentClass){
    currentClass = upperCase(currentClass);
    if(currentClass == "WANDERER"){ return Character::WANDERER; } else
    if(currentClass == "ELF"){ return Character::ELF; } else
    if(currentClass == "LIGHT_ELF"){ return Character::LIGHT_ELF; } else
    if(currentClass == "DARK_ELF"){ return Character::DARK_ELF; } else
    if(currentClass == "SPRITE"){ return Character::SPRITE; } else
    if(currentClass == "PIXIE"){ return Character::PIXIE; } else
    if(currentClass == "IMP"){ return Character::IMP; } else
    if(currentClass == "NYMPH"){ return Character::NYMPH; } else
    if(currentClass == "HOB"){ return Character::HOB; } else
    if(currentClass == "EFT"){ return Character::EFT; } else
    if(currentClass == "TRITON"){ return Character::TRITON; } else
    if(currentClass == "WYVERN"){ return Character::WYVERN; } else
    if(currentClass == "NPC"){ return Character::NPC; } else
    {
        return Character::WANDERER;
    }
    return Character::WANDERER;
}




Character::Character() :
    name("Un-named Wanderer"),
    characterClass(WANDERER),
    gender(MALE),
    facing(DOWN),
    characterState(STILL),
    stateIsLocked(false),
    bodyPartTiles_(8),
    x(0),
    y(0),
    xVel(0),
    yVel(0),
    xVelMax(200),
    yVelMax(200),
    lastMoveTime(0),
    nameLoc(0)
{
    stats.setValue( Stats::HP, 10 );
    stats.setValue( Stats::MAX_HP, 10 );
    stats.setValue( Stats::MP, 8 );
    stats.setValue( Stats::MAX_MP, 8 );
    stats.setValue( Stats::MAX_EXP, 100 );
    stats.setValue( Stats::STAM, 20 );
    stats.setValue( Stats::MAX_STAM, 20 );

    bodyPartTiles_[HEAD].setSource( cache::tilesets.loadResource("tilesets/heads.xml"), 0, 0 );
    bodyPartTiles_[FACE].setSource( cache::tilesets.loadResource("tilesets/faces.xml"), 0, 0 );
    bodyPartTiles_[BODY].setSource( cache::tilesets.loadResource("tilesets/bodyM.xml"), 0, 0 );
    bodyPartTiles_[LEFT_HAND].setSource( cache::tilesets.loadResource("tilesets/hands.xml"), 0, 0 );
    bodyPartTiles_[RIGHT_HAND].setSource( cache::tilesets.loadResource("tilesets/hands.xml"), 0, 0 );
    bodyPartTiles_[LEFT_FOOT].setSource( cache::tilesets.loadResource("tilesets/feet.xml"), 0, 0 );
    bodyPartTiles_[RIGHT_FOOT].setSource( cache::tilesets.loadResource("tilesets/feet.xml"), 0, 0 );

    setAnim( cache::charAnimations.loadResource("anim/char/c_walk.iaf") );
}

Character::~Character(){ ; }


//gets
bool Character::stateLocked()const{ return stateIsLocked; }

float Character::getXVel()const{ return xVel; }

float Character::getYVel()const{ return yVel; }

enum Character::Jobs Character::getClass()const{ return characterClass; }

enum Directions Character::getDirection()const{ return facing; }

enum Character::Genders Character::getGender()const{ return gender; }

const std::string& Character::getName()const{ return name; }

Character::CharacterStates Character::getState()const{ return characterState; }

std::string Character::getClassStr()const{
    return jobToString(characterClass);
}

CharAnimation Character::getAnim()const{ return anim_; }

const Tile& Character::getTile( enum BodyParts part )const{ return bodyPartTiles_[part]; }



//sets
void Character::setClass(enum Character::Jobs newClass){ characterClass = newClass; }

void Character::setDirection(enum Directions newDirection){ facing = newDirection; }

void Character::setState(enum Character::CharacterStates newState){ characterState = newState; }

void Character::setGender(enum Character::Genders newGender){
    gender = newGender;
    if( gender == MALE ){
        bodyPartTiles_[BODY].setSource( cache::tilesets.loadResource("tilesets/bodyM.xml"), 0, 0 );
    } else {
       bodyPartTiles_[BODY].setSource( cache::tilesets.loadResource("tilesets/bodyF.xml"), 0, 0 );
    }
}

void Character::lockState(bool v){ stateIsLocked = v; }

void Character::setName(const std::string& newName){ name = newName; }

void Character::setAnim(const CharAnimation &newAnim){ anim_ = newAnim; }

void Character::setAnim(CharAnimation *newAnim){
    if( newAnim != NULL ){
        anim_ = *newAnim;

        anim_.setTile(HEAD, bodyPartTiles_[HEAD]);
        anim_.setTile(FACE, bodyPartTiles_[FACE]);
        anim_.setTile(BODY, bodyPartTiles_[BODY]);
        anim_.setTile(LEFT_HAND, bodyPartTiles_[LEFT_HAND]);
        anim_.setTile(RIGHT_HAND, bodyPartTiles_[RIGHT_HAND]);
        anim_.setTile(LEFT_FOOT, bodyPartTiles_[LEFT_FOOT]);
        anim_.setTile(RIGHT_FOOT, bodyPartTiles_[RIGHT_FOOT]);
    }
}

void Character::setTile( enum BodyParts part, const Tile& newTile ){
    bodyPartTiles_[part] = newTile;
    anim_.setTile(part, newTile);
}



void Character::checkLevelUp(){
    if ( stats.getValue(Stats::EXP) < 0 ){ stats.setValue(Stats::EXP, 0); }
    if ( stats.getValue(Stats::EXP) >= stats.getValue(Stats::MAX_EXP) ){
        stats.addValue(Stats::LVL, 1);
        stats.setValue(Stats::EXP, 0) ;
        stats.setValue(Stats::MAX_EXP, getExpNeeded( stats.getValue(Stats::LVL) ));
    }
}

long Character::getExpNeeded(unsigned level)const{
    //x = lvel
    //y = exp tnl
    //y = x**3 + 22x**2 + 11x +100
    return ( (level*level*level) + (22*level*level) + (11*level) + 100 );
}




void Character::attack(){
    if( !stateIsLocked ){
        characterState = ATTACKING;
        stateIsLocked = 1;
    }
}

//move
void Character::move(){
    //  calculate the distance that should be moved
    if( lastMoveTime == 0 ){ lastMoveTime = SDL_GetTicks(); }
    float deltaTime = SDL_GetTicks() - lastMoveTime;
    //  move character
    y += (deltaTime*yVel)/1000.f;
    x += (deltaTime*xVel)/1000.f;
    //update timer
    lastMoveTime = SDL_GetTicks();
}

void Character::move(enum Directions directionToMove){
    if( !stateIsLocked ){
        facing = directionToMove;
        characterState = MOVING;
        switch(facing){
            case UP:
                yVel = -yVelMax;
                break;

            case DOWN:
                yVel = yVelMax;
                break;

            case LEFT:
                xVel = -xVelMax;
                break;

            case RIGHT:
                xVel = xVelMax;
                break;
        }
        move();
    }
}

void Character::stop(){
    xVel = 0;
    yVel = 0;
    characterState = STILL;
}

void Character::use(Item& item){
    if(item.getDurability() != 0){
        if( !item.actions_.empty() ){
            //cycle throught the actions list
            for(unsigned int i = 0; i < item.actions_.size(); ++i ){
                string action = item.actions_.at(i);
                vector <string> components;
                string strTemp;

                //extract seperate data components
                for( string::size_type j = 0; j < action.length(); ++j ){
                    if( action.at(j) != ' ' ){
                        strTemp += action.at(j);
                    } else {
                        //skip past white space
                        ++j;
                        //store data component extracted
                        components.push_back(strTemp);
                    }
                }

                if( components.at(0) == "HEAL" ){
                    if( isNumber(components.at(1)) ){
                        int value = atoi(components.at(1).c_str());
                        //caller->addHp(value);
                        stats.setValue(Stats::HP, value);
                    } else {
                        cerr << __FILE__ << " " << __LINE__ << ": " << "Value after Item action HEAL must be an integer" << endl;
                    }
                }
                if( components.at(0) == "MANA" ){
                    if( isNumber(components.at(1)) ){
                        int value = atoi(components.at(1).c_str());
                        //caller->addMp(value);
                        stats.setValue(Stats::MP, value);
                    } else {
                        cerr << __FILE__ << " " << __LINE__ << ": " << "Value after Item action HEAL must be an integer" << endl;
                    }
                }
                if( components.at(0) == "WARP" ){
                    //WARP 0 0 green.inm (x, y, map name)
                }
            }
            item.minusDurability();
        }
    }
}


void Character::draw(SDL_Surface *dest){
    draw(dest, int(x), int(y));
}

//draw
void Character::draw(SDL_Surface *dest, int x, int y){
    //back of hp bar
    healthBack.x = x;
    healthBack.y = y-5;
    healthBack.w = bodyPartTiles_[BODY].getSize().w;
    healthBack.h = 5;
    //hp bar
    healthFront.x = healthBack.x+1;
    healthFront.y = healthBack.y+1;
    if(stats.getValue(Stats::MAX_HP) > 0){
        healthFront.w = (healthBack.w-2)*stats.getValue(Stats::HP)/stats.getValue(Stats::MAX_HP);
    }else{
        healthFront.w = 0;
    }
    healthFront.h = healthBack.h-2;


    //where to put name
    int nameWidth = name.length()*6;
    int nameLoc = x + centerX(nameWidth, bodyPartTiles_[BODY].getWidth());

    //draw hp bar and name
    drawText( name, font::mainFont.at(16), colour::black, dest, nameLoc,
                y+bodyPartTiles_[HEAD].getHeight()
                 + bodyPartTiles_[BODY].getHeight()
                 + bodyPartTiles_[LEFT_FOOT].getHeight() );
    SDL_FillRect(dest, &healthBack, SDL_MapRGB(dest->format, 0,0,0));
    SDL_FillRect(dest, &healthFront, SDL_MapRGB(dest->format, 0,255,0));

    //draw speech bubble
    speechBubble.draw(dest, x+bodyPartTiles_[BODY].getWidth()*.75, y-10-speechBubble.getHeight());

    //      animation
    move();
    switch(characterState){
        case TYPING:
            drawText("Typing...", font::mainFont.at(16), colour::black, 50, dest, x, y-24);
            stateIsLocked = true;
            //delibertaly falling through
        case STILL:
            //draws the first frame of the animation
            anim_.draw( dest, facing, x, y, 0 );
            break;

        case MOVING:
            anim_.draw( dest, facing, x, y );
            break;

        case ATTACKING:
            anim_.draw( dest, facing, x, y, 0 );
            break;

        case EQUIPING:
            anim_.draw( dest, facing, x, y, 0 );
            break;

        default:
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unknown character state. Uninitiated?" << endl;
            characterState = STILL;
    }
}



bool Character::save( const std::string& filename ){
    //open file for writting
    std::string fixedFn = correctFilepath(filename);
    ofstream file( fixedFn.c_str(), ios::binary);
    if (!file.is_open()){
        file.close();
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to open file (ifstream): " << fixedFn << endl;
        return 0;
    } else {
        inp::INFPacket packet;

        packet << revision_;
        packet << name;
        packet << static_cast<Uint8>(characterClass);
        packet << static_cast<Uint8>(gender);
        packet << static_cast<Uint8>(facing);
        packet << x;
        packet << y;
        packet << xVelMax;
        packet << yVelMax;

        if( !stats.savePacket(packet) ){ return 0; }
        if( !inventory.savePacket(packet) ){ return 0; }

        if( !bodyPartTiles_[HEAD].savePacket(packet) ){ return 0; }
        if( !bodyPartTiles_[FACE].savePacket(packet) ){ return 0; }
        if( !bodyPartTiles_[BODY].savePacket(packet) ){ return 0; }
        if( !bodyPartTiles_[LEFT_HAND].savePacket(packet) ){ return 0; }
        if( !bodyPartTiles_[RIGHT_HAND].savePacket(packet) ){ return 0; }
        if( !bodyPartTiles_[LEFT_FOOT].savePacket(packet) ){ return 0; }
        if( !bodyPartTiles_[RIGHT_FOOT].savePacket(packet) ){ return 0; }

        file.write( (char*)packet.getBuf(), packet.getLength() );
        file.close();

        return true;
    }
}

bool Character::load( const std::string& filename ){
    //open file for reading
    std::string fixedFn = correctFilepath(filename);
    ifstream file(fixedFn.c_str(), ios::binary);
    if (!file.is_open()){
        file.close();
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to open file (ifstream): " << fixedFn << endl;
        return 0;
    } else {
        //get filesize
        size_t filesize = getFileSize(file);

        // read file into packet
        inp::INFPacket packet;
        packet.readFile( file, filesize );
        file.close();


        {
            int fileVer;
            packet >> fileVer;
            if( fileVer != revision_ ){
                std::cerr << __FILE__ << __LINE__ << "incompatible file version" << std::endl;
                return 0;
            }
        }

        //load data from packet
        Uint8 tmp;

        packet >> name;
        if(packet.readDone()){ return 0; }

        packet >> tmp;
        if(packet.readDone()){ return 0; }
        if( !validJob(tmp) ){ return 0;}
        characterClass = static_cast<Character::Jobs>(tmp);

        packet >> tmp;
        if(packet.readDone()){ return 0; }
        if( !validGender(tmp) ){ return 0;}
        gender = static_cast<Character::Genders>(tmp);

        packet >> tmp;
        if( packet.readDone() ){ return 0; }
        if( !validDirection(tmp) ){ return 0;}
        facing = static_cast<Directions>(tmp);

        packet >> x;
        if(packet.readDone()){ return 0; }

        packet >> y;
        if(packet.readDone()){ return 0; }

        packet >> xVelMax;
        if(packet.readDone()){ return 0; }

        packet >> yVelMax;
        if(packet.readDone()){ return 0; }


        if( !stats.loadPacket(packet)  ||  !inventory.load(packet)          ||
            !bodyPartTiles_[HEAD].loadPacket(packet)         ||
            !bodyPartTiles_[FACE].loadPacket(packet)         ||
            !bodyPartTiles_[BODY].loadPacket(packet)         ||
            !bodyPartTiles_[LEFT_HAND].loadPacket(packet)    ||
            !bodyPartTiles_[RIGHT_HAND].loadPacket(packet)   ||
            !bodyPartTiles_[LEFT_FOOT].loadPacket(packet)    ||
            !bodyPartTiles_[RIGHT_FOOT].loadPacket(packet) )
        {
            return 0;
        }

        return true;
    }
}
