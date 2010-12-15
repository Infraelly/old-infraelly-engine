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


//InfraellyWindow.cpp

#include "Screen.hpp"
#include "InfraellyWindow.hpp"
#include "fonts.hpp"


InfraellyWindow::InfraellyWindow() :
    controlsAttached_(true),
    collapsed_(false),
    oldHeight_(0),
    collapseBtn_(new gcn::Button("o")),
    hideBtn_(new gcn::Button("X"))
{
    setFont(font::guiTitleFont.at(font::stdGuiTitleFontSize));

    collapseBtn_->setFocusable(false);
    collapseBtn_->setTabInEnabled(false);
    collapseBtn_->setTabOutEnabled(true);
    collapseBtn_->setBaseColor(gcn::Color(0x0000AA));
    collapseBtn_->setForegroundColor(0xFFFFFF);
    collapseBtn_->setFrameSize(0);
    collapseBtn_->setSize(10, 10);

    hideBtn_->setFocusable(false);
    hideBtn_->setTabInEnabled(false);
    hideBtn_->setTabOutEnabled(true);
    hideBtn_->setBaseColor(gcn::Color(0x5555AA));
    hideBtn_->setForegroundColor(0xFFFFFF);
    hideBtn_->setFrameSize(0);
    hideBtn_->setSize(20, 10);

    add(collapseBtn_);
    add(hideBtn_);

    collapseBtn_->addMouseListener(this);
    hideBtn_->addMouseListener(this);
}

InfraellyWindow::~InfraellyWindow(){
    delete hideBtn_;
    delete collapseBtn_;
}




void InfraellyWindow::resizeToContent(){
    gcn::Window::resizeToContent();
    repositionControls(true);
    oldHeight_ = getHeight();
}

void InfraellyWindow::repositionControls(bool adjustHeight){
    if( controlsAttached_ ){
        //set the width of the collapse button
        collapseBtn_->setWidth( getWidth() - hideBtn_->getWidth() - 2*getFrameSize() );

        if( !collapsed_ ){
            //Controlls at bottom
            if( adjustHeight ){
                collapseBtn_->setPosition( 0, getHeight() - getTitleBarHeight() - 2*getFrameSize() );
            } else {
                collapseBtn_->setPosition( 0, getHeight() - getTitleBarHeight() - 2*getFrameSize() - collapseBtn_->getHeight());
            }
            hideBtn_->setPosition( collapseBtn_->getX()+collapseBtn_->getWidth(), collapseBtn_->getY() );
        } else {
            //Controlls at top
            collapseBtn_->setPosition( 0, 0 );
            hideBtn_->setPosition( collapseBtn_->getX()+collapseBtn_->getWidth(), 0 );
        }

        if( adjustHeight ){
            setHeight( collapseBtn_->getY() + collapseBtn_->getHeight() + getTitleBarHeight() + 2*getFrameSize() );
        }

        //make sure controlls appear on top of eveyrthing
        moveToTop(collapseBtn_);
        moveToTop(hideBtn_);
    }
}



int InfraellyWindow::getViewControllsHeight()const{ return collapseBtn_->getHeight(); }



void InfraellyWindow::removeViewControls(){
    controlsAttached_ = false;
    collapseBtn_->setVisible(false);
    hideBtn_->setVisible(false);
    resizeToContent();
}

void InfraellyWindow::addViewControls(){
    if( !controlsAttached_ ){
        controlsAttached_ = true;
        collapseBtn_->setVisible(true);
        hideBtn_->setVisible(true);
        resizeToContent();
    }
}

void InfraellyWindow::mouseClicked(gcn::MouseEvent& mouseEvent){
    gcn::Window::mouseClicked(mouseEvent);
    if( mouseEvent.isConsumed() ){ return; };

    //if click was from closebutton
    if( mouseEvent.getSource() == hideBtn_ ){
        setVisible(0);
        mouseEvent.consume();
    } else
    //if click was from collap
    if( mouseEvent.getSource() == collapseBtn_ ){
        collapsed_ = !collapsed_; //toggles collapses state
        mouseEvent.consume();
        if( !collapsed_ ){
            setHeight(oldHeight_);
            repositionControls();
        } else {
            oldHeight_ = getHeight();
            repositionControls(true);
        }
    }
}

