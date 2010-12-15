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


#ifndef ITEM_HPP
#define ITEM_HPP

//  Item.hpp


#include <string>
#include <fstream>
#include <vector>

#include "Cacheable.hpp"

#include "TSprite.hpp"
#include "Animation.hpp"


/*******************************************************************
Food is food. ie potions?
Weapons and Amour are resident
Quest cant be used or droped
Misc everything else

Max Durability:
Less than 0 for infinte use
    ie -1

Always use minusDurability() at end of use(). This allows more flexibility
*********************************************************************/





class Item : public Cacheable {
    public:
        static const int revision_ = 1;

        //  Categories for items
        enum ItemType{
            FOOD,
            WEAPON,
            AMOUR,
            QUEST,
            MISC
        };

        // required gender to use item
        enum ItemGender{
            MALE,
            FEMALE,
            ASEXUAL
        };


        std::vector<std::string> actions_;   //  Item's instruction set

        Item();
        explicit Item(const std::string& filename);
        ~Item();


        //  Returns the name of the item
        const std::string& getName()const;
        //  Returns description of item
        const std::string& getDescription()const;
        //  Returns the how many uses the item has left
        int getDurability()const;
        //  Returns the maximum number of uses the item can have
        int getMaxDurability()const;
        //  Returns if item is drop-able
        bool isDropable()const;
        //  Retruns if the item is stackable
        bool isStackable()const;
        //  Returns the category the item is in
        enum ItemType getItemType()const;
        //  Returns the required gender to use ite
        enum ItemGender getGender()const;
        //  Returns the thumbnail preview of the item
        const TSprite& getTile()const;
        //  Returns the thumbnail preview of the item
        const TSprite& getSprite()const;
        //  Retruns the animation used
        const std::string& getAnimFilename()const;
        //  Return the animation used
        const Animation& getAnim()const;


        //  Sets the item's name to "itemName"
        void setName(const std::string& itemName);
        //  sets the item's dfescription
        void setDescription(const std::string& newDesc);
        //  Sets how many uses the item has left
        void setDurability(int newDurability);
        //  Sets the maximum number of uses the item can have
        void setMaxDurability(int newMaxDuability);
        //  Sets if the item is drop-able
        void setDropable(bool val);
        //  sets wetather the item is stackable
        void setStackable(bool val);
        //  Sets the item's type to "newType"
        void setItemType(enum ItemType newType);
        // sets the gender required to use the item
        void setGender(enum ItemGender newGender);
        //  Sets the thumbnail preview of the item
        void setTile(const TSprite& newTile);
        //  Sets the thumbnail preview of the item
        void setSprite(const TSprite& newSprite);
        //  Sets the animation used (if saved, permenant change)
        void setAnimFilename(const std::string& newAnimFilename);
        //  Sets the animation used (instant temp change)
        void setAnim(const Animation& newAnim);


        //  This takes 1 from the item's dirabilty
        //  If the item has no Uses left, the name will be prefixed with "Broken" or "Used"
        //  This should always be called after the item is used
        void minusDurability();


        //save item
        bool save( const std::string& filename )const;
        bool savePacket( inp::INFPacket& pack )const;
        // load item
        bool load( const std::string& filename );
        bool loadPacket( inp::INFPacket& pack );
        bool load( const ResourcePack& pack, const std::string& filepath );
        //  Load item file to use as tileset by resource cache
        bool cache_load( const std::string& filename ){ return load(packs::mainPack, filename); }


        //  Draws the Tile (for on map or inventory preview)
        void drawTile(SDL_Surface *dest, int x, int y);
        //  Draws the animation stored in frames (in action)
        //  if  freeze is true, the animation will not be applied to the sprite
        void drawAnim(SDL_Surface *dest, int x, int y, bool freeze = false);



    private:
        void init();

        std::string name_;           //  The name of the item
        std::string desc_;           //  descriptoion of item
        int durability_;             //  How many uses the item has left
        int maxDurability_;          //  The maximum possible uses
        bool dropable_;              //  Whether the item can be droped
        bool stackable_;              //  Wheather the item can be grouped with
                                     //     other identical items
        enum ItemType itemType_;     //  The type of item
        enum ItemGender itemGender_; //  Required gender to use item

        TSprite image_;              //  Static Image  (thumbnail preview)
        TSprite sprite_;             //  Tsprite of item (item equiped)
        std::string animFilename_;   //  Animation file to use
        Animation anim_;             //  The animaton used
};

bool validItemType(int itemType);
bool validItemGender(int itemType);
std::string ItemTypeToString(const enum Item::ItemType& type);
enum Item::ItemType stringToItemType(std::string itemType);




#endif
