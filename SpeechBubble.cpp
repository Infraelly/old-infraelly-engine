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


//  SpeechBubble.cpp

#include "SpeechBubble.hpp"

//drawSurfacePPAlpha()
//drawText()
#include "globalFunc.hpp"
#include "fonts.hpp"
#include "colours.hpp"

#include "Tileset.hpp"

#include "dataPacks.hpp"
//#include "caches.hpp"


bool SpeechBubble::firstInstance_ = true;
Tileset SpeechBubble::tileset_;
Tile SpeechBubble::top_;
Tile SpeechBubble::bottom_;
Tile SpeechBubble::left_;
Tile SpeechBubble::right_;
Tile SpeechBubble::center_;
Tile SpeechBubble::TL_;
Tile SpeechBubble::TR_;
Tile SpeechBubble::BL_;
Tile SpeechBubble::BR_;

SpeechBubble::SpeechBubble(const std::string& newText) :
    text_(newText),
    composedImage_(NULL)
{
    if( firstInstance_ ){
        //make load bubble image
        tileset_.load( packs::mainPack, "tilesets/speechbubble.xml" );

        //side images
        top_.setSource( &tileset_, 1, 0 );
        bottom_.setSource( &tileset_, 1, 2 );
        left_.setSource( &tileset_, 0, 1 );
        right_.setSource( &tileset_, 2, 1 );
        //center_, ie background
        center_.setSource( &tileset_, 1, 1 );
        //corner images
        TL_.setSource( &tileset_, 0, 0 );
        TR_.setSource( &tileset_, 2, 0 );
        BL_.setSource( &tileset_, 0, 2 );
        BR_.setSource( &tileset_, 2, 2 );

        firstInstance_ = false;
    }

    makeBubble();

    visibleTimer_.clear();
    visibleTimer_.start();
}

SpeechBubble::SpeechBubble(const SpeechBubble& src){
    if( &src != this ){
        visibleTimer_ = src.visibleTimer_;
        text_ = src.text_;
        composedImage_ = NULL;
    }
}
SpeechBubble& SpeechBubble::operator=(const SpeechBubble& rhs){
    if( &rhs != this ){
        visibleTimer_ = rhs.visibleTimer_;
        text_ = rhs.text_;
        composedImage_ = NULL;
    }
    return *this;
}

SpeechBubble::~SpeechBubble(){
    //destoy image of bubble
    if(composedImage_ != NULL){
        SDL_FreeSurface(composedImage_);
        composedImage_ = NULL;
    }
}


void SpeechBubble::setText(const std::string& newText){
    text_ = newText;
    makeBubble();
    visibleTimer_.clear();
    visibleTimer_.start();
}

const std::string& SpeechBubble::getText()const{ return text_; }


int SpeechBubble::getWidth()const{
    if( composedImage_ != NULL ){
        return composedImage_->w;
    }
    return 0;
}


int SpeechBubble::getHeight()const{
    if( composedImage_ != NULL ){
        return composedImage_->h;
    }
    return 0;
}



void SpeechBubble::draw(SDL_Surface* dest, int x, int y){
    if( visibleTimer_.getTime() > timeout_ ){
        if( visibleTimer_.isTiming() ){ visibleTimer_.stop(); }
        return;
    }
    if( composedImage_ ){
        drawSurfacePPAlpha(composedImage_, dest, x, y);
    } else {
        makeBubble();
    }
}


