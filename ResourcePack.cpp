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


#include "ResourcePack.hpp"

#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

//we use SDL data types
#include "SDL/SDL.h"
//use SDL_net's endian features
#include "SDL/SDL_net.h"

#include "globalFunc.hpp"

#include "zlib.h"


//extra byte for the '\0' (although its not written to file)
const char ResourcePack::copyRightNotice[512] =
"This file, and its contents, is the property of the Infraelly Developers. By \
opening this file you have “reverse engineered”, and or, “decompiled” a part of\
 Infraelly. This violates the EULA you agreed to upon obtaining, and or, using \
 Infraelly. You have committed a criminal act and can be persecuted in the \
 court of law. Close this file immediately.";

//extra byte for the '\0' (although its not written to file)
const char ResourcePack::version[11] = "01.100.000";

const size_t ResourcePack::hashSize = SHA1().DigestSize();
const size_t ResourcePack::headerSize = 524;
const std::ifstream::pos_type ResourcePack::version_loc = 512;
const std::ifstream::pos_type ResourcePack::numFiles_loc = 522;



ResourcePack::ResourcePack() :
    errorState(GOOD),
    rcRoot_(correctFilepath("data/"))
{
    buildheader();
}

ResourcePack::ResourcePack(const std::string& packfilepath, const std::string& rootDir, bool load) :
    errorState(GOOD),
    rcRoot_(correctFilepath(packfilepath))
{
    if(load){
        //  Use pack
        if(  !loadPack( correctFilepath(packfilepath) )  ){
            buildheader();
        }
    } else {
        buildheader();
    }
}

void ResourcePack::buildheader(){
    //prepare header with fileData_
    fileData_.resize(headerSize);
    memcpy(&fileData_[0], copyRightNotice, 512);
    memcpy(&fileData_[version_loc], version, 10);
    //change the number of files in FileData
    SDLNet_Write16( 0, &fileData_[numFiles_loc]);
}



ResourcePack::~ResourcePack(){}



std::string ResourcePack::getErrorString(enum ResourcePackError error)const{
    switch( error ){
        case FILE_NOT_FOUND:
            return "File not found";
        case PACK_NOT_FOUND:
            return "Pack not found";
        case BAD_HASH:
            return "Bad hash";
        case ABRUPT_FILE_END:
            return "Abrupt end of file";
        case ABRUPT_PACK_END:
            return "Abrupt end of pack";
        case BAD_PACK_VER:
            return "Incompatible pack version";
        case GOOD:
            return "Opperation success";
        default:
            return "Unknown Error Code";
    }
}


bool ResourcePack::loadPack(const std::string& packPath){
    #ifdef USE_PACKED_RESOURCES
        //  Use pack
        //open pack and read into data
        std::ifstream rcFile( correctFilepath(packPath).c_str(), std::ios::binary|std::ios::in );
        if( !rcFile ){
            errorState = FILE_NOT_FOUND;
            return false;
        }
        std::ifstream::pos_type filesize = getFileSize(rcFile);
        if( filesize < headerSize ){ return 0; }
        //prepare the data buf
        fileData_.clear();
        fileData_.resize(filesize);
        //read file to memory
        rcFile.read( (char*)&fileData_[0], filesize );
        //close file
        rcFile.close();


        //read loc
        std::ifstream::pos_type read_loc = 0;
        //skip 512 bytes (copyright notice)
        read_loc += 512;

        //check the version string
        char loadVersion[11];
        loadVersion[10] = '\0'; //make null ternimated str from input
        memmove( loadVersion, &fileData_[read_loc], 10 );
        read_loc += 10;
        std::string loadVersionStr = loadVersion;
        if( version != loadVersionStr ){ return 0; }

        //read number of files
        Uint16 numFiles = SDLNet_Read16(&fileData_[read_loc]);
        read_loc += 2;


        //starting reading the index
        for( unsigned int i = 0; i < numFiles; ++i ){
            //read length of filename
            if( read_loc > fileData_.size() ){ errorState = ABRUPT_PACK_END; return 0; }
            Uint16 len = SDLNet_Read16(&fileData_[read_loc]);
            read_loc += 2;
            //read filename
            if( read_loc > fileData_.size() ){ errorState = ABRUPT_PACK_END; return 0; }
            char fn[len+1];
            fn[len] = '\0';//make null ternimated str from input
            memmove( fn, &fileData_[read_loc], len );
            read_loc += len;
            std::string filename = fn;
            //read start(offset) of file
            if( read_loc > fileData_.size() ){ errorState = ABRUPT_PACK_END; return 0; }
            Uint32 start = SDLNet_Read32(&fileData_[read_loc]);
            read_loc += 4;
            //read size of file
            if( read_loc > fileData_.size() ){ errorState = ABRUPT_PACK_END; return 0; }
            Uint32 size = SDLNet_Read32(&fileData_[read_loc]);
            read_loc += 4;
            //read SHA1 hash
            if( read_loc > fileData_.size() ){ errorState = ABRUPT_PACK_END; return 0; }
            byte hash_buf[hashSize];
            memmove( hash_buf, &fileData_[read_loc], hashSize );
            read_loc += hashSize;

            //create a ResourceFile and add to index
            ResourceFile rcFile;
            rcFile.start = start-headerSize   ;
            rcFile.size = size;
            memmove( &rcFile.hash[0], hash_buf, hashSize );
            rcFile.filepath = fn;

            //add to index
            index_[filename] = rcFile;
        }

        //index read, remove from fileData. this keeps the file offsets right
        RawDataIterator start = fileData_.begin();
        advance(start, read_loc);
        RawData tmp(start, fileData_.end());
        fileData_ = tmp;

        errorState = GOOD;
        return 1;
    #else
        //  Use physical files
        errorState = GOOD;
        return 1;
    #endif
}



