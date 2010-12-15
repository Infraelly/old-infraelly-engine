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


#include "PasswordField.hpp"

using namespace gcn;

PasswordField::PasswordField(){
    adjustHeight();
    setWidth(100);
}

PasswordField::PasswordField(const std::string& text){
    setText(text);
    adjustHeight();
    setWidth(100);
}

void PasswordField::draw(Graphics* graphics){
    //Taken from \widgets\textfield.cpp

    Color faceColor = getBaseColor();
    Color highlightColor, shadowColor;
    int alpha = getBaseColor().a;
    highlightColor = faceColor + 0x303030;
    highlightColor.a = alpha;
    shadowColor = faceColor - 0x303030;
    shadowColor.a = alpha;

    // Draw a border.
    graphics->setColor(shadowColor);
    graphics->drawLine(0, 0, getWidth() - 1, 0);
    graphics->drawLine(0, 1, 0, getHeight() - 2);
    graphics->setColor(highlightColor);
    graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
    graphics->drawLine(0, getHeight() - 1, getWidth() - 1, getHeight() - 1);

    // Push a clip area so the other drawings don't need to worry
    // about the border.
    graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 2));

    graphics->setColor(getBackgroundColor());
    graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

    if (isFocused())
    {
        drawCaret(graphics, getFont()->getWidth(mText.substr(0, mCaretPosition)) - mXScroll);
    }

    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());

    //replace text with *
    std::string secretText = mText;
    for( std::string::size_type i = 0; i < secretText.length(); ++i ){
        secretText.at(i) = 'o';
    }
    graphics->drawText(secretText, 1 - mXScroll, 1);

    graphics->popClipArea();
}