//This function assemBL_es the bubble frame
//  Then draws the text_.
// No word wrapping for now
void SpeechBubble::makeBubble(){
    //get size of the text_
    int textHeight = getStringHeight(font::mainFont.at(font::stdChatFontSize), text_);
    int textWidth = getStringWidth(font::mainFont.at(font::stdChatFontSize), text_);

    //add the height/width of the frame
    int bubbleHeight = textHeight + top_.getHeight() +
                        center_.getHeight() + bottom_.getHeight();
    int bubbleWidth = textWidth + left_.getWidth() + right_.getWidth();
    if( bubbleWidth<0 || bubbleHeight<0 ){ return; }

    //destoy old image of bubble
    if(composedImage_ != NULL){
        SDL_FreeSurface(composedImage_);
        composedImage_ = NULL;
    }

    //allocate a new surface for the bubble
    composedImage_ = createSurface(bubbleWidth, bubbleHeight, 24, SDL_HWSURFACE);
    //Cant get alpha to do what i want it to... will draw and BL_end with white.
    // then draw to dest using colour key... (FFS!!!)
    SDL_SetAlpha(composedImage_, 0, 0);
    SDL_FillRect(composedImage_, NULL, SDL_MapRGB(composedImage_->format,255,255,255));
    SDL_SetColorKey(composedImage_, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(composedImage_->format,255,255,255));



    //Draw background
    for(int y = top_.getHeight(); y < bubbleHeight-(2*bottom_.getHeight()); y += center_.getHeight()){
        for(int x = left_.getWidth(); x < bubbleWidth-(2*right_.getWidth()); x += center_.getWidth()){
            center_.draw(composedImage_, x, y);
        }
    }


    //Draw the frame onto the surface
    //top_ left_ corner
    TL_.draw(composedImage_, 0, 0);

    //top_ edge
    for(int x = TL_.getWidth(); x < bubbleWidth-TR_.getWidth(); x += top_.getWidth()){
        top_.draw(composedImage_, x, 0);
    }

    //top_ right_ corner
    TR_.draw(composedImage_, bubbleWidth-TL_.getWidth(), 0);

    //left_ edge
    for(int y = TL_.getHeight(); y < bubbleHeight-(2*BL_.getHeight()); y += left_.getHeight()){
        left_.draw(composedImage_, 0, y);
    }

    //right_ edge
    for(int y = TR_.getHeight(); y < bubbleHeight-(2*BR_.getHeight()); y += right_.getHeight()){
        right_.draw(composedImage_, bubbleWidth-right_.getWidth(), y);
    }

    //bottom_ left_ corner
    BL_.draw(composedImage_, 0, bubbleHeight-BL_.getHeight());

    //bottom_ edge
    for(int x = BL_.getWidth(); x < bubbleWidth-BR_.getWidth(); x += bottom_.getWidth()){
        bottom_.draw(composedImage_, x, bubbleHeight-bottom_.getHeight());
    }

    //bottom_ right_ Corner
    BR_.draw(composedImage_, bubbleWidth-BR_.getWidth(), bubbleHeight-BR_.getHeight());


    // patch bits, for when the size of frame is not divisiBL_e by size of tiles
    //checks can be included later, to better efficiency
    for(int x = left_.getWidth(); x < bubbleWidth-(right_.getWidth()+center_.getWidth()); x += center_.getWidth()){
        center_.draw(composedImage_, x, bubbleHeight-bottom_.getHeight()-center_.getHeight() );
    }
    for(int y = top_.getHeight(); y < bubbleHeight-(bottom_.getHeight()+center_.getHeight()); y += center_.getHeight()){
        center_.draw(composedImage_, bubbleWidth-(right_.getWidth()+center_.getWidth()), y );
    }
    center_.draw(composedImage_, bubbleWidth-(right_.getWidth()+center_.getWidth()),
                    bubbleHeight-(BR_.getHeight()+center_.getHeight()));
    //top_.draw(composedImage_, bubbleWidth-TR_.getWidth()-top_.getWidth(), 0);
    left_.draw(composedImage_, 0, bubbleHeight-(BL_.getHeight()+left_.getHeight()));
    right_.draw(composedImage_, bubbleWidth-right_.getWidth(), bubbleHeight-(BR_.getHeight()+right_.getHeight()));



    //Draw text
    drawText( text_, font::mainFont.at(font::stdChatFontSize), colour::black,
              composedImage_, TL_.getWidth(), TL_.getHeight() );
}
