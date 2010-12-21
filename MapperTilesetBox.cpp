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


//  MapperTilesetBox.cpp

#include "MapperTilesetBox.hpp"
#include "InfraellyImageLoader.hpp"
#include "caches.hpp"

using namespace std;


MapperTilesetBox::MapperTilesetBox(){
    init();
}

MapperTilesetBox::MapperTilesetBox(AreaMap *newWorld){
    init();
    world = newWorld;
}

MapperTilesetBox::~MapperTilesetBox(){
    delete newTilesetButton;
    delete loadTilesetButton;
    delete tilesetDetailsButton;
    delete closeTilesetButton;
    delete tilesetDropDown;
    delete tilesetImageScroller;
    delete tilesetImage;
}

void MapperTilesetBox::init(){
    //set caption
    setCaption("Tileset");

    //init world
    world = NULL;

    InfraellyImageLoader *loader = dynamic_cast<InfraellyImageLoader *>(gcn::Image::getImageLoader());
    if( loader == NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Image Loader is not an InfraellyImageLoader object" << endl;
        return;
    }

    //load tileset buttons as tileset
    Tileset *btn_ptr = cache::tilesets.loadResource("tilesets/editorButtons.xml");

    //make the attributes grid
    attribGrid[0][0] = MapTile::BLOCK;
    attribGrid[0][1] = MapTile::HEAL;
    attribGrid[0][2] = MapTile::BAN;
    attribGrid[1][0] = MapTile::WARP;
    attribGrid[1][1] = MapTile::HURT;
    attribGrid[1][2] = MapTile::KILL;


    //initialise the buttons
    newTilesetButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 0, 0) )   );
    loadTilesetButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 3, 0) )   );
    tilesetDetailsButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 6, 0)  )   );
    closeTilesetButton = new gcn::ImageButton(   loader->load( Tile(btn_ptr, 5, 0)  )   );

    //initialise the tileset dropdown
    tilesetList = new TilesetList;
    tilesetDropDown = new gcn::DropDown(tilesetList);

    //initialise thetileset display area
    tilesetImage = new TilesetIcon;
    tilesetImageScroller = new InfraellyScroller(tilesetImage);


    //add self as listeners
    newTilesetButton->addMouseListener(this);
    loadTilesetButton->addMouseListener(this);
    closeTilesetButton->addMouseListener(this);
    tilesetImage->addMouseListener(this);
    tilesetDropDown->addSelectionListener(this);


    //make a none item in the list
    attribTs = cache::tilesets.loadResource("tilesets/attrib.xml");
    cache::tilesets.addPointer( attribTs );
    tilesetDropDown->setSelected(0);


    //set none to be focusable
    newTilesetButton->setFocusable( false );
    loadTilesetButton->setFocusable( false );
    tilesetDetailsButton->setFocusable( false );
    closeTilesetButton->setFocusable( false );
    tilesetDropDown->setFocusable( false );
    tilesetImage->setFocusable( false );

    //add the buttons to self
    add(newTilesetButton);
    add(loadTilesetButton);
    add(tilesetDetailsButton);
    add(closeTilesetButton);
    add(tilesetDropDown);
    add(tilesetImageScroller);


    setWidth(300);

    //resize window to fit stuffs
    resizeToContent();
}

void MapperTilesetBox::setMap(AreaMap *newWorld){ world  = newWorld; }

void MapperTilesetBox::resetPositions(){
    //set positions
    newTilesetButton->setPosition(0, 0);
    loadTilesetButton->setPosition(newTilesetButton->getX()+newTilesetButton->getWidth(), 0);
    closeTilesetButton->setPosition(loadTilesetButton->getX()+loadTilesetButton->getWidth(), 0);
    tilesetDetailsButton->setPosition( closeTilesetButton->getX()+closeTilesetButton->getWidth(), 0);
    tilesetDropDown->setPosition( 0, closeTilesetButton->getY()+closeTilesetButton->getHeight() );
    tilesetImageScroller->setPosition(0, tilesetDropDown->getY()+tilesetDropDown->getHeight());


    //set sizes
    tilesetDropDown->setWidth( getWidth() );
    tilesetImageScroller->setWidth( getWidth() );
    tilesetImageScroller->setHeight(400);
}

void MapperTilesetBox::setWidth(int width){
    InfraellyWindow::setWidth(width);
    resetPositions();
}

void MapperTilesetBox::setHeight(int height){
    InfraellyWindow::setWidth(height);
    resetPositions();
}





void MapperTilesetBox::loadTileset(Tileset *tileset){
    tilesetList->addElement(tileset);
}

void MapperTilesetBox::setTilesetPreview(Tileset *tileset){
    InfraellyImageLoader *loader = dynamic_cast<InfraellyImageLoader *>(gcn::Image::getImageLoader());
    if( loader == NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Image Loader is not an InfraellyImageLoader object" << endl;
        return;
    }
   if( tileset != NULL ){
        if( tileset->getImage() != NULL ){
            tilesetImage->setTileset( tileset );
        }
    }
}



AreaMap *MapperTilesetBox::getWorld()const{ return world; }

