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


//  Animation.cpp

#include "Animation.hpp"
#include <iostream>
#include <cstddef>
#include <memory>
#include <algorithm>
#include "globalFunc.hpp"
#include "SDL/SDL_rotozoom.h"
#include "caches.hpp"


bool Animation::validAnimType(int type){
    return ( (type==ITEM) || (type==CHARACTER_PART) || (type==OBJECT) );
}

bool Animation::validSnapPoint(int point){
    return ( (point==HEAD)       || (point==BODY)      || (point==LEFT_HAND) ||
             (point==RIGHT_HAND) || (point==LEFT_FOOT) || (point==RIGHT_FOOT) );
}

bool Animation::validKeyPoint(int point){
    return ( (point==TL)   || (point==TOP) || (point==TR)    ||
             (point==LEFT) || (point==MID) || (point==RIGHT) ||
             (point==BL)   || (point==BOT) || (point==BR) );
}



Animation::Animation() :
    name_("unamed animation"),
    type_(ITEM),
    snapPoint_(RIGHT_HAND),
    origin_(BOT),
    alignment_(MID),
    snapSource_(NULL),
    drawn_(NULL),
    final_(NULL),
    frameDelay_(1000/5),
    lastFrameAdvance_(0),
    frameProgression_(0)
{ }

Animation::Animation(const Animation& src){
    name_ = src.name_;
    type_ = src.type_;
    snapPoint_ = src.snapPoint_;
    origin_ = src.origin_;
    alignment_ = src.alignment_;
    snapSource_ = src.snapSource_ ;
    drawn_ = NULL;
    final_ = NULL;
    frameDelay_ = src.frameDelay_;
    lastFrameAdvance_ = src.lastFrameAdvance_;
    frameProgression_ = src.frameProgression_;

    frames_ = src.frames_;
    fxFrames_ = src.fxFrames_;
}

Animation& Animation::operator=(const Animation& rhs){
    if( &rhs != this ){
        name_ = rhs.name_;
        type_ = rhs.type_;
        snapPoint_ = rhs.snapPoint_;
        origin_ = rhs.origin_;
        alignment_ = rhs.alignment_;
        snapSource_ = rhs.snapSource_ ;
        drawn_ = NULL;
        final_ = NULL;
        frameDelay_ = rhs.frameDelay_;
        lastFrameAdvance_ = rhs.lastFrameAdvance_;
        frameProgression_ = rhs.frameProgression_;

        frames_ = rhs.frames_;
        fxFrames_ = rhs.fxFrames_;
    }
    return *this;
}

Animation::~Animation(){
    //destroy the fxes
    for( ActiveFxIterator itr = activeFx_.begin(); itr != activeFx_.end(); ++itr ){
        itr->second->stop();
        delete itr->second;
        itr->second = NULL;
    }
    if( drawn_ != NULL ){
        SDL_FreeSurface(drawn_);
        drawn_ = NULL;
    }
    if( final_ != NULL ){
        SDL_FreeSurface(final_);
        final_ = NULL;
    }
}


//  Skips one frame in animation
void Animation::frameForward(){
    if( frames_.empty() ){ return; }

    //move forward a frame
    ++frameProgression_;
    if( frameProgression_ > frames_.size()-1 ){
        frameProgression_ = 0;
    }
    //make sure the sizes are corect
    if( fxFrames_.size() != frames_.size() ){
        std::cerr << __FILE__ << " " << __LINE__ << " fx/frame vector size fail." << std::endl;
        return;
    }
    //run fxControl
    std::string fxId = fxFrames_.at(frameProgression_).getId();
    switch( fxFrames_.at(frameProgression_).getAction() ){
        case AnimFxControl::NOTHING:
            break;
        case AnimFxControl::START:
            //start effect
            {
                //get the fx object
                std::auto_ptr<AnimFx> newFx = fxFrames_.at(frameProgression_).getFx();
                //give it an id
                newFx->setId( fxId );
                //check if it is already active
                ActiveFxIterator itr = activeFx_.find( fxId );
                if( itr == activeFx_.end() ){
                    //add too active list
                    itr = activeFx_.insert( std::pair<std::string, AnimFx*>(fxId, newFx.release()) );
                    itr->second->start();
                } else {
                    std::cerr << __FILE__ << " " << __LINE__ << " fx " << fxId << " already active" << std::endl;
                    //auto_ptr destroys newFx
                    return;
                }
            }
            break;

        case AnimFxControl::STOP:
            {
                //stop effect(s)
                //check if effect is active
                ActiveFxIterator itr = activeFx_.find( fxId );
                while( itr != activeFx_.end() ){
                    //remove fx
                    itr->second->stop();
                    delete itr->second;
                    itr->second = NULL;
                    activeFx_.erase(itr);
                    ActiveFxIterator itr = activeFx_.find( fxId );
                }
            }
        default:
            std::cerr << __FILE__ << " " << __LINE__ << " fx " << fxId << " has ukown action" << std::endl;
            return;
    }
    //cant un run effects =/
}

