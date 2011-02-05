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


//  Tileset.cpp

#include <iostream>
#include <fstream>
#include "Tileset.hpp"
#include "globalFunc.hpp"
#include <tinyxml.h>
#include "caches.hpp"
#include "dataPacks.hpp"

using namespace std;


Tileset::Tileset::Tileset() :
    image(NULL),
    transKey(NULL),
    tileWidth(0),
    tileHeight(0),
    xOffset(0),
    yOffset(0),
    imageFilename("No file path"),
    alias(imageFilename)
{}

Tileset::Tileset(const std::string& newFilename) :
    image(NULL),
    transKey(NULL),
    tileWidth(0),
    tileHeight(0),
    xOffset(0),
    yOffset(0),
    imageFilename(newFilename),
    alias(imageFilename)
{
    load(newFilename);
}

Tileset::Tileset(const std::string& newFilename, const std::string& newAlias) :
    image(NULL),
    transKey(NULL),
    tileWidth(0),
    tileHeight(0),
    xOffset(0),
    yOffset(0),
    imageFilename(newFilename),
    alias(newAlias)
{
    load(newFilename, newAlias);
}


Tileset::Tileset( const std::string& newFilename,   const std::string& newAlias,
                  const SDL_Color& newTransKey) :
    image(NULL),
    transKey(NULL),
    tileWidth(0),
    tileHeight(0),
    xOffset(0),
    yOffset(0),
    imageFilename(newFilename),
    alias(newAlias)
{
    loadImage(newFilename, newAlias, newTransKey);
}

Tileset::Tileset( const std::string& newFilename,    const std::string& newAlias,
                  unsigned int newTileWidth,         unsigned int newTileHeight) :
    image(NULL),
    transKey(NULL),
    tileWidth(newTileWidth),
    tileHeight(newTileHeight),
    xOffset(0),
    yOffset(0),
    imageFilename(newFilename),
    alias(newAlias)
{
    loadImage(newFilename, newAlias, newTileWidth, newTileHeight);
}

Tileset::Tileset( const std::string& newFilename,   const std::string& newAlias,
                  const SDL_Color& newTransKey,     unsigned int newTileWidth,
                  unsigned int newTileHeight) :
    image(NULL),
    transKey(NULL),
    tileWidth(newTileWidth),
    tileHeight(newTileHeight),
    xOffset(0),
    yOffset(0),
    imageFilename(newFilename),
    alias(newAlias)
{
    loadImage(newFilename, newAlias, newTransKey, newTileWidth, newTileHeight);
}


// copy ctor: deals with unitiated objects
Tileset::Tileset(const Tileset& src){
    image = NULL;
    transKey = src.transKey;
    tileWidth = src.tileWidth;
    tileHeight = src.tileHeight;
    xOffset = src.xOffset;
    yOffset = src.yOffset;
    imageFilename = src.imageFilename;
    alias = src.alias;

    if( transKey == NULL ){
        loadImage(imageFilename, alias, tileWidth, tileHeight);
    } else {
        loadImage(imageFilename, alias, *transKey, tileWidth, tileHeight);
    }
}

Tileset& Tileset::operator=(const Tileset& rhs){
    if( &rhs != this ){
        transKey = rhs.transKey;
        tileWidth = rhs.tileWidth;
        tileHeight = rhs.tileHeight;
        xOffset = rhs.xOffset;
        yOffset = rhs.yOffset;
        imageFilename = rhs.imageFilename;
        alias = rhs.alias;

        //only free image if theres something stored there
        if( image != NULL ){ SDL_FreeSurface(image); }
        image = NULL;
        if( transKey == NULL ){
            loadImage(imageFilename, alias, tileWidth, tileHeight);
        } else {
            loadImage(imageFilename, alias, *transKey, tileWidth, tileHeight);
        }
    }
    return *this;
}



Tileset::~Tileset(){
    delete transKey;
}







int Tileset::loadImage(const std::string& newFileName){
    /* dont delete, this resource is magaed by cache
    if( image != NULL ){
        delete image;
    }*/
    image = cache::images.loadResource(newFileName);
    imageFilename = newFileName;
    tileWidth = 0;
    tileHeight = 0;
    xOffset = 0;
    yOffset = 0;
    delete transKey;
    transKey = NULL;
    alias = imageFilename;
    if( image == NULL ){
        return -1;
    } else {
        return 1;
    }
}


int Tileset::loadImage(const std::string& newFileName, const std::string& newAlias){
    /* dont delete, this resource is magaed by cache
    if( image != NULL ){
        delete image;
    }*/
    image = cache::images.loadResource(newFileName);
    imageFilename = newFileName;
    tileWidth = 0;
    tileHeight = 0;
    xOffset = 0;
    yOffset = 0;
    delete transKey;
    transKey = NULL;
    if( isSpace(newAlias) ){
        alias = imageFilename;
    } else {
        alias = newAlias;
    }
    if( image == NULL ){
        return -1;
    } else {
        return 1;
    }
}

