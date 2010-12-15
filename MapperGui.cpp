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


//  MapperGui.cpp

#include "MapperGui.hpp"
#include <iostream>
#include "globalFunc.hpp"
#include "fonts.hpp"
#include "Screen.hpp"

MapperGui::MapperGui(){
    init();
    resetPositions();
}

MapperGui::MapperGui(SDL_Surface *screen, AreaMap *worldMap){
    init();
    setTarget(screen);
    world = worldMap;
    resetPositions();

}

MapperGui::~MapperGui(){
    delete fileToolBar;
    delete layerToggleBox;
    delete tilesetBox;

    delete loadMapBox;
    delete makeTilesetBox;
    delete loadTilesetBox;
    delete filenameBox;
    delete mapDetailsBox;
    delete tilesetDetailsBox;
    delete worldImagePlaceHolder;
    delete worldScroller;
    if( worldImage != NULL ){
        SDL_FreeSurface( worldImage );
    }
}



void MapperGui::init(){
    //enable tabbing
    setTabbingEnabled( true );

    // used to regen map prev when map is edited
    mapTouched = 1;

    //init "world"
    world = NULL;

    //curent actoin
    action = NOTHING;

    // this is where the AreaMap object draws to
    worldImage = NULL;

    worldImagePlaceHolder = new gcn::Container;
    worldImagePlaceHolder->addMouseListener(this);
    worldImagePlaceHolder->setOpaque( false );


    worldScroller = new InfraellyScroller( worldImagePlaceHolder );
    worldScroller->setOpaque( false );



    // fileToolBar
    fileToolBar = new MapperFileToolBar;
    fileToolBar->addMouseListener(this);

    // layer box
    layerToggleBox = new MapperLayerToggleBox;

    // Tileset box
    tilesetBox = new MapperTilesetBox;
    tilesetBox->addMouseListener(this);
    tilesetBox->getTilesetDropDown()->addSelectionListener(this);

    //init the load tileset dialogue
    loadTilesetBox = new LoadTilesetBox;
    loadTilesetBox->addWidgetListener(this);
    //init the make tileset dialogue
    makeTilesetBox = new MakeTilesetBox;
    makeTilesetBox->addWidgetListener(this);
    //tileset details
    tilesetDetailsBox = new TilesetDetailsBox;
    mapDetailsBox = new MapDetailsBox;

    //map loading box
    loadMapBox = new LoadMapBox;
    loadMapBox->addWidgetListener(this);


    //file name getter
    filenameBox = new FilenameBox("Please input a filename");
    filenameBox->addWidgetListener(this);


    //put the gui objects into the container
    add(worldScroller);
    add(fileToolBar);
    add(layerToggleBox);
    add(tilesetBox);
    add(tilesetDetailsBox);
    add(mapDetailsBox);
    add(loadTilesetBox);
    add(makeTilesetBox);
    add(loadMapBox);
    add(filenameBox);
}



void MapperGui::resetPositions(){
    BaseGui::resetPositions();

    //set the drwable region of the map prev to screen size
    worldScroller->setWidth( container->getWidth() );
    worldScroller->setHeight( container->getHeight() );


    //set co-ords
    // fileToolBar
    // top left
    fileToolBar->setX(0);
    fileToolBar->setY(0);

    //layerToggleBox
    //under tool bar
    layerToggleBox->setX( 0 );
    layerToggleBox->setY( fileToolBar->getY()+fileToolBar->getHeight() );

    //top right
    tilesetBox->setX( container->getWidth() - tilesetBox->getWidth() );
    tilesetBox->setY( 0 );

    //middle of screen
    //filenameBox->setX( centerX(Screen::getWidth(), filenameBox->getWidth()) );
    //filenameBox->setY( centerY(Screen::getHeight(), filenameBox->getHeight()) );
}







MapperFileToolBar *MapperGui::getFileToolBar()const{ return fileToolBar; }

MapperLayerToggleBox *MapperGui::getLayerToggleBox()const{ return layerToggleBox; }

MapperTilesetBox *MapperGui::getTilesetBox()const{ return tilesetBox; }










