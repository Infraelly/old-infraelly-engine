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


//  Item.cpp

#include <iostream>
#include "Item.hpp"
#include "INFPacket.hpp"
#include "globalFunc.hpp"
#include "dataPacks.hpp"

using namespace std;


bool validItemType(int itemType){
    return ( (itemType==Item::FOOD)  || (itemType==Item::WEAPON) ||
             (itemType==Item::AMOUR) || (itemType==Item::QUEST)  ||
             (itemType==Item::MISC) );
}

bool validItemGender(int itemGender){
    return ( (itemGender==Item::MALE)  || (itemGender==Item::FEMALE) ||
             (itemGender==Item::ASEXUAL) );
}

string ItemTypeToString(const enum Item::ItemType& type){
    switch(type){
        case Item::FOOD: return "Food";
        case Item::WEAPON: return "Weapon";
        case Item::AMOUR: return "Amour";
        case Item::QUEST: return "Quest";
        case Item::MISC: return "Misc";
        default: return "Unknown";
    }
    return "Unknown";
}

enum Item::ItemType stringToItemType(std::string itemType){
    itemType = upperCase(itemType);

    if(itemType == "FOOD"){ return Item::FOOD; } else
    if(itemType == "WEAPON"){ return Item::WEAPON; } else
    if(itemType == "AMOUR"){ return Item::AMOUR; } else
    if(itemType == "QUEST"){ return Item::QUEST; } else
    if(itemType == "MISC"){ return Item::MISC; } else {
        return Item::MISC;
    }

    return Item::MISC;
}





Item::Item(){
    init();
}

Item::Item(const std::string& filename){
    init();
    load(correctFilepath(filename));
}

Item::~Item(){}

void Item::init(){
    name_ = "Mystery Item";
    desc_ = "Wonder what this does?";
    durability_ = 1;
    maxDurability_ = durability_;
    dropable_ = true;
    itemType_ = MISC;
    itemGender_ = ASEXUAL;

    animFilename_ = "";
}


const std::string& Item::getName()const{ return name_; }

const std::string& Item::getDescription()const{ return desc_; }

int Item::getDurability()const{ return durability_; }

int Item::getMaxDurability()const{ return maxDurability_; }

bool Item::isDropable()const{ return dropable_; }

bool Item::isStackable()const{ return stackable_; }

enum Item::ItemType Item::getItemType()const{ return itemType_; }

enum Item::ItemGender Item::getGender()const{ return itemGender_; }

const TSprite& Item::getTile()const{ return image_; }

const TSprite& Item::getSprite()const{ return sprite_; }

const std::string& Item::getAnimFilename()const{ return animFilename_; }

const Animation& Item::getAnim()const{ return anim_; }




void Item::setName(const std::string& itemName){ name_ = itemName; }

void Item::setDescription(const std::string& newDesc){ desc_ = newDesc; }

void Item::setDurability(int newDurability){ durability_ = newDurability; }

void Item::setMaxDurability(int newMaxDuability){ maxDurability_ = newMaxDuability; }

void Item::setDropable(bool val){ dropable_ = val; }

void Item::setStackable(bool val){ stackable_ = val; }

void Item::setItemType(enum ItemType newType){ itemType_ = newType; }

void Item::setGender(enum ItemGender newGender){ itemGender_ = newGender; }

void Item::setTile(const TSprite& newTile){ image_ = newTile; }

void Item::setSprite(const TSprite& newSprite){
    sprite_ = newSprite;
    anim_.setImage(sprite_);
}

void Item::setAnimFilename(const std::string& newAnimFilename){
    animFilename_ = newAnimFilename;
    anim_.load(animFilename_);
}

void Item::setAnim(const Animation& newAnim){
    anim_ = newAnim;
    anim_.setImage(sprite_);
}




void Item::minusDurability(){
    if(maxDurability_ > 0){
        if(durability_ > 0){
            --durability_;
        };
        if(durability_ == 0){
            if( (itemType_ == WEAPON) || (itemType_ == AMOUR) ){
                name_ = "Broken " + name_;
            } else {
                name_ = "Used " + name_;
            }
        }
    }
}




//save/load
bool Item::save(const std::string& filename)const{
    //open file for reading in ascii mode
    std::string fixedFn = correctFilepath(filename);
    ofstream file(fixedFn.c_str());
    if (!file.is_open()){
        file.close();
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to open file (ofstream): " << fixedFn << endl;
        return 0;
    } else {
        inp::INFPacket packet;

        if( savePacket(packet) ){
            file.write( (char*)packet.getBuf(), packet.getLength() );
            file.close();
            return true;
        } else {
            file.close();
            return false;
        }
    }
}

bool Item::savePacket( inp::INFPacket& packet )const{
    packet << revision_;
    packet << name_;
    packet << desc_;
    packet << durability_;
    packet << maxDurability_;
    packet << dropable_;
    packet << stackable_;
    packet << static_cast<int>(itemType_);
    packet << static_cast<int>(itemGender_);

    packet << image_.getFilename();
    packet << sprite_.getFilename();
    packet << animFilename_;

    //actions
    packet << (int) actions_.size();

    for( unsigned int i = 0; i < actions_.size(); ++i ){
        packet << actions_.at(i);
    }

    return true;
}


bool Item::load(const std::string& filename){
    //open file for reading
    std::string fixedFn = correctFilepath(filename);
    ifstream file(correctFilepath(fixedFn).c_str(), ios::binary);
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
        return loadPacket(packet);
    }
}

bool Item::loadPacket( inp::INFPacket& packet ){
    packet >> name_;
    packet >> desc_;

    packet >> durability_;
    packet >> maxDurability_;
    packet >> dropable_;
    packet >> stackable_;

    int type;
    packet >> type;
    if( validItemType(type) ){
        itemType_ = static_cast<enum ItemType>(type);
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Invalid item type" << endl;
        return false;
    }

    packet >> type;
    if( validItemGender(type) ){
        itemGender_ = static_cast<enum ItemGender>(type);
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Invalid item gender" << endl;
        return false;
    }


    std::string spriteFilename = "";
    packet >> spriteFilename;
    if( !image_.loadXml(spriteFilename) ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to load Item image" << endl;
    }

    packet >> spriteFilename;
    if( spriteFilename != "" ){
        if( !sprite_.loadXml(spriteFilename) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to load Item sprite" << endl;
        }
    }

    packet >> animFilename_;
    if( animFilename_ != "" ){
        if( !anim_.load(animFilename_) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to load Item animation" << endl;
        }
    }
    anim_.setImage(sprite_);


    //actions
    std::string action = "";
    int numActions = 0;
    packet >> numActions;

    actions_.clear();
    for( unsigned int i = 0; i < actions_.size(); ++i ){
        packet >> action;
        actions_.push_back(action);
    }

    return true;
}

bool Item::load( const ResourcePack& pack, const std::string& filepath ){
    ResourceFile rcf;
    packs::mainPack.getResource( filepath, rcf );

    inp::INFPacket packet;
    packet.readBuf( &packs::mainPack.getRawData()[rcf.start], rcf.size );
    return loadPacket(packet);
}


//drawers
void Item::drawTile(SDL_Surface *dest, int x, int y){
    image_.draw(dest, x, y);
}

void Item::drawAnim(SDL_Surface *dest, int x, int y, bool freeze){
    if( !freeze ){
        anim_.draw(dest, x, y);
    } else {
        sprite_.draw(dest, x, y);
    }
}