//  Skip "numberOfFrames" number of frames
void Animation::frameForward(unsigned numberOfFrames){
    if( frames_.empty() ){ return; }
    for(size_t i = 0; i < numberOfFrames; ++i ){
        frameForward();
    }
}

//  Rewinds one frame backward in animation
void Animation::frameBackwards(){
    if( frames_.empty() ){ return; }

    --frameProgression_;
    //make sure the sizes are corect
    if( fxFrames_.size() != frames_.size() ){
        std::cerr << __FILE__ << " " << __LINE__ << " fx/frame vector size fail." << std::endl;
        return;
    }
    //make sure the sizes are corect
    if( fxFrames_.size() != frames_.size() ){
        std::cerr << __FILE__ << " " << __LINE__ << " fx/frame vector size fail." << std::endl;
        return;
    }
    //unrun fxControl
    std::string fxId = fxFrames_.at(frameProgression_).getId();
    switch( fxFrames_.at(frameProgression_).getAction() ){
        case AnimFxControl::NOTHING:
            break;
        case AnimFxControl::START:
            {
                //stop effect(s)
                //check if effect is active
                ActiveFxIterator itr = activeFx_.find( fxId );
                while( itr != activeFx_.end() ){
                    //remove fx
                    itr->second->stop();
                    delete itr->second;
                    itr->second = NULL;
                    activeFx_.erase(itr);
                    ActiveFxIterator itr = activeFx_.find( fxId );
                }
            }
        case AnimFxControl::STOP:
            //if its alredy ben stoped, strangs things may happen if we restrart an effect
            /* //start
            //get the fx object
            auto_ptr<AnimFx*> newFx = fxFrames_.at(frameProgression).getFx();
            //give it an id
            newFx->setId( fxId );
            //check if it is already active
            if( activeFx_.find( fxId ) == activeFx_.end() ){
                //add too active list
                activeFx_[fxId] = newFx.release();
                activeFx_[fxId].start();
            } else {
                std::cerr << __FILE__ << " " << __LINE__ << " fx " << fxId << " already active" << std::endl;
                //auto_ptr destroys newFx
                return;
            }
            break;*/
        default:
            std::cerr << __FILE__ << " " << __LINE__ << " fx " << fxId << " has ukown action" << std::endl;
            return;
    }
    //run FX(es)
    for( ActiveFxIterator itr = activeFx_.begin(); itr != activeFx_.end(); ++itr ){
        itr->second->run();
    }
}

//  Rewinds "numberOfFrames" number of frames backwards
void Animation::frameBackwards(unsigned numberOfFrames){
    if( frames_.empty() ){ return; }

    for( size_t i = 0; i < numberOfFrames; ++i ){
        frameBackwards();
    }
}



//  Sets name of animation
void Animation::setName(const std::string& newName){ name_ = newName; }

//  Sets animation type
void Animation::setType(enum AnimType type){ type_ = type; }

//  sets the snap point
void Animation::setSnapPoint(enum SnapPoint snapPoint){ snapPoint_ = snapPoint; }

//  sets origin of animation image
void Animation::setOrigin(enum KeyPoint origin){ origin_ = origin; }