void MapperGui::widgetHidden(const gcn::Event& event){
    // create a new map
    if( event.getSource() == loadMapBox ){
        if( loadMapBox->isInfoReady() ){
            //release old World (or create if NULL ptr)
            if( world != NULL ){
                notify("Releasing old world from memory");
                world->unload();
            } else {
                notify("Making a new AreaMap object");
                world = new AreaMap;
            }

            //make the new layers
            notify("Allocating layers...");
            world->setSize(6);
            for( size_t i = 0; i < world->size(); ++i ){
                world->getLayer(i).setSize( loadMapBox->getMapWidth(), loadMapBox->getMapHeight() );
            }


            // set other properties
            notify("Setting Properties");
            world->setFilename( loadMapBox->getFilename() );
            world->setMapName( loadMapBox->getMapName() );
            world->setExclusive( loadMapBox->isExclusive() );
            for( size_t i = 0; i < world->size(); ++i ){
                world->getLayer(i).setTileWidth( loadMapBox->getTileWidth() );
                world->getLayer(i).setTileHeight( loadMapBox->getTileHeight() );
            }
            unNotify();

            // free old surface
            if( worldImage != NULL ){
                SDL_FreeSurface(worldImage);
                worldImage = NULL;
            };
            //set the info as un usable  now
            loadMapBox->infoTaken();

            mapDetailsBox->setInfoSource( world );
        }
    } else



    // from the filename widget
    if( event.getSource() == filenameBox ){
        if( filenameBox->isInfoReady() ){
            std::string filename = correctFilepath(filenameBox->getFilename());
            switch( action ){
                case SAVEAS:
                    if( world != NULL ){
                        if ( world->size() == 0 ){
                            alert("Theres no map to save (try the new map button =P)");
                        } else if( (filename != "") && !filename.empty() ){
                            if( !world->saveMap(filename) ){
                                alert( ("Cant save file: " + filename) );
                            } else {
                                alert("Saved");
                            }
                        }
                    } else {
                        alert("Theres no map to save (try the new map button =P)");
                    }
                    action = NOTHING;
                    filenameBox->infoTaken();
                    break;

                case OPEN:
                    if( world == NULL ){ world = new AreaMap; }
                    if( !world->loadMap(filename) ){
                        alert( ("Cant open file: " + filename) );
                    } else {
                        //  free old worldImage
                        if( worldImage != NULL ){
                            SDL_FreeSurface(worldImage);
                            worldImage = NULL;
                        };
                    }
                    action = NOTHING;
                    filenameBox->infoTaken();
                    break;

                case NOTHING:
                    break;

                default:
                    action = NOTHING;
                    break;
            }
            mapDetailsBox->setInfoSource( world );
        }
    } else


    //trying to make a new tileste
    if( event.getSource() == makeTilesetBox ){
        if( makeTilesetBox->isInfoReady() ){
            //  check the save filename
            if( getFileSize(correctFilepath(makeTilesetBox->getSaveFilename())) ){
                notify("File already exists. If you save, old tileset def will be overwritten");
            }

            //set the info as un usable  now
            makeTilesetBox->infoTaken();
        }
    }

    mapTouched = 1;
}





void MapperGui::valueChanged(const gcn::SelectionEvent& event){
    //if event from drop down
    if( event.getSource() == tilesetBox->getTilesetDropDown() ){
        //change the info source for tilset info box
        tilesetDetailsBox->setInfoSource( tilesetBox->getSelectedTileset() );
    }
}










