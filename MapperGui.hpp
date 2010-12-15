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


#ifndef MAPPERGUI_HPP
#define MAPPERGUI_HPP

//  MapperGui.hpp

#include <string>

#include "guichan.hpp"

#include "BaseGui.hpp"

#include "AreaMap.hpp"
#include "InfraellyScroller.hpp"
#include "InfraellyImageLoader.hpp"
#include "MapperFileToolBar.hpp"
#include "MapperLayerToggleBox.hpp"
#include "MapperTilesetBox.hpp"
#include "LoadTilesetBox.hpp"
#include "MakeTilesetBox.hpp"
#include "TilesetDetailsBox.hpp"
#include "Notification.hpp"
#include "AlertBox.hpp"
#include "LoadMapBox.hpp"
#include "FilenameBox.hpp"
#include "MapDetailsBox.hpp"



class MapperGui : public BaseGui,
                  public gcn::WidgetListener,
                  public gcn::SelectionListener
{
    public:
        MapperGui();
        MapperGui(SDL_Surface *screen, AreaMap *worldMap = NULL);
        ~MapperGui();



        //  Resets positions of widgets
        void resetPositions();



        //  Returns the FileToolBar
        MapperFileToolBar *getFileToolBar()const;
        //  Returns the LayerBox
        MapperLayerToggleBox *getLayerToggleBox()const;
        //  Returns the TilesetBox
        MapperTilesetBox *getTilesetBox()const;




        //  Used for geting info from popups
        //  info is collected when  widgets are hideen
        void widgetHidden(const gcn::Event& event);
        //  Listener, for the drop down (tileset info)
        void valueChanged(const gcn::SelectionEvent& event);
        //mouse listener
        void mouseClicked(gcn::MouseEvent& mouseEvent);




        // overide draw to generate an image of the map
        void draw();





    private:
        //  So that we can run functions after awidget is hidden
        enum Actions{
            SAVEAS,
            OPEN,
            NOTHING
        } action;


        void init();                                //  Initialises object


        //  Makes sure worldImage is ready, and draws world onto world image
        void prepareWorldImage();
        //  Draws world onto WorldImage (called prepareWorldImage)
        //  Beware, this function does not check if it's dependent structures
        //      are properly initialised.
        void drawWorld();




        bool mapTouched;                            //  When the map changes, regen image
        AreaMap *world;                             //  Stores the AreaMap at hand
        SDL_Surface *worldImage;                    //  Stores the image of thh map
        gcn::Container *worldImagePlaceHolder;      //  Place Holder for map's image
        InfraellyScroller *worldScroller;           //  Scrolls Map

        MapperFileToolBar *fileToolBar;             //  The file toolBar
        MapperLayerToggleBox *layerToggleBox;       //  The layerBox
        MapperTilesetBox *tilesetBox;               //  The tileset box

        LoadMapBox *loadMapBox;                     //  The load map dialogue
        LoadTilesetBox *loadTilesetBox;             //  The laod tileset dialoge
        MakeTilesetBox *makeTilesetBox;             //  The laod tileset dialoge

        TilesetDetailsBox *tilesetDetailsBox;       //  The tileset details dialogue
        MapDetailsBox *mapDetailsBox;               //  The map Details dialogue

        FilenameBox *filenameBox;                   //  The input file name dialogue
};




#endif