//  sets aligenment of the animation to snap point
void Animation::setAlignment(enum KeyPoint align){ alignment_ = align; }

//  sets image of object being animated
void Animation::setImage(const TSprite& image){
    image_ = image;

    if( drawn_ != NULL ){
        SDL_FreeSurface(drawn_);
        drawn_ = NULL;
    }
    drawn_ = createSurface(image_.getWidth(), image_.getHeight(), 32, SDL_HWSURFACE);
}

//  Sets the object which it will snap to
void Animation::setSnapSource(Tile* snapSource){ snapSource_ = snapSource; }

//  Sets the frame number
void Animation::setFrameProgression(unsigned frameNumber){
    if( frames_.empty() ){
        frameProgression_ = 0;
        return;
    }

    if ( frameNumber > frames_.size()-1 ){
        frameProgression_ = frames_.size()-1;
    } else {
        frameProgression_ = frameNumber;
    }
}

//  Sets speed of animation (in fps)
void Animation::setFPS(unsigned int FPS){
    if( FPS != 0 ){
        frameDelay_ = 1000/FPS;
    }
}





//  gets anaimation name
const std::string& Animation::getName()const{ return name_; }

//  gets animation type
enum Animation::AnimType Animation::getType()const{ return type_; }

//  gets the snap point
enum Animation::SnapPoint Animation::getSnapPoint()const{ return snapPoint_; }

//  gets origin of animation image
enum Animation::KeyPoint Animation::getOrigin()const{ return origin_; }

//  gets aligenment of the animation to snap point
enum Animation::KeyPoint Animation::getAlignment()const{ return alignment_; }

//  Gets the object which it will snap to
Tile* Animation::getSnapSource()const{ return snapSource_; }

//  sets image of object being animated
const TSprite& Animation::getImage()const{ return image_; }

//  Returns the number of frame
int Animation::getFrameCount()const{ return frames_.size(); }

//  Returns the speed of the sprite (in FPS)
int Animation::getFPS()const{
    if( frameDelay_ == 0 ){ return 0; }
    return (int)(1000/frameDelay_);
}

//  Returns which frame the animation is up to
int Animation::getFrameProgression(){
    advanceFrames();
    return frameProgression_;
}


//advances frames according to FPS
void Animation::advanceFrames(){
    if( frames_.empty() ){ return; }

    if( frameDelay_ != 0 ){
        //calculate no frames to advance
        int framesBehind = (SDL_GetTicks()-lastFrameAdvance_)/frameDelay_;
        if( framesBehind > 0 ){
            //advance
            frameForward( framesBehind );
            //set last forward time
            lastFrameAdvance_ = SDL_GetTicks();
        }
    }
}



//save animation to
bool Animation::save( inp::INFPacket& pack )const{
    pack << revision_;
    pack << name_;
    pack << type_;
    pack << snapPoint_;
    pack << origin_;
    pack << alignment_;
    pack << image_.getFilename();

    pack << frameDelay_;

    pack << getFrameCount();

    for( int i = 0; i < getFrameCount(); ++i ){
        pack << frames_.at(i).savePacket();
        pack << fxFrames_.at(i).savePacket();
    }

    return true;
}


//save animation to file
bool Animation::save( const std::string& filename )const{
    if( !getFrameCount() ){ return 0; }

    //open file
    std::string fixedFn = correctFilepath(filename);
    std::ofstream file( fixedFn.c_str(), std::ios::out|std::ios::binary );
    if( (!file.good()) || (!file.is_open()) ){
        file.close();
        std::cout << "Unable to open file \"" << fixedFn << "\"" << std::endl;
        return -1;
    }

    inp::INFPacket pack;
    save(pack);

    file.write( (char*)pack.getBuf(), pack.getLength() );
    file.close();

    return true;
}