void MapperGui::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    //click from the map image ( worldImagePlaceHolder )
    if( mouseEvent.getSource() == worldImagePlaceHolder ){
        //consume mouse event
        mouseEvent.consume();
        //click from the tilesetImage
        if( world != NULL ){
            if( world->empty() ){ return; };

            int layerNum = layerToggleBox->getWorkingLayer();
            Layer& workingLayer = world->getLayer(layerNum);

            // tileset select
            int tileWidth = workingLayer.getTileWidth();
            int tileHeight = workingLayer.getTileHeight();

            // get the x-y co-ords reletive to the tileset region
            int col = mouseEvent.getX() - worldImagePlaceHolder->getX();
            int row = mouseEvent.getY() - worldImagePlaceHolder->getY();

            // if its a multiple of tilewidth/height (ie in the barrier between)
            if( col % tileWidth == 0){ ++col; };
            if( row % tileHeight == 0){ ++row; };

            // turn co-ords into row/col
            col = col/tileWidth;
            row = row/tileHeight;


            // calculate the amount of tiles off the screen
            int trueCol = worldImagePlaceHolder->getX();
            int trueRow = worldImagePlaceHolder->getY();

            //  if its a multiple of tilewidth/height (ie in the barrier between)
            if( trueCol % tileWidth == 0){ ++col; };
            if( trueRow % tileHeight == 0){ ++row; };
            // turn co-ords into row/col
            trueCol = trueCol/tileWidth;
            trueRow = trueRow/tileHeight;
            //add the on screen offset to the offsecreen tiles
            trueCol += col-1;
            trueRow += row-1;

            //check if within range
            if( (trueCol < workingLayer.getWidth()) &&
                (trueRow < workingLayer.getHeight()) ){
                //if left click
                if( mouseEvent.getButton() == gcn::MouseEvent::LEFT ){
                    //set the tile wher mouse is on map, to selected tile on ts
                    //if the attrib ts is on
                    if( tilesetBox->getSelectedTileset() == tilesetBox->getAttribTileset() ){
                        //only replace the attibute
                        workingLayer.at(trueCol, trueRow).setAttribute( tilesetBox->getSelectedTile().getAttribute() );
                    } else {
                        //replace old tile
                        workingLayer.at(trueCol, trueRow) = tilesetBox->getSelectedTile();
                    }
                } else if( mouseEvent.getButton() == gcn::MouseEvent::MIDDLE ){
                    //fill layer, to selected tile on ts
                    //if the attrib ts is on
                    if( tilesetBox->getSelectedTileset() == tilesetBox->getAttribTileset() ){
                        //only replace the attibute
                        for(int i = 0; i < workingLayer.getHeight(); ++i){
                            for(int j = 0; j < workingLayer.getWidth(); ++j){
                                workingLayer.at(j, i).setAttribute( tilesetBox->getSelectedTile().getAttribute() );
                            }
                        }
                    } else {
                        //replace old tile
                        for(int i = 0; i < workingLayer.getHeight(); ++i){
                            for(int j = 0; j < workingLayer.getWidth(); ++j){
                                workingLayer.at(j, i) = tilesetBox->getSelectedTile();
                            }
                        }
                    }
                } else if ( mouseEvent.getButton() == gcn::MouseEvent::RIGHT ){
                    //if the attrib ts is on
                    if( tilesetBox->getSelectedTileset() == tilesetBox->getAttribTileset() ){
                        //clear attrib
                        workingLayer.at(trueCol, trueRow).setAttribute( MapTile::REGULAR );
                    } else {
                        //clear the tileset (delete tile)
                        workingLayer.at(trueCol, trueRow).setSource( NULL, 0, 0 );
                    }
                }
                mapTouched = 1;
            }
        }//end if event from drop down
    } else




    //click from the tilset panel
    //load tileset button
    if( mouseEvent.getSource() == tilesetBox->getLoadTilesetButton() ){
        mouseEvent.consume();
        loadTilesetBox->setVisible( true );
        loadTilesetBox->requestFocus();
    } else

    //make tileset button
    if( mouseEvent.getSource() == tilesetBox->getMakeTilesetButton() ){
        mouseEvent.consume();
        makeTilesetBox->setVisible( true );
        makeTilesetBox->requestFocus();
    } else

    //close tileset button
    if( mouseEvent.getSource() == tilesetBox->getCloseTilesetButton() ){
        mouseEvent.consume();
        tilesetDetailsBox->setInfoSource( NULL );
    } else

    //display tileset info
    if( mouseEvent.getSource() == tilesetBox->getTilesetDetailsButton() ){
        mouseEvent.consume();
        tilesetDetailsBox->setInfoSource( tilesetBox->getSelectedTileset() );
        tilesetDetailsBox->setVisible( true );
        tilesetDetailsBox->requestFocus();
    } else




    // from file tool bar
    //new doc
    if( mouseEvent.getSource() == fileToolBar->getNewDocButton() ){
        mouseEvent.consume();
        loadMapBox->setVisible( true );
        loadMapBox->requestFocus();
    } else

    //save document
    if( mouseEvent.getSource() == fileToolBar->getSaveDocButton() ){
        mouseEvent.consume();
        if( world == NULL ){
            alert("Theres no map to save (try the new map button =P)");
            return;
        }
        if( world->size() == 0 ){
            alert("Theres no map to save (try the new map button =P)");
            return;
        }
        std::string a =  world->getFilename() ;
        if( world->getFilename() == "NULL" || world->getFilename().empty() ||
            isSpace(world->getFilename())  || world->getFilename() == "" ){
            filenameBox->setFilename("data/maps/");
            filenameBox->setVisible( true );
            action = SAVEAS;
        } else {
            if( !world->saveMap(world->getFilename()) ){
                alert( ("Cant save file: " + world->getFilename()) );
                return;
            } else {
                alert("Saved");
            }
        }
    } else

    // save as
    if( mouseEvent.getSource() == fileToolBar->getSaveAsButton() ){
        mouseEvent.consume();
        if( world == NULL ){
            alert("Theres no map to save (try the new map button =P)");
            return;
        }
        if( world->size() == 0 ){
            alert("Theres no map to save (try the new map button =P)");
            return;
        }
        action = SAVEAS;
        filenameBox->setFilename("data/maps/");
        filenameBox->setVisible( true );
    } else

    //open new doc (close current 1st)
    if( mouseEvent.getSource() == fileToolBar->getOpenDocButton() ){
        mouseEvent.consume();
        action = OPEN;
        filenameBox->setFilename("data/maps/");
        filenameBox->setVisible( true );
    } else









    //close (no save) and reOpen
    if( mouseEvent.getSource() == fileToolBar->getReopenDocButton() ){
        mouseEvent.consume();
        if( world != NULL ){
            std::string filename = correctFilepath(world->getFilename());
            //delete map
            notify("Releasing old world from memory");
            world->unload();
            //clear the tileset array
            notify("Releasing tilesets");
            tilesetBox->freeTilesets();
            //reopen
            notify("Reopening file");
            if( !world->loadMap(filename) ){
                alert( ("Cant open file: " + filename) );
            }
            unNotify();
            mapDetailsBox->setInfoSource( world );
            //  free old worldImage
            if( worldImage != NULL ){
                SDL_FreeSurface(worldImage);
                worldImage = NULL;
            };
            mapTouched = 1;
            action = NOTHING;
        }
    } else

    //close
    if( mouseEvent.getSource() == fileToolBar->getCloseDocButton() ){
        mouseEvent.consume();
        //delete map
        notify("Releasing old world from memory");
        if( world != NULL ){
            //delete worlds
            world->unload();
            //clear the tileset array
            notify("Releasing tilesets");
            tilesetBox->freeTilesets();
            unNotify();
            mapDetailsBox->setInfoSource( world );
            //  free old worldImage
            if( worldImage != NULL ){
                SDL_FreeSurface(worldImage);
                worldImage = NULL;
            }
            mapTouched = 1;
            action = NOTHING;
        }
    } else

    //get details
    if( mouseEvent.getSource() == fileToolBar->getDetailsButton() ){
        mouseEvent.consume();
        mapDetailsBox->setVisible( true );
    }
}
