//  Retrieves the requested file, and stores a ResourceFile into "dest"
bool ResourcePack::getResource( std::string filepath, ResourceFile& dest )const
{
    #ifdef USE_PACKED_RESOURCES
        makeUnixFilePath(filepath);
        //  Use pack
        if( findFile(filepath) == 0 ){
            errorState = FILE_NOT_FOUND;
            return 0;
        }

        dest = index_[filepath];
        errorState = GOOD;
        return 1;
    #else
        //  Use physical files
        if( findFile(correctFilepath(rcRoot_+"/"+filepath)) == 0 ){
            if( addFile(filepath) == 0 ){
                return 0;
            }
        }

        dest = index_[filepath];
        errorState = GOOD;
        return 1;
    #endif
}


//  Stores the requested file in "dest" as a string (useful if
//  retrieving a text file).
bool ResourcePack::getResourceText( std::string filepath, std::string& dest )const
{
    #ifdef USE_PACKED_RESOURCES
        makeUnixFilePath(filepath);
        //  Use pack
        if( findFile(filepath) == 0 ){ return 0; }

        char fileStr[index_[filepath].size+1];
        fileStr[index_[filepath].size+1] = '\0';

        memcpy( fileStr, &fileData_[index_[filepath].start], index_[filepath].size );
        dest = fileStr;

        errorState = GOOD;
        return 1;
    #else
        //  Use physical files
        std::ifstream rcFile( correctFilepath(rcRoot_+"/"+filepath).c_str(), std::ios::ascii|std::ios::in );
        //open file to return
        if( !rcFile ){
            errorState = FILE_NOT_FOUND;
            return false;
        }

        //  Get filesize
        std::ifstream::pos_type filesize = getFileSize(rcFile);

        std::ostringstream fileStr;
        fileStr << rcFile;

        dest = fileStr.str();

        errorState = GOOD;
        return 1;
    #endif
}


//  Dumps a resource to a physical file
bool ResourcePack::dumpResource( std::string resourceFilepath, const std::string& dumpFilepath )const{
    #ifdef USE_PACKED_RESOURCES
        makeUnixFilePath(resourceFilepath);
        //  Use pack
        //  See if file alredy in pack
        if( index_.find(resourceFilepath) == index_.end() ){
            errorState = FILE_NOT_FOUND;
            return 0;
        }

        ResourceFile rc;
        getResource(resourceFilepath, rc);
        std::string rcFn( resourceFilepath, resourceFilepath.find_last_of("/")+1,
                          resourceFilepath.length()-resourceFilepath.find_last_of("/") );
        std::string dumpFn = makeUnixFilePath(dumpFilepath);

        std::ofstream of_file( dumpFn.c_str(), std::fstream::binary);
        if(of_file.is_open()){
            of_file.write( (char*)&getRawData().at(rc.start), rc.size);
            of_file.flush();
            of_file.close();
        } else {
            std::cerr << __FILE__ << ": " << __LINE__ << ": Failed to dump \"" + resourceFilepath + " to \"" + dumpFn << std::endl;
            errorState = PACK_NOT_FOUND;
            return 0;
        }


        errorState = GOOD;
        return 1;
    #else
        return 1;
    #endif
}

