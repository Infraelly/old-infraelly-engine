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


#include "TSpriteIcon.hpp"

#include "InfraellyImageLoader.hpp"


TSpriteIcon::TSpriteIcon() :
    sprite_(NULL)
{
    setFrameSize(1);
    setFocusable(0);
    setVisible(1);
}

TSpriteIcon::TSpriteIcon(TSprite *tsp) :
    sprite_(tsp)
{
    setFrameSize(1);
    setFocusable(0);
    setVisible(1);
    updateImage();
    adjustSize();
}

TSpriteIcon::~TSpriteIcon(){ clean(); }



TSprite *TSpriteIcon::getTSprite()const{ return sprite_; }

void TSpriteIcon::setTSprite(TSprite *newTsp){
    sprite_ = newTsp;
    updateImage();
}



void TSpriteIcon::adjustSize(){
    if( sprite_ != NULL ){
        //resize
        setSize( sprite_->getWidth(), sprite_->getHeight() );
    }
}

void TSpriteIcon::clean(){
    //clean cached frames
    if( !frames_.empty() ){
        for( size_t i = 0; i < frames_.size(); ++i ){
            delete frames_.at(i);
            frames_.at(i) = NULL;
        }
        frames_.clear();
    }
}


void TSpriteIcon::updateImage(){
    if( sprite_ != NULL ){
        clean();

        InfraellyImageLoader loader;

        //create new cache of frames
        for( size_t i = 0; i < sprite_->getFrameCount(); ++i ){
            frames_.push_back(  loader.load( sprite_->getFrame(i) )  );
        }
    }
}



void TSpriteIcon::draw(gcn::Graphics *graphics){
    if( (sprite_ != NULL) && (!frames_.empty()) ){
        setImage( frames_.at(sprite_->getFrameProgression()) );
    }
    gcn::Icon::draw(graphics);
}

