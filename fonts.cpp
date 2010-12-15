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


// fonts.cpp

#include <iostream>
#include <vector>
#include <string>

#include <SDL/SDL_ttf.h>
#include "sdltruetypefont.hpp"

#include "globalFunc.hpp"
#include "dataPacks.hpp"


using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;


namespace font{
    //filenames of fonts

    //  All things within gui
    string guiFontFilename = correctFilepath("fonts/efflores.ttf");
    //  The title bar of the gui (and the intro menu)
    string guiTitleFontFilename = correctFilepath("fonts/forgottb.ttf");

    //  Used for everything else
    //  Characters'name, Characters' speech bubbles, "pres esc to esscape"
    string mainFontFilename = correctFilepath("fonts/forgotte.ttf");
    //  Used for credits
    string creditFontFilename = correctFilepath("fonts/creditva.ttf");


    //standard font sizes
    unsigned short stdChatFontSize = 15;
    unsigned short stdGuiTitleFontSize = 14;
    unsigned short stdGuiFontSize = 12;


    //holds the SDL_ttf fonts
    vector<TTF_Font *> mainFont;
    vector<TTF_Font *> creditFont;

    //these are guichan fonts
    std::vector<gcn::contrib::SDLTrueTypeFont *> guiFont;
    std::vector<gcn::contrib::SDLTrueTypeFont *> guiTitleFont;



    //preloads fonts
    void loadFonts(){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Loading fonts..." << endl;

        bool mainFound = packs::mainPack.findFile( mainFontFilename );
        bool creditFound = packs::mainPack.findFile( creditFontFilename );
        bool mainGuiFound = packs::mainPack.findFile( guiFontFilename );
        bool titleGuiFound = packs::mainPack.findFile( guiTitleFontFilename );

        if( !mainFound ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Main font not found \""
                 << mainFontFilename << "\"" << endl;
        }
        if( !creditFound ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Credit font not found \""
                 << creditFontFilename << "\"" << endl;
        }
        if( !mainGuiFound ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "MainGui font not found \""
                 << guiFontFilename << "\"" << endl;
        }
        if( !titleGuiFound ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "TitleGui font not found \""
                 << guiTitleFontFilename << "\"" << endl;
        }

        //get file info from pack
        ResourceFile mainRcf, creditFontRcf, mainGuiRcf, titleGuiRcf;
        packs::mainPack.getResource( mainFontFilename, mainRcf );
        packs::mainPack.getResource( creditFontFilename, creditFontRcf );
        packs::mainPack.getResource( guiFontFilename, mainGuiRcf );
        packs::mainPack.getResource( guiTitleFontFilename, titleGuiRcf );

        //load fonts
        for (int i = 0; i < 76; ++i ){
            TTF_Font *ttfMainFont = NULL;
            TTF_Font *ttfCreditFont = NULL;
            gcn::contrib::SDLTrueTypeFont *ttfMainGuiFont = NULL;
            gcn::contrib::SDLTrueTypeFont *ttfTitleGuiFont = NULL;
            #if USE_PACKED_RESOURCES
                SDL_RWops *rw;
                if( mainFound ){
                    rw = SDL_RWFromMem((char*)&packs::mainPack.getRawData()[mainRcf.start], mainRcf.size);
                    ttfMainFont = TTF_OpenFontRW( rw, true, i );
                }
                if( creditFound ){
                    rw = SDL_RWFromMem((char*)&packs::mainPack.getRawData()[creditFontRcf.start], creditFontRcf.size);
                    ttfCreditFont = TTF_OpenFontRW( rw, true, i );
                }
                if( mainGuiFound ){
                    rw = SDL_RWFromMem((char*)&packs::mainPack.getRawData()[mainGuiRcf.start], mainGuiRcf.size);
                    if( rw != NULL ) ttfMainGuiFont = new gcn::contrib::SDLTrueTypeFont( rw, true, i );
                }
                if( titleGuiFound ){
                    rw = SDL_RWFromMem((char*)&packs::mainPack.getRawData()[titleGuiRcf.start], titleGuiRcf.size);
                    if( rw != NULL ) ttfTitleGuiFont = new gcn::contrib::SDLTrueTypeFont( rw, true, i );
                }
            #else
                ttfMainFont = TTF_OpenFont(mainFontFilename.c_str(), i);
                ttfCreditFont = TTF_OpenFont(creditFontFilename.c_str(), i);
                ttfMainGuiFont = new gcn::contrib::SDLTrueTypeFont(guiFontFilename, i);
                ttfTitleGuiFont = new gcn::contrib::SDLTrueTypeFont(guiTitleFontFilename, i);
            #endif

            mainFont.push_back( ttfMainFont );
            creditFont.push_back( ttfCreditFont );
            guiFont.push_back( ttfMainGuiFont );
            guiTitleFont.push_back( ttfTitleGuiFont );


            //check errors
            if(mainFont.at(i) == NULL){
                cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_TTF error loading font \"" << mainFontFilename
                     << "\", size " << i << ": " << TTF_GetError() << endl;
            }
            if(creditFont.at(i) == NULL){
                cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_TTF error loading font \"" << creditFontFilename
                     << "\", size " << i << ": " << TTF_GetError() << endl;
            }

            if(guiFont.at(i) == NULL){
                cerr << __FILE__ << " " << __LINE__ << ": " << "guichan error loading font \"" << guiFontFilename
                     << "\", size " << i << ": " << TTF_GetError() << endl;
            }
            if(guiTitleFont.at(i) == NULL){
                cerr << __FILE__ << " " << __LINE__ << ": " << "guichan error loading font \"" << guiTitleFontFilename
                     << "\", size " << i << ": " << TTF_GetError() << endl;
            }

        }

        cerr << __FILE__ << " " << __LINE__ << ": " << "Fonts loaded" << endl << endl;
    }




    //frees preloaded fonts
    void freeFonts(){

        cerr << __FILE__ << " " << __LINE__ << ": " << "Closing fonts..." << endl;

        //  free fonts
        for (int i = 0; i < 76; ++i ){
            TTF_CloseFont(mainFont.at(i));
            TTF_CloseFont(creditFont.at(i));
            delete guiFont.at(i);
            delete guiTitleFont.at(i);
        }


        cerr << __FILE__ << " " << __LINE__ << ": " << "Fonts closed" << endl << endl;
    }

}
