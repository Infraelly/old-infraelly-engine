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


#include "GameConfig.hpp"

#include "tinyxml.h"
#include "globalFunc.hpp"

/*------------------------------------
        Default Load up settings
-------------------------------------*/
const int GameConfig::fps = 30;

#ifdef DEBUG
    const bool GameConfig::bypassMainMenu = true;
#else
    const bool GameConfig::bypassMainMenu = false;
#endif



bool GameConfig::startFullscreen = false;

//  full screen res
int GameConfig::fScreenWidth = 1024;
int GameConfig::fScreenHeight = 768;
int GameConfig::fScreenBpp = 32;


//  windowed res
#ifdef DEBUG
    int GameConfig::wScreenWidth = 1024;
    int GameConfig::wScreenHeight = 768;
    int GameConfig::wScreenBpp = 32;

    bool GameConfig::sound = true;
    bool GameConfig::music = false;
#else
    int GameConfig::wScreenWidth = 1024;
    int GameConfig::wScreenHeight = 768;
    int GameConfig::wScreenBpp = 24;

    bool GameConfig::sound = true;
    bool GameConfig::music = true;
#endif

int GameConfig::audioRate = 44110;
int GameConfig::aduioBuffSize = 1024;


int GameConfig::wTestScreenWidth = 800;
int GameConfig::wTestScreenHeight = 640;


std::string GameConfig::serverIp = "localhost";
int GameConfig::serverPort = 1337;
int GameConfig::maxConnections = 100;
int GameConfig::serverThreads = 4;




bool GameConfig::saveXml(const std::string &filename){
    //xml doc
	TiXmlDocument doc;

    //xml header
	TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
    // header
	TiXmlElement *root = new TiXmlElement( "Infraelly" );
	doc.LinkEndChild( root );

	//Screen reso
    TiXmlElement *screenEl = new TiXmlElement( "Screen" );
	root->LinkEndChild( screenEl );
    screenEl->SetAttribute("fullscreen", startFullscreen);
    //windowed
    TiXmlElement *screenWinEl = new TiXmlElement( "Windowed" );
    screenEl->LinkEndChild( screenWinEl );
    screenWinEl->SetAttribute("w", wScreenWidth);
    screenWinEl->SetAttribute("h", wScreenHeight);
    screenWinEl->SetAttribute("bpp", wScreenBpp);
    //fullscreen
    TiXmlElement *screenFsEl = new TiXmlElement( "Fullscreen" );
    screenEl->LinkEndChild( screenFsEl );
    screenFsEl->SetAttribute("w", fScreenWidth);
    screenFsEl->SetAttribute("h", fScreenHeight);
    screenFsEl->SetAttribute("bpp", fScreenBpp);

    //audio
    TiXmlElement *audioEl = new TiXmlElement( "Audio" );
    root->LinkEndChild( audioEl );
    audioEl->SetAttribute("music", music);
    audioEl->SetAttribute("fx", sound);
    audioEl->SetAttribute("rate", audioRate);
    audioEl->SetAttribute("buffer", aduioBuffSize);

	//Server
    TiXmlElement *serverEl = new TiXmlElement( "Server" );
    root->LinkEndChild( serverEl );
    serverEl->SetAttribute("host", serverIp);
    serverEl->SetAttribute("port", serverPort);
    serverEl->SetAttribute("maxCon", maxConnections);
    serverEl->SetAttribute("threads", serverThreads);

	return doc.SaveFile( correctFilepath(filename) );
}

bool GameConfig::loadXml(const std::string &filename){
    //xml doc
    std::string fixedFn = correctFilepath(filename);
    TiXmlDocument doc( fixedFn );
	if( !doc.LoadFile() ){
        std::cerr << __FILE__ << __LINE__ << ": Can't read file: " << fixedFn << std::endl;
        return 0;
    }

    TiXmlHandle hDoc(&doc);
	TiXmlElement *pElem(NULL);
	TiXmlHandle root(NULL);
	TiXmlHandle screenRoot(NULL);

    //  find header
    pElem = hDoc.FirstChildElement( "Infraelly" ).ToElement();
    if( !pElem ){
        std::cout << "Config: header not found" << std::endl;
        return 0;
    }
    root = TiXmlHandle(pElem);

    //Screen resolution
    pElem = root.FirstChildElement( "Screen" ).ToElement();
    if( pElem == NULL ){
        std::cout << "Config: screen element not found" << std::endl;
        return 0;
    } else {
        screenRoot = root.FirstChildElement( "Screen" );
        pElem->QueryValueAttribute("fullscreen", &startFullscreen );
        //win
        TiXmlElement *windElem = screenRoot.FirstChildElement( "Windowed" ).ToElement();
        if( windElem == NULL ){
            std::cout << "Config: windowed element not found" << std::endl;
            return 0;
        } else {
            windElem->QueryIntAttribute("w", &wScreenWidth );
            windElem->QueryIntAttribute("h", &wScreenHeight );
            windElem->QueryIntAttribute("bpp", &wScreenBpp );
        }
        //fs
        TiXmlElement *fsElem = screenRoot.FirstChildElement( "Fullscreen" ).ToElement();
        if( fsElem == NULL ){
            std::cout << "Config: fullscreen element not found" << std::endl;
            return 0;
        } else {
            fsElem->QueryIntAttribute("w", &fScreenWidth );
            fsElem->QueryIntAttribute("h", &fScreenHeight );
            fsElem->QueryIntAttribute("bpp", &fScreenBpp );
        }
    }

    // Adudio
    pElem = root.FirstChildElement( "Audio" ).ToElement();
    if( pElem == NULL ){
        std::cout << "Config: audio element not found" << std::endl;
        return 0;
    } else {
        pElem->QueryValueAttribute("music", &music );
        pElem->QueryValueAttribute("fx", &sound );
        pElem->QueryIntAttribute("rate", &audioRate );
        pElem->QueryIntAttribute("buffer", &aduioBuffSize );
    }

	//Server
    pElem = root.FirstChildElement( "Server" ).ToElement();
    if( pElem == NULL ){
        std::cout << "Config: server element not found" << std::endl;
        return 0;
    } else {
        pElem->QueryValueAttribute("host", &serverIp );
        pElem->QueryIntAttribute("port", &serverPort );
        pElem->QueryIntAttribute("maxCon", &maxConnections );
        pElem->QueryIntAttribute("threads", &serverThreads );
    }

    return true;
}

