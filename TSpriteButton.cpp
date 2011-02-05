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


#include "TSpriteButton.hpp"
#include "Screen.hpp"
#include "InfraellyImageLoader.hpp"

TSpriteButton::TSpriteButton() :
    image_(NULL)
{}

TSpriteButton::~TSpriteButton(){
    delete image_;
}

void TSpriteButton::setTSprite(const TSprite &sprite){
    sprite_ = sprite;
    setSize( sprite_.getWidth(), sprite_.getHeight() );
}

void TSpriteButton::clearSprite(){
    sprite_.unload();
}

void TSpriteButton::draw(gcn::Graphics* graphics){
    delete image_;
    image_ = InfraellyImageLoader().load( sprite_.getFrame(sprite_.getFrameProgression()) );
    setImage(image_);

// from guichan/widgets/button.c
    int textX;
    int textY = getHeight() / 2 - getFont()->getHeight() / 2;

    switch (getAlignment())
    {
      case gcn::Graphics::LEFT:
          textX = mSpacing;
          break;
      case gcn::Graphics::CENTER:
          textX = getWidth() / 2;
          break;
      case gcn::Graphics::RIGHT:
          textX = getWidth() - mSpacing;
          break;
      default:
          throw GCN_EXCEPTION("Unknown alignment.");
    }

    graphics->setFont(getFont());

    if (isPressed())
    {
        graphics->drawText(getCaption(), textX + 1, textY + 1, getAlignment());
    }
    else
    {
        graphics->drawText(getCaption(), textX, textY, getAlignment());

        if (isFocused())
        {
            graphics->drawRectangle(gcn::Rectangle(2, 2, getWidth() - 4,
                                              getHeight() - 4));
        }
    }
}