// load animation from
bool Animation::load( inp::INFPacket& pack ){
    //empty out old frames
    clear();

    //  check version
    int fileVer;
    pack >> fileVer;
    if( fileVer != revision_ ){
        std::cerr << __FILE__ << __LINE__ << ": Animation: incompatible file version" << std::endl;
        return 0;
    }

    pack >> name_;
    if( pack.readDone() ){ return 0; }

    int enum_var;
    pack >> enum_var;
    if(!validAnimType(enum_var)){ return 0; }
    type_ = static_cast<AnimType>(enum_var);
    if( pack.readDone() ){ return 0; }

    pack >> enum_var;
    if(!validSnapPoint(enum_var)){ return 0; }
    snapPoint_ = static_cast<SnapPoint>(enum_var);
    if( pack.readDone() ){ return 0; }

    pack >> enum_var;
    if(!validKeyPoint(enum_var)){ return 0; }
    origin_ = static_cast<KeyPoint>(enum_var);
    if( pack.readDone() ){ return 0; }

    pack >> enum_var;
    if(!validKeyPoint(enum_var)){ return 0; }
    alignment_ = static_cast<KeyPoint>(enum_var);
    if( pack.readDone() ){ return 0; }

    std::string fn;
    pack >> fn;
    //if(!image_.loadXml(fn)){return 0; }
    if( pack.readDone() ){ return 0; }

    pack >> frameDelay_;
    if( pack.readDone() ){ return 0; }

    int frameCount;
    pack >> frameCount;

    AnimFrame frame;
    AnimFxControl fxFrame;

    for( int i = 0; i < frameCount; ++i ){
        if( pack.readDone() ){ return 0; }
        frame.loadPacket(pack);
        if( pack.readDone() ){ return 0; }
        fxFrame.loadPacket(pack);

        frames_.push_back(frame);
        fxFrames_.push_back(fxFrame);
    }

    return true;
}


// load animation from file.
bool Animation::load( const std::string& filename ){
    //open file
    std::string fixedFn = correctFilepath(filename);
    std::ifstream file( fixedFn.c_str(), std::ios::in|std::ios::binary );
    if( (!file.good()) || (!file.is_open()) ){
        file.close();
        std::cout << "Unable to open file \"" << fixedFn << "\"" << std::endl;
        return 0;
    }
    //get filesize
    size_t filesize = getFileSize(file);

    // read file into packet
    inp::INFPacket pack;
    pack.readFile( file, filesize );
    file.close();

    return load(pack);
}


//  load animation from a packet
bool Animation::load( const ResourcePack& pack, const std::string& filepath){
    std::string fixedFn = makeUnixFilePath(filepath);
    ResourceFile rcf;
    if( !pack.getResource(fixedFn, rcf) ){
        std::cerr << __FILE__ << ": " << __LINE__ << ": Requested file \"" << fixedFn << "\" not found in supplied pack" << std::endl;
        return false;
    }

    filename_ = correctFilepath(fixedFn);
    // read file into packet
    inp::INFPacket packet;
    packet.readBuf( const_cast<unsigned char*>(&pack.getRawData()[rcf.start]), rcf.size );

    return load(packet);
}





