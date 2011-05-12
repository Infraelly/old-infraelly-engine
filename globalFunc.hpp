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


#ifndef GLOBAL_FUNCT_HPP
#define GLOBAL_FUNCT_HPP

//  globalFunc.hpp

#include <string>
#include <iosfwd>
#include <fstream>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


enum Directions{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};
bool validDirection(int dir);

enum BodyParts{
    HEAD = 0,
    BODY = 1,
    LEFT_HAND = 2,
    RIGHT_HAND = 3,
    LEFT_FOOT = 4,
    RIGHT_FOOT = 5,
    FACE = 6,
    HAIR = 7
};
bool validBodyPart(int part);

//returns an SDL_rect with parameters as given
SDL_Rect makeRect(Sint16 _x, Sint16 _y, Uint16 w, Uint16 h);


//fade effects
//recursivley draws a translucent over lay
void fadeTo(SDL_Surface *dest, const SDL_Color& colour, int timeMS);

//recursivley draws a translucent over lay
void fadeTo(SDL_Surface *dest, SDL_Surface *targetImage, int timeMS);

//manipulate RGB value of every pixel mathemtical to match SUPER SLOW!!!
void rgbFadeTo(SDL_Surface *dest, SDL_Surface *targetImage, int timeMS);

//shadow sufaces (for fading)
//returns a duplicate of source
SDL_Surface *duplicateSurface(SDL_Surface *source, Uint32 flags = NULL);

//creates an empty surface
SDL_Surface *createSurface(int width, int height, int bbp, Uint32 flags);



//pixel drawing (taken from SDL mailing list)
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);


//for draing directly to pixel buffer
void LOCK(SDL_Surface *dest);
void UNLOCK(SDL_Surface *dest);


//returns 1 if collision, else 0
bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2);



//Returns rgb into SDL_Color
SDL_Color rgb(Uint8 r, Uint8 g, Uint8 b);


//draws a (part or whole) surface onto another
// Standard sdl_blit semantics
void drawSurface(SDL_Surface *src, SDL_Surface *destin, int x, int y);
void drawSurface(SDL_Surface *src, SDL_Rect srcSize, SDL_Surface *destin, int x, int y);
void drawSurface(SDL_Surface *src, int srcX, int srcY, int srcW, int srcH, SDL_Surface *destin, int destX, int destY);

//draws a (part or whole) surface onto another
//Does not use per surface alpha. Uses per pixel alpha.
//uses sdl_gfx
void drawSurfacePPAlpha(SDL_Surface *src, SDL_Surface *destin, int x, int y);
void drawSurfacePPAlpha(SDL_Surface *src, SDL_Rect srcSize, SDL_Surface *destin, int x, int y);
void drawSurfacePPAlpha(SDL_Surface *src, int srcX, int srcY, int srcW, int srcH, SDL_Surface *destin, int destX, int destY);

//attempts to do alpha compositing blits the way intended
//void drawRBGAtoRGBA(SDL_Surface *src, SDL_Rect *srcSize, SDL_Surface *destin, int x, int y);

//copies pixels including alpha chanel
void copyPixels(SDL_Surface *src, SDL_Rect srcSize, SDL_Surface *destin, unsigned int x, unsigned int y);


//draws text onto another surface
void drawText(const std::string& text, TTF_Font *font, const SDL_Color& colour, SDL_Surface *dest, int x, int y);
void drawText(const std::string& text, TTF_Font *font, const SDL_Color& colour, short a, SDL_Surface *dest, int x, int y);
void drawText(const std::string& text, const std::string& fontFile, int size, const SDL_Color&, SDL_Surface *dest, int x, int y);
void drawText(const std::string& text, const std::string& fontFile, int size, const SDL_Color&, short a, SDL_Surface *dest, int x, int y);
void drawText(const std::string& text, const std::string& fontFile, int size, short r, short g, short b, short a, SDL_Surface *dest, int x, int y);
void drawText(const std::string& text, const std::string& fontFile, int size, short r, short g, short b, SDL_Surface *dest, int x, int y);



// returns an optimised version of a surface
SDL_Surface *optimizeSurface(SDL_Surface *surface, bool freeSurface);
SDL_Surface *optimizeSurface(SDL_Surface *surface, const SDL_Color& transKey, bool freeSurface);

//loads optimized image, returns a point to it
SDL_Surface *loadImage(const std::string& fileName);
SDL_Surface *loadImage(const std::string& fileName, const SDL_Color& transKey);
SDL_Surface *loadImage(SDL_RWops *src, int freesrc);
SDL_Surface *loadImage(SDL_RWops *src, int freesrc, const SDL_Color& transKey);