MapTile MapperTilesetBox::getSelectedTile()const{ return selectedTile; }

Tileset *MapperTilesetBox::getSelectedTileset()const{
    return tilesetList->at( tilesetDropDown->getSelected() );
}

const Tileset *MapperTilesetBox::getAttribTileset()const{ return attribTs; }



gcn::DropDown *MapperTilesetBox::getTilesetDropDown()const{ return tilesetDropDown; }

gcn::ImageButton *MapperTilesetBox::getMakeTilesetButton()const{ return newTilesetButton; }

gcn::ImageButton *MapperTilesetBox::getLoadTilesetButton()const{ return loadTilesetButton; }

gcn::ImageButton *MapperTilesetBox::getCloseTilesetButton()const{ return closeTilesetButton; }

gcn::ImageButton *MapperTilesetBox::getTilesetDetailsButton()const{ return tilesetDetailsButton; }





void MapperTilesetBox::freeTilesets(){
    //strip dependancy
    for( int i = 0; i < tilesetList->getNumberOfElements(); ++i ){
        //get the pointer to the tileset to remove
        Tileset *toRemove = tilesetList->at( i );
        //check if the user is trying to dlete teh nullTS
        if( toRemove != tilesetList->at(0) ){
            //change the selected ts to the nullTS
            tilesetDropDown->setSelected(0);
            //delete the entry from teh list
            tilesetList->removeElementAt( i );


            //if there is a map activley associated
            if( world != NULL ){
                //if world is initiated
                if( !world->empty() ){
                    //stip dependency from map working on
                    //if there IS dependency
                    //cycle through layers
                    for( size_t i = 0; i < world->size(); ++i ){
                        //cycle thru tiles
                        for( size_t j = 0; j < world->getLayer(i).getHeight() * world->getLayer(i).getWidth(); ++j){
                            if( world->getLayer(i).index(j).getTileset() == toRemove ){
                                world->getLayer(i).index(j).setTileset(NULL);
                            }
                        }
                    }
                }
            }
        }
    }


    // remove attribTs from the tilesets vector (no delete)
    cache::tilesets.removePointer(attribTs);

    //delete all tilesets
    tilesetList->clear();

    //  add the attrivTs back on
    cache::tilesets.addPointer(attribTs);

    //reset properties
    tilesetDropDown->setSelected( 0 );
    tilesetList->at( 0 );
    selectedTile.setTileset( attribTs );
    setTilesetPreview( attribTs );
}




void MapperTilesetBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    InfraellyWindow::mouseClicked(mouseEvent);

    if( mouseEvent.isConsumed() ){ return; };

    //if click was from closeTilesetButton
    if( mouseEvent.getSource() == closeTilesetButton ){
        //consume mouse event
        mouseEvent.consume();
        //get the pointer to the tileset to remove
        Tileset *toRemove = tilesetList->at( tilesetDropDown->getSelected() );

        //check if the user is trying to dlete teh nullTS
        if( toRemove != tilesetList->at(0) ){
            //store number to remove
            int removeIndex = tilesetDropDown->getSelected();
            //change the selected ts to the nullTS
            tilesetDropDown->setSelected(0);
            //delete the entry from teh list
            tilesetList->removeElementAt( removeIndex );


            //if there is a map activley associated
            if( world != NULL ){
                //stip dependency from map working on
                //if there IS dependency
                if( !world->empty() ){
                    //cycle through layers
                    for( size_t i = 0; i < world->size(); ++i ){
                        //cycle thru tiles
                        for( size_t j = 0; j < world->getLayer(i).getHeight() * world->getLayer(i).getWidth(); ++j){
                            if( world->getLayer(i).index(j).getTileset() == toRemove ){
                                world->getLayer(i).index(j).setTileset(NULL);
                            }
                        }
                    }
                }
            }
            //dependency of tileset stripped
            //delete the tileset
            cache::tilesets.erase(toRemove);

            //remove from selected tile
            selectedTile.setSource(NULL,0,0);
        }
    }


    //click from the tilesetImage
    if( mouseEvent.getSource() == tilesetImage ){
        //consume mouse event
        mouseEvent.consume();
        if( !tilesetList->empty() ){
            Tileset *selectedTs = selectedTile.getTileset();
            if( selectedTs != NULL ){
                // tileset select
                int tileWidth = selectedTs->getTileWidth();
                int tileHeight = selectedTs->getTileHeight();

                // get the x-y co-ords reletive to the tileset region
                int col = mouseEvent.getX() - (tilesetImage->getX()+selectedTs->getXOffset());
                int row = mouseEvent.getY() - (tilesetImage->getY()+selectedTs->getYOffset());

                // if its a multiple of tilewidth/height (ie in the barrier between)
                if( col % tileWidth == 0){ ++col; };
                if( row % tileHeight == 0){ ++row; };

                // turn co-ords into row/col
                col = col/tileWidth;
                row = row/tileHeight;


                // calculate the amount of tiles off the screen
                int trueCol = tilesetImage->getX();
                int trueRow = tilesetImage->getY();

                //  if its a multiple of tilewidth/height (ie in the barrier between)
                if( trueCol % tileWidth == 0){ ++col; };
                if( trueRow % tileHeight == 0){ ++row; };
                // turn co-ords into row/col
                trueCol = trueCol/tileWidth;
                trueRow = trueRow/tileHeight;
                //add the on screen offset to the offsecreen tiles
                trueCol += col-1;
                trueRow += row-1;

                //store the tile's new row-col
                selectedTile.setSource( selectedTs, trueCol, trueRow);
                //store attribute if on atrib ts
                if( selectedTile.getTileset() == attribTs ){
                    selectedTile.setAttribute(attribGrid[trueCol][trueRow]);
                }
            }// end if null
        }//end if empty list
    }//end if event from drop down*/
}