bool ResourcePack::dumpIndex( const std::string& dumpFilepath )const{
    std::ofstream of_file( correctFilepath(dumpFilepath).c_str() );
    if(of_file.is_open()){
        for(FileIndexIterator itr = index_.begin(); itr != index_.end(); ++itr){
            of_file << itr->second.filepath << std::endl;
        }
        of_file.flush();
        of_file.close();
    } else {
        std::cerr << __FILE__ << ": " << __LINE__ << ": Failed to dump index" << std::endl;
        errorState = BAD_HASH;
        return 0;
    }
    return 1;
}


//  Returns true if the file can be found (exists).
bool ResourcePack::findFile( std::string filepath )const{
    #ifdef USE_PACKED_RESOURCES
        makeUnixFilePath(filepath);
        //  Use pack
        //  See if file alredy in pack
        if( index_.find(filepath) == index_.end() ){
            errorState = FILE_NOT_FOUND;
            return 0;
        }

        errorState = GOOD;
        return 1;
    #else
        //  Use physical files
        //  if we can get filesize, it exists, so all good
        return getFileSize(correctFilepath(rcRoot_+"/"+filepath));
    #endif
}


//  Checks integrity of a file
//  Returns true if file is good
bool ResourcePack::checkFile( std::string filepath )const{
    #ifdef USE_PACKED_RESOURCES
        makeUnixFilePath(filepath);
        //  Use pack
        //  See if file alredy in pack
        if( index_.find(filepath) == index_.end() ){
            return 0;
        }

        //rehash file
        std::vector<byte> hash(hashSize);
        SHA1().CalculateDigest( &hash[0],
                                (byte*)&fileData_[index_[filepath].start],
                                index_[filepath].size );

        //check new hash against hash in index
        if( hash == index_[filepath].hash ){
            return 1;
        } else {
            return 0;
        }
    #else
        //  Use physical files
        //  if we can get filesize, it exists, so all good
        return getFileSize(correctFilepath(rcRoot_+"/"+filepath));
    #endif
}


//  Checks integrity entire Pack (can be REALY slow)
//  Returns true if all files in pack are good
bool ResourcePack::checkPack()const{
    #ifdef USE_PACKED_RESOURCES
        //  Use pack
        //  index while writting out. no compatibility lost, so no problem

        //iterate through the index, and check the integrity of each file
        for(FileIndexIterator itr = index_.begin(); itr != index_.end(); ++itr){
            if( !checkFile(itr->second.filepath) ){ return 0; }
        }
        return 1;
    #else
        //  Use physical files
        //meh, no pack to check realy.
        errorState = GOOD;
        return 1;
    #endif
}


//  Adds file to Pack
bool ResourcePack::addFile( std::string filepath ){
    filepath = correctFilepath(filepath);
    //open file to add to pack
    std::ifstream rcFile( (rcRoot_+"/"+filepath).c_str(), std::ios::binary|std::ios::in );
    if( !rcFile ){
        errorState = FILE_NOT_FOUND;
        return false;
    }
    makeUnixFilePath( filepath );

    //  Get filesize
    std::ifstream::pos_type filesize = getFileSize(rcFile);

    //  See if file alredy in pack
    if( index_.find(filepath) != index_.end() ){
        rcFile.close();
        errorState = GOOD;
        return 1;
    }

    //create a ResourceFile and add to index
    ResourceFile rcFileEntry;
    rcFileEntry.start = getEndLastFilePos();
    rcFileEntry.size = filesize;
    rcFileEntry.filepath = filepath;


    //make some space for the file on fileData_
    fileData_.resize( fileData_.size() + filesize, NULL );
    //load file into filedata
    rcFile.read( (char*)&fileData_[rcFileEntry.start], filesize );

    //calculate the hash
    SHA1().CalculateDigest( &rcFileEntry.hash[0],
                            (byte*)(&fileData_[rcFileEntry.start]),
                            filesize );

    //add to index
    index_[filepath] = rcFileEntry;

    //change the number of files in FileData
    SDLNet_Write16( SDLNet_Read16( &fileData_[numFiles_loc] )+1, &fileData_[numFiles_loc]);

    errorState = GOOD;
    rcFile.close();
    return 1;
}


