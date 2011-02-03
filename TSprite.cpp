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


//  TSprite.cpp

#include "TSprite.hpp"
#include <iostream>
#include "caches.hpp"
#include "globalFunc.hpp"
#include "dataPacks.hpp"
#include "Tile.hpp"

using namespace std;


TSprite::TSprite() :
    src_(NULL),
    numImages_(0),
    imagesPerRow_(0),
    xOffset_(0),
    yOffset_(0),

    frameDelay_(1000/5),
    lastFrameAdvance_(0),
    frameProgression_(0),

    alias("unamed tsprite")
{}

TSprite::TSprite(const Tile &src) :
    src_(src.getTileset()),
    numImages_(0),
    imagesPerRow_(0),
    xOffset_(0),
    yOffset_(0),

    frameDelay_(0),
    lastFrameAdvance_(0),
    frameProgression_(0),

    alias("unamed tsprite (made from tile)")
{
    if( src_ ){
        numImages_ = src_->getCols() * src_->getRows();
        imagesPerRow_ =src_->getRows();

        frames_.push_back(src);
    }
}

TSprite::~TSprite(){}




int TSprite::getFrameCount()const{ return frames_.size(); }

int TSprite::getFps()const{ return (int)(1000/frameDelay_); }

int TSprite::getFrameProgression(){
    advanceFrames();
    return frameProgression_;
}



Tileset *TSprite::getSource()const{ return src_; }

int TSprite::getNumImages()const{ return numImages_; }

int TSprite::getImagesPerRow()const{ return imagesPerRow_; }

int TSprite::getXOffset()const{ return xOffset_; }

int TSprite::getYOffset()const{ return yOffset_; }

const std::string& TSprite::getAlias()const{ return alias; }




int TSprite::getWidth()const{
    if( !frames_.empty() ){
        return frames_.at(0).getWidth();
    }
    //else
    return 0;
}

int TSprite::getHeight()const{
    if( !frames_.empty() ){
        return frames_.at(0).getHeight();
    }
    //else
    return 0;
}

const Tile* TSprite::getFrame(size_t frameNo){
    if( !frames_.empty() ){
        if( (frameNo > 0) && (frameNo < frames_.size()) ){
            return &frames_.at(frameNo);
        } else {
            if( !frames_.empty() ){
                return &frames_.at(0);
            }
        }
    }
    //else
    return NULL;
}





void TSprite::setFrameProgression(unsigned int frameNumber){
    if ( frameNumber > frames_.size()-1 ){
        frameProgression_ = frames_.size()-1;
    } else {
        frameProgression_ = frameNumber;
    }
}


void TSprite::setFps(unsigned int FPS){
    if( FPS == 0 ) { frameDelay_ = -1; return; }
    frameDelay_ = 1000/FPS;
}

void TSprite::setAlias(const std::string& newAlias){ alias = newAlias; }


void TSprite::frameForward(){
    ++frameProgression_;
    if (frameProgression_ > frames_.size()-1){
        frameProgression_ = 0;
    }
}

void TSprite::frameBackwards(){
    --frameProgression_;
    if (frameProgression_ < 0){
        frameProgression_ = frames_.size()-1;
    }
}

void TSprite::frameForward(unsigned numberOfFrames){
    for (unsigned i = 0; i < numberOfFrames; ++i ){
        frameForward();
    }
}

void TSprite::frameBackwards(unsigned numberOfFrames){
    for (unsigned i = 0; i < numberOfFrames; ++i ){
        frameBackwards();
    }
}



int TSprite::load( Tileset *srcTileset,
                    unsigned int numImages,      unsigned int imagesPerRow,
                    unsigned int xOffsetTiles,   unsigned int yOffsetTiles,
                    int fps )
{
    if( (srcTileset != NULL) && (imagesPerRow > 0) &&
        (numImages > 0) && (imagesPerRow > 0) ){
        //range check params
        if( xOffsetTiles < 0 ){ xOffsetTiles = 0; };
        if( yOffsetTiles < 0 ){ yOffsetTiles = 0; };
        if( imagesPerRow > srcTileset->getCols() ){ imagesPerRow = srcTileset->getCols(); }
        if( numImages > (srcTileset->getRows()*srcTileset->getCols()) ){
            numImages = (srcTileset->getRows()*srcTileset->getCols());
        }
        if( numImages < imagesPerRow ){ imagesPerRow = numImages; }

        //clean out old frames
        frames_.clear();

        frameProgression_ = 0;
        src_ = srcTileset;
        numImages_ = numImages;
        imagesPerRow_ = imagesPerRow;
        xOffset_ = xOffsetTiles;
        yOffset_ = yOffsetTiles;
        setFps(fps);

        int startCol = xOffset_;
        int endCol = xOffset_ + imagesPerRow;

        int startRow = yOffset_;
        int remainder = 0;
        if( numImages > imagesPerRow ){
            remainder = numImages%imagesPerRow;
        }
        int endRow = yOffset_ + (numImages/(imagesPerRow-remainder));

        //do some range checking and confine the range of start/end row/col
        if( startCol > srcTileset->getCols() ){ startCol = 0; };
        if( startRow > srcTileset->getRows() ){ startRow = 0; };
        if( endCol > srcTileset->getCols() ){ endCol = srcTileset->getCols(); };
        if( endRow > srcTileset->getRows() ){ endRow = srcTileset->getRows(); };


        int row = startRow;
        int col = startCol;

        Tile tmp;
        //load up the images which will form a sqaure
        for( row = startRow ; row < endRow; ++row ){
            for( col = startCol; col < endCol; ++col ){
                tmp.setSource(srcTileset, col, row);
                frames_.push_back(tmp);
            }
        }

        //load remaining images in last row
        for( col = startCol; col < remainder; ++col ){
            tmp.setSource(srcTileset, col, row);
            frames_.push_back(tmp);
        }
        return 1;
    }
    return -1;
}