void MapperTilesetBox::valueChanged(const gcn::SelectionEvent& event){
    //if event from drop down
    if( event.getSource() == tilesetDropDown ){
        Tileset *newTileset = tilesetList->at( tilesetDropDown->getSelected() );
        selectedTile.setTileset( newTileset );
        if( tilesetList->at( tilesetDropDown->getSelected() ) != attribTs ){
            selectedTile.setAttribute( MapTile::REGULAR );
        } else {
            selectedTile.setAttribute( attribGrid[0][0] );
        }
        setTilesetPreview( newTileset );
    }
}













void MapperTilesetBox::draw(gcn::Graphics* graphics){
    // draw components
    InfraellyWindow::draw(graphics);

    //draw selection overlay
    if( !tilesetList->empty() ){
        Tileset *selectedTs = tilesetList->at( tilesetDropDown->getSelected() );
        if( selectedTs != NULL ){
            if( selectedTs->getImage() != NULL ){
                //tile width and height
                int tileWidth = selectedTs->getTileWidth();
                int tileHeight = selectedTs->getTileHeight();

                //calculate amount of px off the screen
                int offsetX = tilesetImage->getX();
                int offsetY = tilesetImage->getY();

                // convert pixel values to row, width
                int offCol = offsetX/tileWidth;
                int offRow = offsetY/tileHeight;



                //get the row and col of the selected tile
                int trueCol = selectedTile.getCol();
                int trueRow = selectedTile.getRow();


                //to find out the reletive row/col to draw to
                //  we must minus the offscreen tiles from the true tiles co-ords
                int drawCol = trueCol + offCol;
                int drawRow = trueRow + offRow;


                //calculate where to draw in pxs
                int drawX = (drawCol * tileWidth) + (tilesetImageScroller->getX() + getFrameSize());
                int drawY = (drawRow * tileHeight) + (tilesetImageScroller->getY() + getTitleBarHeight());


                //add offsets according to scroller
                drawX += offsetX%tileWidth;
                drawY += offsetY%tileHeight;


                //dimensions of the frame
                gcn::Rectangle d( drawX+selectedTs->getXOffset(),
                                  drawY+selectedTs->getYOffset(),
                                  tileWidth, tileHeight );

                //only draw if within region
                if(  ( d.x < tilesetImageScroller->getX()+tilesetImageScroller->getWidth() ) &&
                     ( d.x+d.width > tilesetImageScroller->getX() ) &&
                     ( d.y < tilesetImageScroller->getY()+tilesetImageScroller->getHeight()+getTitleBarHeight() ) &&
                     ( d.y+d.height > tilesetImageScroller->getY()+getTitleBarHeight() )
                ){
                    //setcolour to blue
                    graphics->setColor( gcn::Color( 0, 0, 255) );
                    //draw a frame
                    graphics->drawRectangle( d );
                    //decrease the dimensions by one pixel
                    ++d.x;
                    ++d.y;
                    d.width -= 2;
                    d.height -= 2;
                    //draw another frame (inside prev one)
                    graphics->drawRectangle( d );
                } else {
                    //not in region, draw a bob
                    //off the left
                    if( (d.x+d.width) < tilesetImageScroller->getX() ){
                        d.x = tilesetImageScroller->getX();
                        d.y += (tileHeight/2)-5;
                    };
                    //off the right
                    if( d.x > (tilesetImageScroller->getWidth()+tilesetImageScroller->getX()) ){
                        d.x = tilesetImageScroller->getWidth()-10;
                        d.y += (tileHeight/2)-5;

                    };
                    //off the top
                    if( (d.y+d.height) < tilesetImageScroller->getY() ){
                        d.y = tilesetImageScroller->getY()+getTitleBarHeight();
                        d.x += (tileWidth/2)-5;
                    };
                    //off the bottom
                    if( d.y > (tilesetImageScroller->getHeight()+tilesetImageScroller->getY()+getTitleBarHeight())  ){
                        d.y = tilesetImageScroller->getY()+tilesetImageScroller->getHeight()+getTitleBarHeight() - 10;
                        d.x += (tileWidth/2)-5;
                    };
                    //draw the bob (5x5px box)
                    d.width = 10;
                    d.height = 10;
                    //setcolour to red
                    graphics->setColor( gcn::Color( 255, 0, 0) );
                    graphics->fillRectangle( d );
                }
            }
        }
    }
}
