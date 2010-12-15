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


using namespace std;


bool Character::validDirection(int dir){
    return ( (dir==UP) || (dir==LEFT) || (dir==DOWN) || (dir==RIGHT) );
}
bool Character::validState(int state){
    return ( (state==ATTACKING) || (state==CASTING) ||
             (state==EQUIPING)  || (state==MOVING)  ||
             (state==STILL) );
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
    x(0),
    y(0),
    xVel(0),
    yVel(0),
    xVelMax(200),
    yVelMax(200),
    lastMoveTime(0),

    frameProgression(0),
    hand1AnimOffsetY(0),
    hand2AnimOffsetY(0),
    foot1AnimOffsetX(0),
    foot2AnimOffsetX(0),
    nameLoc(0)
{
    stats.setValue( Stats::HP, 10 );
    stats.setValue( Stats::MAX_HP, 10 );
    stats.setValue( Stats::MP, 8 );
    stats.setValue( Stats::MAX_MP, 8 );
    stats.setValue( Stats::MAX_EXP, 100 );
    stats.setValue( Stats::STAM, 20 );
    stats.setValue( Stats::MAX_STAM, 20 );

    head.setSource( cache::tilesets.loadResource("tilesets/heads.xml"), 0, 0 );
    face.setSource( cache::tilesets.loadResource("tilesets/faces.xml"), 0, 0 );
    body.setSource( cache::tilesets.loadResource("tilesets/bodyM.xml"), 0, 0 );
    hands.setSource( cache::tilesets.loadResource("tilesets/hands.xml"), 0, 0 );
    feet.setSource( cache::tilesets.loadResource("tilesets/feet.xml"), 0, 0 );
}

Character::~Character(){ ; }


//gets
const enum Character::Jobs& Character::getClass()const{ return characterClass; }

const enum Character::Directions& Character::getDirection()const{ return facing; }

const enum Character::Genders& Character::getGender()const{ return gender; }

const std::string& Character::getName()const{ return name; }

enum Character::CharacterStates Character::getState()const{ return characterState; }

const std::string Character::getClassStr()const{
    return jobToString(characterClass);
}

bool Character::stateLocked()const{ return stateIsLocked; }

float Character::getXVel()const{ return xVel; }

float Character::getYVel()const{ return yVel; }







//sets
void Character::setClass(const enum Character::Jobs& newClass){ characterClass = newClass; }

void Character::setDirection(const enum Character::Directions& newDirection){ facing = newDirection; }

void Character::setState(const enum Character::CharacterStates& newState){ characterState = newState; }

void Character::setGender(const enum Character::Genders& newGender){
    gender = newGender;
    if( gender == MALE ){
        body.setSource( cache::tilesets.loadResource("tilesets/bodyM.xml"), 0, 0 );
    } else {
       body.setSource( cache::tilesets.loadResource("tilesets/bodyF.xml"), 0, 0 );
    }
}

void Character::setName(const std::string& newName){ name = newName; }





void Character::checkLevelUp(){
    if ( stats.getValue(Stats::EXP) < 0 ){ stats.setValue(Stats::EXP, 0); }
    if ( stats.getValue(Stats::EXP) >= stats.getValue(Stats::MAX_EXP) ){
        stats.addValue(Stats::LVL, 1);
        stats.setValue(Stats::EXP, 0) ;
        stats.setValue(Stats::MAX_EXP, getExpNeeded( stats.getValue(Stats::LVL) ));
    }
}

long Character::getExpNeeded(unsigned level){
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

void Character::move(const enum Character::Directions& directionToMove){
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
                    int value;
                    if( isNumber(components.at(1)) ){
                        value = atoi(components.at(1).c_str());
                        //caller->addHp(value);
                        stats.setValue(Stats::HP, value);
                    } else {
                        cerr << __FILE__ << " " << __LINE__ << ": " << "Value after Item action HEAL must be an integer" << endl;
                    }
                }
                if( components.at(0) == "MANA" ){
                    int value;
                    if( isNumber(components.at(1)) ){
                        value = atoi(components.at(1).c_str());
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
    healthBack.w = body.getSize().w;
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
    int nameLoc = x + centerX(nameWidth, body.getWidth());

    //draw hp bar and name
    drawText(name, font::mainFont.at(16), colour::black, dest, nameLoc, y+head.getHeight() + body.getHeight() + feet.getHeight());
    SDL_FillRect(dest, &healthBack, SDL_MapRGB(dest->format, 0,0,0));
    SDL_FillRect(dest, &healthFront, SDL_MapRGB(dest->format, 0,255,0));

    //draw speech bubble
    speechBubble.draw(dest, x+body.getWidth()*.75, y-10-speechBubble.getHeight());

    //      animation
    move();
    //advance the frame progression
    ++frameProgression;
    //limit frameprogression
    switch(characterState){
        case STILL:
            frameProgression = 0;
            stateIsLocked = false;
            break;

        case MOVING:
            //4 frame
            if(frameProgression > 3){
                frameProgression = 0;
            }
            //abs(-3ax/16 +a) -a/4
            hand1AnimOffsetY = abs((frameProgression*(-3*hands.getSize().h)/8)+hands.getSize().h) -hands.getSize().h/2;
            hand2AnimOffsetY = -(int( ( abs((frameProgression*(-3*hands.getSize().h)/8)+hands.getSize().h) -hands.getSize().h/2)) +2);
            foot1AnimOffsetX = int( (abs((frameProgression*(-3*feet.getSize().w)/8)+feet.getSize().w) -feet.getSize().w/2)/3 );
            foot2AnimOffsetX = -(int( (abs((frameProgression*(-3*feet.getSize().w)/8)+feet.getSize().w) -feet.getSize().w/2)/3) +2);
            break;

        case ATTACKING:
            //16 frames
            ++frameProgression; // half length
            //8frames
            if(frameProgression > 15){
                frameProgression = 0;
                stateIsLocked = false;
                characterState = STILL;
            }
            //y = abs( b.h+10/8 * x-8 ) [+ b.h +10]
            hand1AnimOffsetY = 0;//abs(  ((body.getSize().h)/8) * (frameProgression-8)  ) -body.getSize().h; //down,right
            hand2AnimOffsetY = 0;//(-1*hand1AnimOffsetY) - body.getSize().h ; //up, left
            //foot2AnimOffsetY = ;
            break;

        case EQUIPING:
            //20 frames
            if(frameProgression > 19){
                frameProgression = 0;
                stateIsLocked = false;
                characterState = STILL;
            }
            break;

        default:
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unknown character state. Uninitiated?" << endl;
            characterState = STILL;
    }


    switch(facing){
        default:
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unkown direction. Uninitiated Character::facing?" << endl;
        case DOWN:
            switch(characterState){
                case STILL:
                default:
                    body.draw(dest, x, y+head.getSize().h); //body
                    feet.draw(dest, x, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) );  //left foot
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) ); //right foot
                    head.draw(dest, x, y);    //head
                    face.draw(dest, x, y);    //face
                    hands.draw(dest, x -(2*hands.getSize().w/3), y+head.getSize().h);   //left hand
                    hands.draw(dest, x +body.getSize().w -(hands.getSize().w/3), y+head.getSize().h);   //right hand
                    break;

                case MOVING:
                    body.draw(dest, x, y+head.getSize().h); //body
                    feet.draw(dest, x,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) +hand2AnimOffsetY/3);  //left foot
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) +hand1AnimOffsetY/3 ); //right foot
                    head.draw(dest, x, y);    //head
                    face.draw(dest, x, y);    //face
                    hands.draw(dest, x -(2*hands.getSize().w/3), y+head.getSize().h +hand1AnimOffsetY);   //left hand
                    hands.draw(dest, x +body.getSize().w -(hands.getSize().w/3), y+head.getSize().h +hand2AnimOffsetY);   //right hand
                    break;

                case ATTACKING:
                    {
                        //center hands to body
                        int hand1X = x; //left
                        int hand2X = x; //right
                        int handY = y + head.getSize().h;
                        if(hands.getSize().w*2+1 > body.getSize().w){
                            hand1X += ((body.getSize().w - hands.getSize().w*2+1) - (hands.getSize().w+2)) /2 ;
                            hand2X = hand1X + hands.getSize().w+1;
                        } else {
                            hand1X += -((hands.getSize().w*2+1 - body.getSize().w)/2) ;
                            hand2X = hand1X + hands.getSize().w+1;
                        }
                        if(hands.getSize().h > body.getSize().h){
                            handY += -((hands.getSize().h - body.getSize().h)/2);
                        } else {
                            handY += ((body.getSize().h - hands.getSize().h) - (hands.getSize().h/2)) /2;
                        }
                        body.draw(dest, x, y+head.getSize().h); //body
                        head.draw(dest, x, y);    //head
                        face.draw(dest, x, y);    //face
                        feet.draw(dest, x, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) -hand2AnimOffsetY/16);  //left foot
                        feet.draw(dest, x +body.getSize().w -hands.getSize().w, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) +hand2AnimOffsetY/8); //right foot
                        hands.draw(dest, hand1X,  handY + hand2AnimOffsetY);   //left hand
                        hands.draw(dest, hand2X,  handY + hand2AnimOffsetY);   //right hand
                    }
                    break;

                case EQUIPING:
                    break;
            }
            break;



        case UP:
            switch(characterState){
                case STILL:
                default:
                    hands.draw(dest, x -(2*hands.getSize().w/3), y+head.getSize().h);   //left hand
                    hands.draw(dest, x +body.getSize().w -(hands.getSize().w/3), y+head.getSize().h);   //right hand
                    feet.draw(dest, x, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) );  //left foot
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) ); //right foot
                    head.draw(dest, x, y); //head
                    body.draw(dest, x, y+head.getSize().h); //body
                    break;

                case MOVING:
                    hands.draw(dest, x -(2*hands.getSize().w/3), y+head.getSize().h +hand1AnimOffsetY); //left hand
                    hands.draw(dest, x +body.getSize().w -(hands.getSize().w/3), y+head.getSize().h +hand2AnimOffsetY);   //right hand
                    feet.draw(dest, x,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) +hand2AnimOffsetY/3 );  //left foot
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) +(hand1AnimOffsetY/3) ); //right foot
                    head.draw(dest, x, y); //head
                    body.draw(dest, x, y+head.getSize().h); //body
                    break;

                case ATTACKING:
                    {
                        //center hands to body
                        int hand1X = x; //left
                        int hand2X = x; //right
                        int handY = y + head.getSize().h;
                        if(hands.getSize().w*2+1 > body.getSize().w){
                            hand1X += ((body.getSize().w - hands.getSize().w*2+1) - (hands.getSize().w+2)) /2 ;
                            hand2X = hand1X + hands.getSize().w+1;
                        } else {
                            hand1X += -((hands.getSize().w*2+1 - body.getSize().w)/2) ;
                            hand2X = hand1X + hands.getSize().w+1;
                        }
                        if(hands.getSize().h > body.getSize().h){
                            handY += -((hands.getSize().h - body.getSize().h)/2);
                        } else {
                            handY += ((body.getSize().h - hands.getSize().h) - (hands.getSize().h/2)) /2;
                        }
                        hands.draw(dest, hand1X,  handY + hand1AnimOffsetY);   //left hand
                        hands.draw(dest, hand2X,  handY + hand1AnimOffsetY);   //right hand
                        feet.draw(dest, x, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) -hand2AnimOffsetY/8);  //left foot
                        feet.draw(dest, x +body.getSize().w -hands.getSize().w, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) +hand2AnimOffsetY/16); //right foot
                        head.draw(dest, x, y); //head
                        body.draw(dest, x, y+head.getSize().h); //body

                    }
                    break;

                case EQUIPING:
                    break;
            }
            break;




        case LEFT:
            switch(characterState){
                case STILL:
                default:
                    hands.draw(dest, x -(2*hands.getSize().w/3), y+head.getSize().h);   //hand on left
                    body.draw(dest, x, y+head.getSize().h); //body
                    head.draw(dest, x, y);    //head
                    face.draw(dest, x-2, y);  //face
                    feet.draw(dest, x, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) );  //foot on left
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) ); //foot on right
                    hands.draw(dest, x +(3*hands.getSize().w/4), y+head.getSize().h +(hands.getSize().h/4));   //hand on right
                    break;

                case MOVING:
                    hands.draw(dest, x -(2*hands.getSize().w/3), y+head.getSize().h +hand1AnimOffsetY);   //hand on left
                    body.draw(dest, x, y+head.getSize().h); //body
                    head.draw(dest, x, y);    //head
                    face.draw(dest, x-2, y);  //face
                    feet.draw(dest, x +foot1AnimOffsetX, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) );  //foot on left
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w +foot2AnimOffsetX, y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) ); //foot on right
                    hands.draw(dest, x +(3*hands.getSize().w/4), y+head.getSize().h +(hands.getSize().h/4) +hand2AnimOffsetY);   //hand on right
                    break;

                case ATTACKING:
                    break;
                case EQUIPING:
                    break;
            }
            break;



        case RIGHT:
            switch(characterState){
                case STILL:
                default:
                    hands.draw(dest, x +body.getSize().w -(hands.getSize().w/3), y+head.getSize().h);   //hand on right
                    body.draw(dest, x, y+head.getSize().h); //body
                    head.draw(dest, x, y);    //head
                    face.draw(dest, x+2, y);  //face
                    feet.draw(dest, x,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) );  //foot on left
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) ); //foot on right
                    hands.draw(dest, x +(3*hands.getSize().w/4), y+head.getSize().h +(hands.getSize().h/4));   //hand on left
                    break;

                case MOVING:
                    hands.draw(dest, x +body.getSize().w -(hands.getSize().w/3), y+head.getSize().h +hand1AnimOffsetY);   //hand on right
                    body.draw(dest, x,  y+head.getSize().h); //body
                    head.draw(dest, x,  y);    //head
                    face.draw(dest, x+2,  y);  //face
                    feet.draw(dest, x +foot2AnimOffsetX,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) );  //foot on left
                    feet.draw(dest, x +body.getSize().w -hands.getSize().w +foot1AnimOffsetX,  y+head.getSize().h +body.getSize().h -(3*feet.getSize().h/4) ); //foot on right
                    hands.draw(dest, x +(3*hands.getSize().w/4), y+head.getSize().h +(hands.getSize().h/4) +hand2AnimOffsetY);   //hand on left
                    break;

                case ATTACKING:
                    break;

                case EQUIPING:
                    break;
            }
            break;
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

        if( !head.savePacket(packet) ){ return 0; }
        if( !face.savePacket(packet) ){ return 0; }
        if( !body.savePacket(packet) ){ return 0; }
        if( !hands.savePacket(packet) ){ return 0; }
        if( !feet.savePacket(packet) ){ return 0; }

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
        if(packet.readDone()){ return 0; }
        if( !validDirection(tmp) ){ return 0;}
        facing = static_cast<Character::Directions>(tmp);

        packet >> x;
        if(packet.readDone()){ return 0; }

        packet >> y;
        if(packet.readDone()){ return 0; }

        packet >> xVelMax;
        if(packet.readDone()){ return 0; }

        packet >> yVelMax;
        if(packet.readDone()){ return 0; }


        if( !stats.loadPacket(packet)  ||  !inventory.load(packet)  ||
            !head.loadPacket(packet)   ||  !face.loadPacket(packet)       ||
            !body.loadPacket(packet)   ||  !hands.loadPacket(packet)      ||
            !feet.loadPacket(packet) )
        {
            return 0;
        }

        return true;
    }
}