void InfraellyWindow::mouseReleased(gcn::MouseEvent& mouseEvent){
    gcn::Window::mouseReleased(mouseEvent);
    if( mouseEvent.isConsumed() ){ return; };

    //bound to screen
    int width = getWidth()+getFrameSize();
    int height = getHeight()+getTitleBarHeight()+getFrameSize();
    if( getX()+width*.25 > Screen::getWidth() ){
        setX( Screen::getWidth() - width*.25 );
    }
    if( getX()+width*.75 < 0 ){
        setX( -width*.75 );
    }
    if( getY() < 0 ){
        setY( 0 );
    }
    if( (getY()+height*.25) > Screen::getHeight() ){
        setY( Screen::getHeight() -height*.25 );
    }
}


void InfraellyWindow::draw(gcn::Graphics* graphics){
    // this is a modified version of gcn::Window::draw(Graphics *graphics)
    // it draws the background color of the window inside the children area


    gcn::Color faceColor = getBaseColor();
    gcn::Color highlightColor, shadowColor;
    int alpha = getBaseColor().a;
    //int width = getWidth() + getFrameSize() * 2 - 1;
    //int height = getHeight() + getFrameSize() * 2 - 1;
    highlightColor = faceColor + 0x303030;
    highlightColor.a = alpha;
    shadowColor = faceColor - 0x303030;
    shadowColor.a = alpha;

    gcn::Rectangle d = getChildrenArea();

    // Fill the background around the content
    graphics->setColor(faceColor);
    // Fill top
    graphics->fillRectangle(gcn::Rectangle(0,0,getWidth(),d.y - 1));
    // Fill left
    graphics->fillRectangle(gcn::Rectangle(0,d.y - 1, d.x - 1, getHeight() - d.y + 1));
    // Fill right
    graphics->fillRectangle(gcn::Rectangle(d.x + d.width + 1,
                                        d.y - 1,
                                        getWidth() - d.x - d.width - 1,
                                        getHeight() - d.y + 1));
    // Fill bottom
    graphics->fillRectangle(gcn::Rectangle(d.x - 1,
                                        d.y + d.height + 1,
                                        d.width + 2,
                                        getHeight() - d.height - d.y - 1));

    if (isOpaque()){
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(d);
    }

    // Construct a rectangle one pixel bigger than the content
    d.x -= 1;
    d.y -= 1;
    d.width += 2;
    d.height += 2;

    // Draw a border around the content
    graphics->setColor(shadowColor);
    // Top line
    graphics->drawLine(d.x,
                       d.y,
                       d.x + d.width - 2,
                       d.y);

    // Left line
    graphics->drawLine(d.x,
                        d.y + 1,
                        d.x,
                        d.y + d.height - 1);

    graphics->setColor(highlightColor);
    // Right line
    graphics->drawLine(d.x + d.width - 1,
                       d.y,
                       d.x + d.width - 1,
                       d.y + d.height - 2);
    // Bottom line
    graphics->drawLine(d.x + 1,
                       d.y + d.height - 1,
                       d.x + d.width - 1,
                       d.y + d.height - 1);

    drawChildren(graphics);

    int textX;
    int textY;

    textY = ((int)getTitleBarHeight() - getFont()->getHeight()) / 2;

    switch (getAlignment())
    {
      case gcn::Graphics::LEFT:
          textX = 4;
          break;
      case gcn::Graphics::CENTER:
          textX = getWidth() / 2;
          break;
      case gcn::Graphics::RIGHT:
          textX = getWidth() - 4;
          break;
      default:
          throw GCN_EXCEPTION("Unknown alignment.");
    }

    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());
    graphics->pushClipArea(gcn::Rectangle(0, 0, getWidth(), getTitleBarHeight() - 1));
    graphics->drawText(getCaption(), textX, textY, getAlignment());

    graphics->popClipArea();
}