//save tsprite data to an xml file
bool TSprite::saveXml( const std::string& savefilename ){
    // if theres no source, the s[prite is invalid/incomplete.
    //  we simply wont write it
    if( src_ == NULL ){ return 0; }

    filename_ = correctFilepath(savefilename);

    //xml doc
	TiXmlDocument doc;

    //xml header
	TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
    //tileset header
	TiXmlElement *root = new TiXmlElement( "TSprite" );
	doc.LinkEndChild( root );

    //tileset Filename
    TiXmlElement *fnEl = new TiXmlElement( "Tileset" );
	root->LinkEndChild( fnEl );
	TiXmlText *fnText = new TiXmlText( src_->getFilename() );
	fnEl->LinkEndChild( fnText );

	//source dimensions
    TiXmlElement *countEl = new TiXmlElement( "ImageCount" );
	root->LinkEndChild( countEl );
    countEl->SetAttribute("frames", numImages_);
    countEl->SetAttribute("perRow", imagesPerRow_);
    countEl->SetAttribute("fps", getFps());

	//offsets key
    TiXmlElement *offsetsEl = new TiXmlElement( "Offsets" );
	root->LinkEndChild( offsetsEl );
    offsetsEl->SetAttribute("x", xOffset_);
    offsetsEl->SetAttribute("y", yOffset_);


	return doc.SaveFile( filename_ );
}

//load tsprite data from an xml file
bool TSprite::loadXml( const TiXmlDocument& doc  ){
    TiXmlHandle hDoc(const_cast<TiXmlDocument*>(&doc));
	TiXmlElement *pElem(NULL);
	TiXmlHandle root(NULL);

    string tilesetFn = "";
    int fps = 8;

    //  find tsprite header
	{
		pElem = hDoc.FirstChildElement( "TSprite" ).ToElement();
		if( !pElem ){
		    std::cerr << "TSprite load fail: TSprite header not found" << std::endl;
            return 0;
        }
		root = TiXmlHandle(pElem);
	}

    //  Get Tileset
	{
        pElem = root.FirstChildElement( "Tileset" ).ToElement();
		if( !pElem ){
            std::cerr << "TSprite load fail: no Tileset element" << std::endl;
		    return 0;
        }
		if( pElem->GetText() == NULL ){
            std::cerr << "TSprite load warning: no text in Tileset element" << std::endl;
		}
        tilesetFn = correctFilepath(pElem->GetText());
    }

    //  Get dimensions
	{
        pElem = root.FirstChildElement( "ImageCount" ).ToElement();
		if( !pElem ){
		    std::cerr << "TSprite load fail: no image count" << std::endl;
		    return 0;
        }
        int frameCount = 0;
        pElem->QueryIntAttribute("frames", &frameCount );
        numImages_ = frameCount;
        pElem->QueryIntAttribute("perRow", &imagesPerRow_ );
        pElem->QueryIntAttribute("fps", &fps);
    }

    //  Get Offsets
	{
        pElem = root.FirstChildElement( "Offsets" ).ToElement();
		if( !pElem ){
		    std::cerr << "TSprite load warning: no Offsets element" << std::endl;
		    xOffset_ = 0;
		    yOffset_ = 0;
        } else {
            pElem->QueryIntAttribute("x", &xOffset_);
            pElem->QueryIntAttribute("y", &yOffset_);
        }
    }

    //  All details ready, load
    Tileset *ts = cache::tilesets.loadResource(tilesetFn);
    if( ts ){
        return load( ts, numImages_, imagesPerRow_, xOffset_, yOffset_, fps );
    }
    return 0;
}


bool TSprite::load(const std::string& filename){
    filename_ = correctFilepath(filename);

    //xml doc
    TiXmlDocument doc( filename_ );
	if( !doc.LoadFile() ){
        std::cerr << __FILE__ << __LINE__ << ": TSprite(load):Can't read file: " << filename_ << std::endl;
        return 0;
    }

    return loadXml(doc);
}

bool TSprite::load( const ResourcePack& pack, const std::string& filepath ){
    if( !packs::mainPack.findFile( filepath ) ){ return false; }
    std::string xmlData;
    packs::mainPack.getResourceText(filepath,xmlData);

    TiXmlDocument doc;
    doc.Parse((const char*)xmlData.c_str(), 0, TIXML_ENCODING_UTF8);
    return loadXml(doc);
}



void TSprite::unload(){ frames_.clear(); }



//advances frames according to FPS
void TSprite::advanceFrames(){
    //calculate no frames to advance
    int framesBehind = (SDL_GetTicks()-lastFrameAdvance_)/frameDelay_;
    if( framesBehind > 0 ){
        //advance
        frameForward( framesBehind );
        //set last forward time
        lastFrameAdvance_ = SDL_GetTicks();
    }
}


void TSprite::draw(SDL_Surface *dest, int x, int y){
    advanceFrames();
    draw(dest, x, y, frameProgression_);
}

void TSprite::draw(SDL_Surface *dest, int x, int y, unsigned int frameNumber){
    if( !frames_.empty() ){
        if( frameNumber >= frames_.size() ){
            frameNumber = frames_.size()-1;
        }
        frames_.at(frameNumber).draw(dest, x, y);
    }
}
