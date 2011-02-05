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


#ifndef RESOURCEPACK_HPP_INCLUDED
#define RESOURCEPACK_HPP_INCLUDED

//******************************************************************************
//
//                              ResourcePack
//
//  This is the resource pack class, it is capable to creating/loading/editing
//  Infraelly resource files.
//
//  ResourcePack allows transparrent switching between loading resources from
//  physical files, and loading from an InfraellyResourceFile(.irf).
//
//  To tell ResourcePack to load files from a pack, define USE_PACKED_RESOURCES
//  To tell ResourcePack to laod files form physical files, undef
//        USE_PACKED_RESOURCES
//
//  All pack editing is always done in memory. SavePack() functions to write
//  to file
//
//******************************************************************************

#include <fstream>
#include <string>
#include <map>
#include <vector>
//size_t
#include <cstddef>
//Sha1 is used for file integrity checks
#include <cryptopp/sha.h>
using CryptoPP::SHA1;
#include "globalFunc.hpp"
#include "ResourceFile.hpp"


class ResourcePack{
    private:
        static const int revision_ = 1;

        //  Error Values used as return values
        enum ResourcePackError{
            FILE_NOT_FOUND,
            FILE_ALREADY_EXIST,
            PACK_NOT_FOUND,
            BAD_HASH,
            ABRUPT_FILE_END,
            ABRUPT_PACK_END,
            BAD_PACK_VER,
            GOOD
        };
        mutable enum ResourcePackError errorState;


    public:
        //  The ResourceRootDir is the root directory where all resources are stored
        //  eg.
        //  Infraelly
        //    +Resources
        //      + Audio
        //         + Music
        //         + Sound
        //      + Images
        //  The ResourceRootDir is the "data/"
        ResourcePack();
        ResourcePack(const std::string& packfilepath, const std::string& rootDir = "data/", bool load = true);
        ~ResourcePack();

        //  Returns a string value corresponding to the error value passed
        std::string getErrorString(enum ResourcePackError error)const;


        //  Will discard all old pack data, making all ResourceFile's
        //  invalid up to this point, Then load data from the pack specified
        //  Returns 0 on fail
        bool loadPack(const std::string& packPath);


        //  Retrieves the requested file, and stores a ResourceFile into "dest"
        //  Returns true on success
        bool getResource( std::string filepath, ResourceFile& dest )const;

        //  Stores the requested file in "dest" as a string (useful if
        //  retrieving a text file).
        //  Returns true on success
        bool getResourceText( std::string filepath, std::string& dest )const;

        //  Dumps a resource to a physical file
        //  returns tru on success
        bool dumpResource( std::string resourceFilepath, const std::string& dumpFilepath )const;
        //  Dumps index to physical file
        //  returns tru on success
        bool dumpIndex( const std::string& dumpFilepath )const;

        //  Returns true if the file can be found (exists).
        bool findFile( std::string filepath )const;

        //  Checks integrity of a file
        //  Returns true if file is good
        bool checkFile( std::string filepath )const;

        //  Checks integrity entire Pack (can be REALY slow)
        //  Returns true if all files in pack are good
        bool checkPack()const;


        //  Adds file to Pack
        //  Returns true on success
        bool addFile( std::string filepath );

        //  Removes file from pack (will not delete physical files)
        //  Returns true on success
        bool removeFile( std::string filepath );


        //  Writes the pack to a physical file
        //  Returns true on success
        bool writePack( const std::string& filepath, bool overwrteOld = true );


        const std::vector<unsigned char>& getRawData()const { return fileData_; }


    private:
        //builds header
        void buildheader();

        //  Returns the position where the index of the file ends
        //  (handy when adding things to end of index...)
        //  Returns 0 on fail
        std::ifstream::pos_type getEndIndexPos()const;

        //  Returns the position where
        std::ifstream::pos_type getEndLastFilePos()const;

        //  typdefs to make life easier
        typedef std::map<std::string, ResourceFile> FileIndex;
        typedef FileIndex::iterator FileIndexIterator;
        typedef std::vector<unsigned char> RawData;
        typedef RawData::iterator RawDataIterator;

        std::string rcRoot_;                            //root dir of pack
        mutable FileIndex index_;                       //store index of pack
        RawData fileData_;                              //store pack contents

        //  Text that appears at beginning of pack. Contains a copyright notice
        static const char copyRightNotice[512];
        //  String which denotes the version of the ResourcePack class
        static const char version[11];
        //  Size of the hash used for file veryfication
        static const size_t hashSize;
        //  Size of the header (copyright note + version num)
        static const size_t headerSize;
        //  Offset of the version string, from beginning of file
        static const std::ifstream::pos_type version_loc;
        //  Offset of the version string, from beginning of file
        static const std::ifstream::pos_type numFiles_loc;
};


#endif // RESOURCEPACK_HPP_INCLUDED