int Tileset::loadImage( const std::string& newFileName,  const std::string& newAlias,
                    const SDL_Color& newTransKey){
    /* dont delete, this resource is magaed by cache
    if( image != NULL ){
        delete image;
    }*/
    image = cache::images.loadResource(newFileName, newTransKey);
    imageFilename = newFileName;
    delete transKey;
    transKey = new SDL_Color( newTransKey );
    tileWidth = 0;
    tileHeight = 0;
    xOffset = 0;
    yOffset = 0;
    if( isSpace(newAlias) ){
        alias = imageFilename;
    } else {
        alias = newAlias;
    }
    if( image == NULL ){
        return -1;
    } else {
        return 1;
    }
}

int Tileset::loadImage( const std::string& newFileName,   const std::string& newAlias,
                    unsigned int newTileWidth, unsigned int newTileHeight){
    /* dont delete, this resource is magaed by cache
    if( image != NULL ){
        delete image;
    }*/
    image = cache::images.loadResource(newFileName);
    imageFilename = newFileName;
    delete transKey;
    transKey = NULL;
    tileWidth = newTileWidth;
    tileHeight = newTileHeight;
    xOffset = 0;
    yOffset = 0;
    if( isSpace(newAlias) ){
        alias = imageFilename;
    } else {
        alias = newAlias;
    }
    if( image == NULL ){
        return -1;
    } else {
        return 1;
    }
}

int Tileset::loadImage( const std::string& newFileName,     const std::string& newAlias,
                    const SDL_Color& newTransKey,       unsigned int newTileWidth,
                    unsigned int newTileHeight ){
    /* dont delete, this resource is magaed by cache
    if( image != NULL ){
        delete image;
    }*/
    image = cache::images.loadResource(newFileName, newTransKey);
    tileWidth = newTileWidth;
    tileHeight = newTileHeight;
    imageFilename = newFileName;
    delete transKey;
    transKey = new SDL_Color( newTransKey );
    xOffset = 0;
    yOffset = 0;
    if( isSpace(newAlias) ){
        alias = imageFilename;
    } else {
        alias = newAlias;
    }
    if( image == NULL ){
        return -1;
    } else {
        return 1;
    }
}


bool Tileset::saveXml( const std::string& savefilename ){
    filename_ = correctFilepath(savefilename);

    //xml doc
	TiXmlDocument doc;

    //xml header
	TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
    //tileset header
	TiXmlElement *root = new TiXmlElement( "Tileset" );
	doc.LinkEndChild( root );


    //tlieset details
    //tileset imageFilename
    TiXmlElement *fnEl = new TiXmlElement( "ImageFilename" );
	root->LinkEndChild( fnEl );
	TiXmlText *fnText = new TiXmlText( imageFilename );
	fnEl->LinkEndChild( fnText );

	//tileset name
    TiXmlElement *tsName = new TiXmlElement( "TilesetName" );
	root->LinkEndChild( tsName );
	TiXmlText *tsText = new TiXmlText( alias );
	tsName->LinkEndChild( tsText );

	//trans key
    TiXmlElement *transKeyEl = new TiXmlElement( "TransKey" );
	root->LinkEndChild( transKeyEl );
	if( transKey != NULL ){
        transKeyEl->SetAttribute("use", 1);
        transKeyEl->SetAttribute("r", transKey->r);
        transKeyEl->SetAttribute("g", transKey->g);
        transKeyEl->SetAttribute("b", transKey->b);
    } else {
        transKeyEl->SetAttribute("use", 0);
        transKeyEl->SetAttribute("r", 0);
        transKeyEl->SetAttribute("g", 0);
        transKeyEl->SetAttribute("b", 0);
    }

	//TileSize
    TiXmlElement *sizeEl = new TiXmlElement( "TileSize" );
	root->LinkEndChild( sizeEl );
    sizeEl->SetAttribute("w", tileWidth);
    sizeEl->SetAttribute("h", tileHeight);

	//TileSize
    TiXmlElement *offsetEl = new TiXmlElement( "Offset" );
	root->LinkEndChild( offsetEl );
    offsetEl->SetAttribute("x", xOffset);
    offsetEl->SetAttribute("y", yOffset);

	return doc.SaveFile( filename_ );
}


