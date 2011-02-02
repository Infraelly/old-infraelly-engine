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

#include "CharAnimation.hpp"
#include "caches.hpp"

CharAnimation::CharAnimation() :
    anims(4)
{
    //set up animation objects
    for(size_t i = 0; i < 4; ++i){
        Animation newanim;
        anims[i].assign(6, newanim);
        for(size_t j = 0; j < anims[i].size(); ++j){
            anims[i][j].setOrigin(Animation::TL);
            anims[i][j].setType(Animation::CHARACTER_PART);

            TSprite *image = cache::tsprites.loadResource("tsprites/head1.xml");
            anims[i][HEAD].setImage( *image );

            image = cache::tsprites.loadResource("tsprites/bodyM1.xml");
            anims[i][BODY].setImage( *image );

            image = cache::tsprites.loadResource("tsprites/hand1.xml");
            anims[i][LEFT_HAND].setImage( *image );
            anims[i][RIGHT_HAND].setImage( *image );

            image = cache::tsprites.loadResource("tsprites/feet1.xml");
            anims[i][LEFT_FOOT].setImage( *image );
            anims[i][RIGHT_FOOT].setImage( *image );
        }
    }
}


void CharAnimation::clear(){
    for(size_t i = 0; i < anims.size(); ++i){
        for(size_t j = 0; j < anims[i].size(); ++j){
            anims[i][j].clear();
        }
    }
}


//save animation to packet
bool CharAnimation::save( inp::INFPacket& pack )const{
    pack << revision_;

    for(size_t i = 0; i < anims.size(); ++i){
        for(size_t j = 0; j < 6; ++j){
            if( !anims[i][j].save(pack) ){ return 0; }
        }
    }

    return 1;
}


//save animation to file
bool CharAnimation::save( const std::string& filename )const{
    //open file
    std::string fixedFn = correctFilepath(filename);
    std::ofstream file( fixedFn.c_str(), std::ios::out|std::ios::binary );
    if( (!file.good()) || (!file.is_open()) ){
        file.close();
        std::cout << "Unable to open file \"" << fixedFn << "\"" << std::endl;
        return -1;
    }

    inp::INFPacket pack;

    if( !save(pack) ){ return 0; }

    file.write( (char*)pack.getBuf(), pack.getLength() );
    file.close();

    return true;
}



// load animation from packet
bool CharAnimation::load( inp::INFPacket& pack ){
    //  check version
    int fileVer;
    pack >> fileVer;
    if( fileVer != revision_ ){
        std::cerr << __FILE__ << __LINE__ << ": Char Animation: incompatible file version" << std::endl;
        return 0;
    }

    for(size_t i = 0; i < anims.size(); ++i){
        for(size_t j = 0; j < 6; ++j){
            if( !anims[i][j].load(pack) ){ return 0; }
        }
    }

    return 1;
}

// load animation from file.
bool CharAnimation::load( const std::string& filename ){
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


//  load animation from a data pack
bool CharAnimation::load( const ResourcePack& pack, const std::string& filepath){
    std::string fixedFn = makeUnixFilePath(filepath);
    ResourceFile rcf;
    if( !pack.getResource(fixedFn, rcf) ){
        std::cerr << __FILE__ << ": " << __LINE__ << ": Requested file \"" << fixedFn << "\" not found in supplied pack" << std::endl;
        return false;
    }

    filename_ = fixedFn;
    // read file into packet
    inp::INFPacket packet;
    packet.readBuf( const_cast<unsigned char*>(&pack.getRawData()[rcf.start]), rcf.size );

    return load(packet);
}



void CharAnimation::draw(SDL_Surface *dest, enum Directions facing,int x, int y){
    for(int i = 0; i < 6; ++i){
        anims[facing][i].draw(dest, x, y);
    }
}

void CharAnimation::draw(SDL_Surface *dest, enum Directions facing,int x, int y, int frameNumber){
    for(int i = 0; i < 6; ++i){
        anims[facing][i].draw(dest, x, y, frameNumber);
    }
}