//draws
void Animation::draw(SDL_Surface *dest, int x, int y){
    //advance the frames
    advanceFrames();

    //calculate xy offsets
    int xOffset = 0;
    int yOffset = 0;
    //calc rotozoom
    int angle = 0;
    float zoom = 1;
    //add offsets from source origin (eg.sword)
    switch(origin_){
        case TL:
            //no offset
            break;

        case TOP:
            xOffset += static_cast<int>( image_.getWidth()*.5f );
            break;

        case TR:
            xOffset += image_.getWidth();
            break;

        case LEFT:
            yOffset += static_cast<int>( image_.getHeight()*.5f );
            break;

        case MID:
            xOffset += static_cast<int>( image_.getWidth()*.5f );
            yOffset += static_cast<int>( image_.getHeight()*.5f );
            break;

        case RIGHT:
            xOffset += image_.getWidth();
            yOffset += static_cast<int>( image_.getHeight()*.5f );
            break;

        case BL:
            yOffset += image_.getHeight();
            break;

        case BOT:
            xOffset += static_cast<int>( image_.getWidth()*.5f );
            yOffset += image_.getHeight();
            break;

        case BR:
            xOffset += image_.getWidth();
            yOffset += image_.getHeight();
            break;

        default:
            std::cerr << __FILE__ << " " << __LINE__ << " bad origin" << std::endl;
            break;
    }

    //add offsets from target alignment (eg.hand)
    if( snapSource_ != NULL ){
        switch(origin_){
            case TL:
                //no offset
                break;

            case TOP:
                xOffset += static_cast<int>( snapSource_->getWidth()*.5f );
                break;

            case TR:
                xOffset += snapSource_->getWidth();
                break;

            case LEFT:
                yOffset += static_cast<int>( snapSource_->getHeight()*.5f );
                break;

            case MID:
                xOffset += static_cast<int>( snapSource_->getWidth()*.5f );
                yOffset += static_cast<int>( snapSource_->getHeight()*.5f );
                break;

            case RIGHT:
                xOffset += snapSource_->getWidth();
                yOffset += static_cast<int>( snapSource_->getHeight()*.5f );
                break;

            case BL:
                yOffset += snapSource_->getHeight();
                break;

            case BOT:
                xOffset += static_cast<int>( snapSource_->getWidth()*.5f );
                yOffset += snapSource_->getHeight();
                break;

            case BR:
                xOffset += snapSource_->getWidth();
                yOffset += snapSource_->getHeight();
                break;

            default:
                std::cerr << __FILE__ << " " << __LINE__ << " bad align" << std::endl;
                break;
        }
    }

    //add offsets from AnimFrame info (if needed)
    if( !frames_.empty() ){
        switch(type_){
            case ITEM:
            case CHARACTER_PART:
                xOffset += frames_.at(frameProgression_).getX();
                yOffset += frames_.at(frameProgression_).getY();
                break;

            case OBJECT:
                //set as abosolute positions
                xOffset = -x;
                yOffset = -y;
                xOffset += frames_.at(frameProgression_).getX();
                yOffset += frames_.at(frameProgression_).getY();

            default:
                std::cerr << __FILE__ << " " << __LINE__ << " bad anim type" << std::endl;
                break;
        }
        angle = frames_.at(frameProgression_).getAngle();
        zoom = frames_.at(frameProgression_).getZoom();
    }

    if( (angle!=0) || (zoom!=1) ){
        //do rotozooming
        //draw to intemediate
        image_.draw( drawn_, 0, 0 );
        //roto zoom
        if( final_ != NULL ){ SDL_FreeSurface(final_); }
        final_ = rotozoomSurface(drawn_, angle, zoom, true);
        //draw to screen
        drawSurface(final_, dest, x+xOffset, y+yOffset);
    } else {
        //draw direct to screen
        image_.draw(dest, x+xOffset, y+yOffset);
    }
}


//add an Fx Control event
void Animation::addFrame(const AnimFrame& newFrame, const AnimFxControl& newFx){
    frames_.push_back(newFrame);
    fxFrames_.push_back(newFx);
}

//removes frame/fx event
void Animation::remove(size_t i){
    if( (i > 0) && (i < frames_.size()) ){
        std::vector<AnimFrame>::iterator frame_itr = frames_.begin();
        std::vector<AnimFxControl>::iterator fx_itr = fxFrames_.begin();
        std::advance(frame_itr, i);
        std::advance(fx_itr, i);
        frames_.erase(frame_itr);
        fxFrames_.erase(fx_itr);
    }
}

//clear
void Animation::clear(){
    name_ = "- Cleared Out Animiation -";
    snapSource_ = NULL;

    frameDelay_ = 0;
    lastFrameAdvance_ = 0;
    frameProgression_ = 0;

    frames_.clear();
    fxFrames_.clear();

    activeFx_.clear();
}


//returns a reference to a frame
AnimFrame* Animation::getFrame(size_t i){
    if( !(i < 0) && (i < frames_.size()) ){
        return &frames_.at(i);
    }
    return NULL;
}

//returns a reference to an Fx frame
AnimFxControl* Animation::getFxControl(size_t i){
    if( !(i < 0) && (i < frames_.size()) ){
        return &fxFrames_.at(i);
    }
    return NULL;
}