//load from xml
bool Tileset::loadXml( const TiXmlDocument& doc ){
    TiXmlHandle hDoc(const_cast<TiXmlDocument*>(&doc));
	TiXmlElement *pElem(NULL);
	TiXmlHandle root(NULL);

    int useTranskey = 0;
    SDL_Color newKey;

    //  find tileset header
	{
		pElem = hDoc.FirstChildElement( "Tileset" ).ToElement();
		if( !pElem ){
		    std::cerr << "Tileset load fail: no header found" << std::endl;
		    return 0;
        } else {
            root = TiXmlHandle(pElem);
        }
	}

    //  Get filename
	{
        pElem = root.FirstChildElement( "ImageFilename" ).ToElement();
		if( !pElem || pElem->GetText() == NULL ){
            std::cerr << "Tileset load fail:" << filename_  <<": no text in imageFilename" << std::endl;
            return 0;
        } else {
            imageFilename = correctFilepath(pElem->GetText());
		}
    }

    //  Get alias
	{
        pElem = root.FirstChildElement( "TilesetName" ).ToElement();
		if( !pElem || pElem->GetText() == NULL ){
            std::cerr << "Tileset load fail: " << filename_ << " no text in alias" << std::endl;
            return 0;
        } else {
            alias = pElem->GetText();
        }
    }

    //  Get transkey
	{
        pElem = root.FirstChildElement( "TransKey" ).ToElement();
		if( !pElem ){
            std::cerr << "Tileset load fail: " << filename_ << " no transkey" << std::endl;
            return 0;
        } else {
            pElem->QueryIntAttribute("use", &useTranskey );
            if( useTranskey ){
                int r, g, b;
                pElem->QueryIntAttribute("r", &r );
                pElem->QueryIntAttribute("g", &g );
                pElem->QueryIntAttribute("b", &b );
                newKey.r = r;
                newKey.g = g;
                newKey.b = b;
            }
        }
    }

    //  Get size
	{
        pElem = root.FirstChildElement( "TileSize" ).ToElement();
		if( !pElem ){
            std::cerr << "Tileset load fail: " << filename_ << " no size" << std::endl;
            return 0;
        } else {
            pElem->QueryIntAttribute("w", &tileWidth );
            pElem->QueryIntAttribute("h", &tileHeight );
        }
    }

    //  Get Offsets
	{
        pElem = root.FirstChildElement( "Offset" ).ToElement();
        if( !pElem ){
            std::cerr << "Tileset load fail: " << filename_ << " no size" << std::endl;
            return 0;
        } else {
            pElem->QueryIntAttribute("x", &xOffset );
            pElem->QueryIntAttribute("y", &yOffset );
        }
    }

    //  All details ready, load!
    int rVal = 0;
    if( useTranskey ){
        rVal = loadImage(imageFilename, alias, newKey, tileWidth, tileHeight);
    } else {
        rVal = loadImage(imageFilename, alias, tileWidth, tileHeight);
    }

    setXOffset(xOffset);
    setYOffset(yOffset);

    if( rVal != -1 ){
        return 1;
    } else {
        return 0;
    }
}

bool Tileset::load(const std::string& filename){
    filename_ = correctFilepath(filename);

    //xml doc
    TiXmlDocument doc( filename_ );
	if( !doc.LoadFile() ){
        std::cerr << __FILE__ << __LINE__ << ": Tileset(load):Can't read file: " << filename_ << std::endl;
        return 0;
    }

    return loadXml(doc);
}

bool Tileset::load( const ResourcePack& pack, const std::string& filepath ){
    if( !packs::mainPack.findFile( filepath ) ){ return false; }
    filename_ = filepath;
    std::string xmlData;
    packs::mainPack.getResourceText(filepath, xmlData);

    TiXmlDocument doc;
    doc.Parse((const char*)xmlData.c_str(), 0, TIXML_ENCODING_UTF8);
    return loadXml(doc);
}




SDL_Surface* Tileset::getImage()const{ return image; }

int Tileset::getTileWidth()const{ return tileWidth; }

int Tileset::getTileHeight()const{ return tileHeight; }

int Tileset::getTilesetWidth()const{
    if(image != NULL){
        return image->w;
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Warning: Attempting to get tilesetWidth from tileset with no source" << endl;
        return 0;
    }
}

int Tileset::getTilesetHeight()const{
    if(image != NULL){
        return image->h;
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Warning: Attempting to get tilesetHeight from tileset with no source" << endl;
        return 0;
    }
}

int Tileset::getXOffset()const{ return xOffset; }

int Tileset::getYOffset()const{ return yOffset; }

const std::string& Tileset::getAlias()const{ return alias; }

const std::string& Tileset::getImageFilename()const{ return imageFilename; }

SDL_Color* Tileset::getTransKey()const{ return transKey; }

int Tileset::getRows()const{
    if( image != NULL ){
        if( (image->h > 0) && (tileHeight > 0) ){
            return (image->h/tileHeight);
        }
    }
    return 0;
}

int Tileset::getCols()const{
    if( image != NULL ){
        if( (image->w > 0) && (tileWidth > 0) ){
            return (image->w/tileWidth);
        }
    }
    return 0;
}







void Tileset::setXOffset(int newXOffset){ xOffset = newXOffset; }

void Tileset::setYOffset(int newYOffset){ yOffset = newYOffset; }

void Tileset::setTileWidth(unsigned int newTileWidth){ tileWidth = newTileWidth; }

void Tileset::setTileHeight(unsigned int newTileHeight){ tileHeight = newTileHeight; }

void Tileset::setTransKey(const SDL_Color& newTransKey){
    if( transKey != NULL ){
        *transKey = newTransKey;
    } else {
        transKey = new SDL_Color( newTransKey );
    }
    SDL_SetColorKey(image, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(image->format, newTransKey.r, newTransKey.g, newTransKey.b));
}

void Tileset::setImage(SDL_Surface *newSource){ image = newSource; }

void Tileset::setImage(SDL_Surface *newSource, const std::string& newAlias){
    image = newSource;
    alias = newAlias;
}

void Tileset::setAlias(const std::string& newAlias){ alias = newAlias; }
