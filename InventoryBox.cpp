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


#include "InventoryBox.hpp"

#include "Inventory.hpp"
#include "InfraellyScroller.hpp"
#include "Tile.hpp"
#include "InfraellyImageLoader.hpp"


InventoryBox::InventoryBox() :
    inventory_(NULL),
    itemContainer_(new gcn::Container),
    scroller_(new InfraellyScroller(itemContainer_)),
    junkBtn_(new gcn::ImageButton),
    dropBtn_(new gcn::ImageButton),
    equipsBtn_(new gcn::ImageButton),
    weightLbl_(new gcn::Label("weight: N/A")),
    lastSize_(0)
{
    setCaption("Inventory");
    setBackgroundColor(gcn::Color(240,240,255));
    setBaseColor(gcn::Color(200,200,255));

    scroller_->setSize(200, 192);
    scroller_->setScrollbarWidth(4);
    scroller_->setScrollPolicy( gcn::ScrollArea::SHOW_AUTO, gcn::ScrollArea::SHOW_ALWAYS );
    scroller_->setBaseColor(gcn::Color(255,240,255));
    scroller_->setBackgroundColor(gcn::Color(245,230,245));
    scroller_->setForegroundColor(gcn::Color(200,180,200));

    itemContainer_->setPosition(0, 0);
    itemContainer_->setSize(scroller_->getWidth() - scroller_->getScrollbarWidth(), 192);
    itemContainer_->setBaseColor(gcn::Color(255,255,255,0));

    junkBtn_->setSize(64, 64);
    junkBtn_->setBaseColor(gcn::Color(255,255,255,0));
    junkBtn_->setFrameSize(0);
    junkBtn_->setFocusable(false);

    dropBtn_->setSize(64, 64);
    dropBtn_->setBaseColor(gcn::Color(255,255,255,0));
    dropBtn_->setFrameSize(0);
    dropBtn_->setFocusable(false);

    equipsBtn_->setSize(64, 64);
    equipsBtn_->setBaseColor(gcn::Color(255,255,255,0));
    equipsBtn_->setFrameSize(0);
    equipsBtn_->setFocusable(false);

    junkBtn_->setPosition(200,0);
    dropBtn_->setPosition(200,64);
    equipsBtn_->setPosition(200,128);

    weightLbl_->setPosition(0, 192);
    weightLbl_->setAlignment( gcn::Graphics::CENTER );

    Tileset buttons("images/inventoryButtons.png", "Inventory Buttons", 64, 64);
    Tile junkSprite, dropSprite, equipSprite;
    junkSprite.setSource(&buttons, 0,0);
    dropSprite.setSource(&buttons, 1,0);
    equipSprite.setSource(&buttons, 2,0);

    InfraellyImageLoader loader;
    junkImage_ = loader.load(junkSprite);
    dropImage_ = loader.load(dropSprite);
    equipImage_ = loader.load(equipSprite);

    junkBtn_->setImage(junkImage_);
    dropBtn_->setImage(dropImage_);
    equipsBtn_->setImage(equipImage_);

    add(scroller_);
    add(junkBtn_);
    add(dropBtn_);
    add(equipsBtn_);
    add(weightLbl_);

    resizeToContent();
    weightLbl_->setWidth( getWidth() );
};

InventoryBox::~InventoryBox(){
    delete itemContainer_;
    delete scroller_;
    delete junkBtn_;
    delete dropBtn_;
    delete equipsBtn_;
    delete weightLbl_;

    delete junkImage_;
    delete dropImage_;
    delete equipImage_;

    for(int i = 0; i < itemHolders_.size(); ++i){
        if( itemHolders_[i] != NULL ){
            delete itemHolders_[i];
            itemHolders_[i] = NULL;
        }
    }
}

Inventory *InventoryBox::getInventory()const{ return inventory_; }

void InventoryBox::setInventory(Inventory *invent){
    inventory_ = invent;

    #ifdef DEBUG
        Item item;
        item.load("items/test");
        for(int i = 0; i < 10; ++i){
            inventory_->gainItem(item);
        }
    #endif
 }

void InventoryBox::logic(){
    InfraellyWindow::logic();

    const float tileSize = 32;

    if( inventory_ ){
        // if there's been a change in size, we need to update
        if( lastSize_ != inventory_->size() ){
            lastSize_ = inventory_->size();
            //update holders
            // make sure we have the right size
            if( itemHolders_.size() < inventory_->getMaxSize() ){
                int extraElements = inventory_->getMaxSize() - itemHolders_.size();
                while( inventory_->getMaxSize() - itemHolders_.size() ){
                    itemHolders_.push_back(new TSpriteButton);
                    itemHolders_.back()->setFocusable(true);
                    itemHolders_.back()->setVisible(true);
                    itemHolders_.back()->setFrameSize(1);
                    itemContainer_->add(itemHolders_.back());
                }

                //position the elements in the container
                int elementsPerRow = (scroller_->getWidth()-scroller_->getScrollbarWidth())/tileSize -.4;
                int spacing = ((scroller_->getWidth()-scroller_->getScrollbarWidth())/(float)(2.5*elementsPerRow) -.4);
                int elementNumber, col, row;
                for(int i = 0; i < extraElements; ++i ){
                    elementNumber = itemHolders_.size()-(extraElements-i);
                    col = elementNumber%elementsPerRow;
                    row = (elementNumber/(float)elementsPerRow);
                    //int x = (col*tileSize) + ((col+1)*spacing);
                    //int y = (row*tileSize) + ((row+1)*spacing);
                    itemHolders_[elementNumber]->setX( (col*tileSize) + ((col+1)*spacing) );
                    itemHolders_[elementNumber]->setY( (row*tileSize) + ((row+1)*spacing) );
                }
                row = (elementNumber/(float)elementsPerRow) +1;
                itemContainer_->setHeight( (row*tileSize) + (row*spacing) + spacing );
            }

            //clear out images
            for(int i = 0; i < itemHolders_.size(); ++i){
                if( itemHolders_[i] != NULL ){
                    itemHolders_[i]->clearSprite();
                }
            }

            //refresh images
            for(int i = 0; i < inventory_->size(); ++i){
                itemHolders_[i]->setTSprite( inventory_->at(i).getTile() );
            }

            // make right size
            for(int i = 0; i < itemHolders_.size(); ++i){
                if( itemHolders_[i] != NULL ){
                    itemHolders_[i]->setSize(tileSize, tileSize);
                }
            }
        }
    }
}