//  Removes file from pack
bool ResourcePack::removeFile( std::string filepath ){
    #ifdef USE_PACKED_RESOURCES
        makeUnixFilePath(filepath);
        //  Use pack
        if( index_.find(filepath) != index_.end() ){
            ResourceFile rc;
            getResource(filepath, rc);
            //remove file from filedata
            RawDataIterator start = fileData_.begin();
            advance(start, rc.start);
            RawDataIterator end = start;
            advance(end, rc.size);
            fileData_.erase( start, end);

            //remove file from the index
            index_.erase(filepath);
            //change the number of files in FileData
            if( SDLNet_Read16(&fileData_[numFiles_loc]) != 0 ){
                SDLNet_Write16( SDLNet_Read16(&fileData_[numFiles_loc])-1 , &fileData_[numFiles_loc]);
            }
            errorState = GOOD;
            return 1;
        } else {
            errorState = FILE_NOT_FOUND;
            return 1;
        }
    #else
        errorState = GOOD;
        return 1;
    #endif
}


//  Writes the pack to a physical file
//  Returns true on success
bool ResourcePack::writePack( const std::string& filepath, bool overwriteOld ){
    //  Check if anything to write
    if( fileData_.empty() || (fileData_.size() == headerSize) ){ return 1; }

    //  If we're not supposed to overwrite the an old file, check if an old one
    //  exists
    if( !overwriteOld ){
        //  check if file exists
        //if we can get filesize, it exists, so all good
        if( getFileSize(correctFilepath(filepath)) ){
            errorState = FILE_ALREADY_EXIST;
            return 0;
        }
    }

    //open file
    std::ofstream file( correctFilepath(filepath).c_str(), std::ios::out|std::ios::binary );
    if( !(file.good() && file.is_open()) ){
        file.close();
        errorState = FILE_NOT_FOUND;
        return 0;
    }

    //write header
    file.write( (char*)&fileData_[0], headerSize );

    //!@# maybe at some later stage, we can sort and reconstruct alphabeticaly?

    //write index
    char buff[hashSize];
    for( FileIndexIterator itr = index_.begin(); itr != index_.end(); ++itr ){
        std::string filename = itr->first;
        makeUnixFilePath(filename);
        //write length of filename
        SDLNet_Write16(filename.length(), buff);
        file.write(buff, 2);
        //write filename
        char fn[filename.length()+1];
        fn[filename.length()] = '\0';//make null ternimated str from input
        memmove( fn, filename.c_str(), filename.length() );
        file.write(fn, filename.length());
        //write start(offset) of file
        SDLNet_Write32(itr->second.start, buff);
        file.write(buff, 4);
        //write size of file
        SDLNet_Write32(itr->second.size, buff);
        file.write(buff, 4);
        //write SHA1 hash
        memmove( buff, &itr->second.hash[0], hashSize );
        file.write(buff, hashSize);
    }

    //compress();

    //  Write contents of filedata
    file.write( (char*)&fileData_[headerSize], fileData_.size()-headerSize );
    file.flush();

    //close the file =]
    file.close();

    errorState = GOOD;
    return 1;
}







std::ifstream::pos_type ResourcePack::getEndIndexPos(){
    #ifdef USE_PACKED_RESOURCES
        //  Use pack
        if( fileData_.size() < headerSize ){ return 0; }

        //read loc
        std::ifstream::pos_type read_loc = numFiles_loc;

        //number of files
        Uint16 numFiles = SDLNet_Read16(&fileData_[read_loc]);

        //starting reading the index
        for( unsigned int i = 0; i < numFiles; ++i ){
            if( read_loc > fileData_.size() ){ errorState = ABRUPT_PACK_END; return 0; }

            //read length of filename
            Uint16 len = SDLNet_Read16(&fileData_[read_loc]);
            read_loc += 2;
            //skip filename
            read_loc += len;
            //skip start(offset) of file
            read_loc += 4;
            //skip size of file
            read_loc += 4;
            //skip SHA1 hash
            read_loc += hashSize;
        }

        return read_loc;
    #else
        //  Use physical files
        return 0;
    #endif
}


std::ifstream::pos_type ResourcePack::getEndLastFilePos(){
    #ifdef USE_PACKED_RESOURCES
        //  Use pack
        //  The last file is really just the end of the file
        return fileData_.size();
    #else
        //  Use physical files
        return 0;
    #endif
}
