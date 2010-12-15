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


#include "TilesetIcon.hpp"

#include "InfraellyImageLoader.hpp"


TilesetIcon::TilesetIcon() :
    showGrid_(true),
    ts_(NULL),
    tsImage_(NULL)
{ ; }

TilesetIcon::TilesetIcon(Tileset *ts, bool showGrid) :
    showGrid_(showGrid),
    ts_(ts),
    tsImage_(NULL)
{
    updateImage();
}

TilesetIcon::~TilesetIcon(){
    if( tsImage_ != NULL ){
        delete tsImage_;
        tsImage_ = NULL;
    }
}



Tileset *TilesetIcon::getTileset()const{ return ts_; }

void TilesetIcon::setTileset(Tileset *newTs){
    ts_ = newTs;
    updateImage();
}


bool TilesetIcon::isShowGrid()const{ return showGrid_; }

void TilesetIcon::setShowGrid(bool val){ showGrid_ = val; }




void TilesetIcon::updateImage(){
    if( ts_ ){
        //load ts into image
        InfraellyImageLoader imgLder;
        gcn::Image *newImage = imgLder.load(ts_);

        if( newImage != NULL ){
            //check if image already loaded
            if( tsImage_ != NULL ){
                delete tsImage_;
                tsImage_ = NULL;
            }
            tsImage_ = newImage;
            setImage( tsImage_ );
        }
    }
}

void TilesetIcon::drawGrid(gcn::Graphics* graphics){
    //we assume the this is running in a single threaded env
    //make sure there is a specifed tileset to display
    if( ts_ ){
        //set draw coloiur to light grey
        graphics->setColor( gcn::Color( 150,150,150 ) );

        //set the drawable region
        graphics->pushClipArea( gcn::Rectangle(0, 0, getWidth(), getHeight()) );

        //vertical lines
        if( ts_->getTileWidth() > 0 ){
            for( int tmpX = ts_->getXOffset();
                 tmpX < getWidth();
                 tmpX += ts_->getTileWidth() ){
                graphics->drawLine( tmpX, 0, tmpX, getHeight() );
            }
        }
        //horizontal lines
        if( ts_->getTileHeight() > 0 ){
            for( int tmpY = ts_->getYOffset();
                 tmpY < getHeight();
                 tmpY += ts_->getTileHeight() ){
                graphics->drawLine( 0, tmpY, getWidth(), tmpY );
            }
        }
        //unset draw region
        graphics->popClipArea();
    }
}


//void TilesetIcon::refresh(){ updateImage(); }


void TilesetIcon::draw(gcn::Graphics* graphics){
    Icon::draw(graphics);
    if( showGrid_ ){ drawGrid(graphics); }
}