void MapperGui::prepareWorldImage(){
    /**************************************
            Make sure the Cache exists
            and is still relevent
    ***************************************/
    //  check if the map has been edited since last drawn
    //  if so, regen layer images
    if( mapTouched ){
        //  check if there a no world
        if( world != NULL ){
            //  check if layers are initialised
            if( !world->empty() ){
                Layer& workingLayer = world->getLayer(layerToggleBox->getWorkingLayer());
                //  check if the worldImage exists and is coherrent
                if( worldImage == NULL ){
                    //  there is no world image, create one
                    worldImage = createSurface(
                                 workingLayer.getWidth()*
                                 workingLayer.getTileWidth(),
                                 workingLayer.getHeight()*
                                 workingLayer.getTileHeight(),
                                 graphics->getTarget()->format->BitsPerPixel,
                                 SDL_SWSURFACE );
                } else
                //  a world image exists, but it is the wrong dimensions
                if ( ( worldImage->w != workingLayer.getWidth()*
                                        workingLayer.getTileWidth() ) ||
                     ( worldImage->h != workingLayer.getHeight()*
                                        workingLayer.getTileHeight() )   )
                {
                    //free old image
                    SDL_FreeSurface(worldImage);
                    //creaet new one of correct dimensions
                    worldImage = createSurface(
                                workingLayer.getWidth()*
                                workingLayer.getTileWidth(),
                                workingLayer.getHeight()*
                                workingLayer.getTileHeight(),
                                graphics->getTarget()->format->BitsPerPixel,
                                SDL_SWSURFACE );
                }
                //match the dimensions of the world image and the PlaceHolder
                worldImagePlaceHolder->setWidth( worldImage->w );
                worldImagePlaceHolder->setHeight( worldImage->h );

                //now that worldImage is properly initialised, draw the world onto it
                drawWorld();
            }   // end layers exists
        }// end if world exists
        mapTouched = 0;
    }// end if touched
}