// returns psudo-random number between (including) lower and upper bound
int random(int lowerBound, int upperBound);



// returns the width, in pixels, of the specified string; using specified font.
unsigned int getStringWidth(TTF_Font *font, const std::string& text);

// returns the height, in pixels, of the specified string; using specified font.
unsigned int getStringHeight(TTF_Font *font, const std::string& text);



//  File io
//skips the given in-filestream until the delimiter is reched.
//delimter is not removed
void skipTo(std::ifstream &file, char delimiter);

//skips up to next non-white sapce character
void skipWhiteSpace(std::ifstream &file);

//reads from given in-filestream untill delimter is reached
//returns the read data as c++ string
//delimter not removed from stream
std::string readTo(std::ifstream &file, char delimtiter);

//reads from given in-filestream untill a newline is reached
//returns the read data as c++ string
//newline character is removed
std::string readToNewline(std::ifstream &file);




//  String processing
//change "\n"(2 characters) in a string loaded from a file to actual newline characters
std::string& correctNewLine(std::string& text);

//takes a string and checks if it is filled with white space
bool isSpace(const std::string& text);
//  Returns true if the input string contains any white spaces characters
bool containsSpace(const std::string& text);
//  returns true if all characters are alpha characters
bool isAlpha(const std::string& text);

//checks if 'input' string is a number
//returns 1 for true. 0 for false.
bool isNumber(const std::string& input);

//returns true if the string is entirely Alphabet characters or Numbers
bool isAlphNum( const std::string& text );


//changes 'input' string to upper case characters
std::string upperCase(std::string input);

//changes 'input' string to lower case characters
std::string lowerCase(std::string input);


//  Trims the whitespace off the back of a string
void trimBack(std::string& text);

//  Trims the whitespace off the front of a string
void trimFront(std::string& text);


//converts int to string (itoa)
//std::string itoa(int num);
//  (use string streams)
std::string itos(int num);

//converts string to int
int atoi(const std::string& text);

//  Converts string into a float
float atof(const std::string& text);


//  Centering elements
//  Returns the X-coordinate of A, neccesary to center A on B
int centerX(int widthA, int widthB);

//  Returns the Y-coordinate of A, neccesary to center A on B
int centerY(int heightA, int heightB);




//  File system io
//renames a file "oldFilename" to "newFilename"
int renameFile(std::string oldFilename, std::string newFilename);

//deletes file with filename "filename"
int removeFile(std::string filename);

//get size of file in bytes
//returns 0 on error
size_t getFileSize(std::string filename);
size_t getFileSize(std::ifstream& file);
size_t getFileSize(std::ofstream& file);  // hmmm... wtf?


/*
so i just found out that
"Contrary to popular belief, the Windows system API accepts forward slash"
(wikipedia, path(computer))

what a waste of time.

these functions will remain here, however, for ease of porting to OSes which
use neither / or \
*/
//  Changes the '/' or '\' to correct character corresponding to the curent OS
std::string correctFilepath(std::string filepath);

//changes all the '/' in a filepath to '\'
void makeWindowsFilePath(std::string& filepath);
std::string makeWindowsFilePath(const std::string& filepath);

//changes all the '\' in a filepath to '/'
void makeUnixFilePath(std::string& filepath);
std::string makeUnixFilePath(const std::string& filepath);




//  List modes for listFiles()
enum DirListMode{
    FILES_LIST_ALL,
    FILES_LIST_DIR,
    FILES_LIST_NO_DIR
};

//  Returns true if the path "filepath" is a directory
bool isDir(const std::string& filepath);

//  Lists all files in the directory pointed to by "filepath"
//  Results will be stored in "fileList"
//  "listmode" will affect what is stored.
//      FILES_LIST_ALL will list both files and directories
//      FILES_LIST_DIR will list only directories
//      FILES_LIST_NO_DIR will list only files, no directories
//  If "reletivePath" is true, the results will be stored as a reletive path
//      from "filepath"
bool listFiles( const std::string& filepath, std::vector<std::string>& fileList,
                enum DirListMode listmode,    bool reletivePath );

//  Same as the above, except it will recursively run through contained directories
bool listFilesRecusive( const std::string& filepath,
                        std::vector<std::string>& fileList, enum DirListMode listmode );

//  Makes path "path", reletive to "root"
void makeReletivePath(const std::string& root, std::string& path);

//  Same as the above, except it works on a list of paths
void makeReletivePaths(const std::string& root, std::vector<std::string>& fileList);


#endif