void MapperGui::drawWorld(){
    //  clear the map surface
    SDL_FillRect(worldImage,NULL,SDL_MapRGB(worldImage->format,255,255,255));
    //  tell the layers to update their cache
    world->imageUpdate();
    //draw re-layers if view is on
    for( size_t i = 0; i < world->size(); ++i ){
        if( layerToggleBox->getLayerControl(i)->isView() ){
            if( !layerToggleBox->getLayerControl(i)->isViewAttribute() ){
                world->getLayer(i).setShowAttributes( false );
            } else {
                world->getLayer(i).setShowAttributes( true );
            }
            world->getLayer(i).draw( worldImage );
        }
    }

    //draw gird
    SDL_Rect vertLine;
    vertLine.x = 0; //inc by tile width
    vertLine.y = 0;
    vertLine.w = 1;
    vertLine.h = worldImage->h;

    SDL_Rect horLine;
    horLine.x = 0;
    horLine.y = 0;  //inc by tile height
    horLine.w = worldImage->w;
    horLine.h = 1;

    //vertical lines
    if( world->getLayer(layerToggleBox->getWorkingLayer()).getTileWidth() > 0 ){
        for( vertLine.x = 0;
                vertLine.x < worldImage->w;
                vertLine.x += world->getLayer(layerToggleBox->getWorkingLayer()).getTileWidth() ){
            SDL_FillRect( worldImage, &vertLine, SDL_MapRGB(worldImage->format,150,150,150) );
        }
    }
    vertLine.x = worldImage->w-1;
    SDL_FillRect( worldImage, &vertLine, SDL_MapRGB(worldImage->format,150,150,150) );
    //horizontal lines
    if( world->getLayer(layerToggleBox->getWorkingLayer()).getTileHeight() > 0 ){
        for( horLine.x = 0;
                horLine.y < worldImage->w;
                horLine.y += world->getLayer(layerToggleBox->getWorkingLayer()).getTileHeight() ){
                SDL_FillRect( worldImage, &horLine, SDL_MapRGB(worldImage->format,150,150,150) );
        }
    }
    horLine.y = worldImage->h-1;
    SDL_FillRect( worldImage, &horLine, SDL_MapRGB(worldImage->format,150,150,150) );
}









void MapperGui::draw(){
    //check if there is a surface to draw on
    if( graphics->getTarget() != NULL ){
        // update the world Image
        prepareWorldImage();

        //blit the map using SDL for speed
        //  we provide clipping details so SDL has less to do
        //  since we already have the details ready
        if( worldImage != NULL ){
            drawSurfacePPAlpha( worldImage,
                         -worldImagePlaceHolder->getX(),
                         -worldImagePlaceHolder->getY(),
                         graphics->getTarget()->w,
                         graphics->getTarget()->h,
                         graphics->getTarget(),
                         0, 0);
        }


        // warn if no world
        if( world == NULL || worldImage == NULL  ){
            graphics->pushClipArea( gcn::Rectangle(0, 0, graphics->getTarget()->w, graphics->getTarget()->h) );
            graphics->setColor( gcn::Color( 0,0,0 ) );
            font::guiFont.at(24)->drawString(graphics, "Please make or a open a map", graphics->getTarget()->w/5, graphics->getTarget()->h/4);
            graphics->popClipArea();
            worldImagePlaceHolder->setWidth( 1 );
            worldImagePlaceHolder->setHeight( 1 );
        } else if( world->empty() ){
            graphics->pushClipArea( gcn::Rectangle(0, 0, graphics->getTarget()->w, graphics->getTarget()->h) );
            graphics->setColor( gcn::Color( 0,0,0 ) );
            font::guiFont.at(24)->drawString(graphics, "Please make or a open a map", graphics->getTarget()->w/5, graphics->getTarget()->h/4);
            graphics->popClipArea();
            worldImagePlaceHolder->setWidth( 1 );
            worldImagePlaceHolder->setHeight( 1 );
        }

        //call the gui's original draw command
        BaseGui::draw();
    }
}
