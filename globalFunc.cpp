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


//  globalFunc.cpp

#include "globalFunc.hpp"

#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#include <dirent.h>
#include <unistd.h>

#include <SDL/SDL_image.h>
#include "SDL/SDL_gfxBlitFunc.h"


using namespace std;

bool validDirection(int dir){
    return (dir==UP) || (dir==RIGHT) || (dir==DOWN) || (dir==LEFT) ;
}

bool validBodyPart(int part){
    return  (part==HEAD) || (part==BODY) || (part==LEFT_HAND) || (part==RIGHT_HAND)
    || (part==LEFT_FOOT) || (part==RIGHT_FOOT) || (part==FACE) || (part==HAIR);
}

SDL_Rect makeRect(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h){
    SDL_Rect tmp;
    tmp.x = _x;
    tmp.y = _y;
    tmp.w = _w;
    tmp.h = _h;
    return tmp;
}


//recursivley draws a translucent over lay
void fadeTo(SDL_Surface *dest, SDL_Surface *targetImage, int timeMS){
    //alpha amount
    int a = 0;

    //current loop cycle
    int cycle = 0;

    //suface to be pasted on
    SDL_SetAlpha(targetImage, SDL_SRCALPHA, a);

    //fps capper variables
    long capStartTime = SDL_GetTicks();
    long capNowTime = SDL_GetTicks();

    //hold how much time left
    long timeLeft = timeMS;

    //time effect started.
    long effectStartTime = SDL_GetTicks();

    //will hold keystate data
    Uint8 *keys = SDL_GetKeyState(NULL);
    while( timeLeft > 0 ){
        //start FPS timer
        capStartTime = SDL_GetTicks();
        //update time left
        timeLeft = (timeMS - (SDL_GetTicks() - effectStartTime));

        //get keystate
        keys = SDL_GetKeyState(NULL);

        //event checking
        SDL_Event event;
        while ( SDL_PollEvent(&event) ){
            if ( event.type == SDL_QUIT ) { exit(EXIT_SUCCESS); }
            if ( event.type == SDL_KEYDOWN ){
                if ( event.key.keysym.sym == SDLK_ESCAPE ) { return; }
                if ( event.key.keysym.sym == SDLK_SPACE ) { return; }
            }
        }

        //fill screen with colour
        drawSurface(targetImage, dest, 0, 0);
        ++cycle;

        //show screen;
        SDL_Flip(dest);

        // cap FPS if needed
        capNowTime = SDL_GetTicks();
        if ( capNowTime-capStartTime < 1000/40 ){
            SDL_Delay( (1000/40) - (capNowTime-capStartTime) );
        };


        //if( timeLeft < (7*timeMS/8) ){ ++a; };
        //if( timeLeft < (3*timeMS/4) ){ ++a; };
        if( timeLeft < (timeMS/2) ){ ++a; };
        //if( timeLeft < (timeMS/3) ){ ++a; };
        if( timeLeft < (timeMS/4) ){ ++a; };
        if( timeLeft < (timeMS/8) ){ ++a; };
        SDL_SetAlpha(targetImage, SDL_SRCALPHA, ++a);
    }

    //in case of overtime, just fill it to that colour (solid)
    SDL_SetAlpha(targetImage, SDL_SRCALPHA, 255);
    drawSurface(targetImage, dest, 0, 0);
}

void fadeTo(SDL_Surface *screen,  const SDL_Color& targetColour, int timeMS){
    //size of screen
    SDL_Rect screenRect;
    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.w = screen->w;
    screenRect.h = screen->h;

    //alpha amount
    int a = 0;

    //current loop cycle
    int cycle = 0;

    //colour suface to be pasted on
    SDL_Surface *surface = duplicateSurface(screen);
    SDL_FillRect(surface, &screenRect, SDL_MapRGB(screen->format, targetColour.r, targetColour.g, targetColour.b));
    SDL_SetAlpha(surface, SDL_SRCALPHA, a);

    //fps capper variables
    long capStartTime = SDL_GetTicks();
    long capNowTime = SDL_GetTicks();

    //hold how much time left
    long timeLeft = timeMS;

    //time effect started.
    long effectStartTime = SDL_GetTicks();

    //will hold keystate data
    Uint8 *keys = SDL_GetKeyState(NULL);
    while( timeLeft > 0 ){
        //start FPS timer
        capStartTime = SDL_GetTicks();
        //update time left
        timeLeft = (timeMS - (SDL_GetTicks() - effectStartTime));

        //get keystate
        keys = SDL_GetKeyState(NULL);

        //event checking
        SDL_Event event;
        while ( SDL_PollEvent(&event) ){
            if ( event.type == SDL_QUIT ) { exit(EXIT_SUCCESS); };
            if ( event.type == SDL_KEYDOWN ){
                if ( event.key.keysym.sym == SDLK_ESCAPE ) { return; }
                if ( event.key.keysym.sym == SDLK_SPACE ) { return; }
            }
        }

        //fill screen with colour
        drawSurface(surface, screen, 0, 0);
        drawSurface(surface, screen, 0, 0);
        ++cycle;

        //show screen;
        SDL_Flip(screen);

        // cap FPS if needed
        capNowTime = SDL_GetTicks();
        if ( capNowTime-capStartTime < 1000/40 ){
            SDL_Delay( (1000/40) - (capNowTime-capStartTime) );
        };


        //if( timeLeft < (7*timeMS/8) ){ ++a; };
        //if( timeLeft < (3*timeMS/4) ){ ++a; };
        if( timeLeft < (timeMS/2) ){ ++a; };
        //if( timeLeft < (timeMS/3) ){ ++a; };
        if( timeLeft < (timeMS/4) ){ ++a; };
        if( timeLeft < (timeMS/8) ){ ++a; };
        SDL_SetAlpha(surface, SDL_SRCALPHA, ++a);
    }

    //in case of overtime, just fill it to that colour (solid)
    SDL_FillRect(screen, &screenRect, SDL_MapRGB(screen->format, targetColour.r, targetColour.g, targetColour.b));
    SDL_FreeSurface(surface);
}


void rgbFadeTo(SDL_Surface *screen, SDL_Surface *targetImage, int timeMS){
    //hold rgb
    Uint8 r,g,b,r2,g2,b2;

    //max height/width to fade
    int maxWidth,maxHeight;
    if(targetImage->w >= screen->w){
        maxWidth = screen->w;
    } else {
        maxWidth = targetImage->w;
    }
    if(targetImage->h >= screen->h){
        maxHeight = screen->h;
    } else {
        maxHeight = targetImage->h;
    }


    //hold how much time left
    long timeLeft = timeMS;

    LOCK(screen);
    LOCK(targetImage);

    //time effect started.
    long effectStartTime = SDL_GetTicks();

    //will hold keystate data
    Uint8 *keys = SDL_GetKeyState(NULL);
    while( timeLeft > 0 ){
        //update time left
        timeLeft = (timeMS - (SDL_GetTicks() - effectStartTime));

        //get keystate
        keys = SDL_GetKeyState(NULL);

        //event checking
        SDL_Event event;
        while ( SDL_PollEvent(&event) ){
            if ( event.type == SDL_QUIT ) { exit(EXIT_SUCCESS); };
            if ( event.type == SDL_KEYDOWN ){
                if ( event.key.keysym.sym == SDLK_ESCAPE ) { return; }
                if ( event.key.keysym.sym == SDLK_SPACE ) { return; }
            }
        }

        //fill screen with colour
        for( int x = 0; x < maxWidth; ++x){
            for( int y = 0; y < maxHeight; ++y){
                SDL_GetRGB(getpixel(screen, x, y), screen->format, &r, &g, &b);
                SDL_GetRGB(getpixel(targetImage, x, y), targetImage->format, &r2, &g2, &b2);

                //      x = time left
                //      y = colour value
                //      G = goal colour
                //      F = Total time given
                //      I = initial colour
                //      y = ((Ix-Gx)/F) + G

                #define _R_ ((r*timeLeft-r2*timeLeft)/timeMS)+r2
                #define _G_ ((g*timeLeft-g2*timeLeft)/timeMS)+g2
                #define _B_ ((b*timeLeft-b2*timeLeft)/timeMS)+b2

                putpixel(screen, x, y, SDL_MapRGB(screen->format, _R_, _G_, _B_));

                #undef _R_
                #undef _G_
                #undef _B_
            }
        }

        //show screen;
        SDL_Flip(screen);
    }

    UNLOCK(screen);
    UNLOCK(targetImage);

    //in case of overtime, just fill it to that colour (solid)
    //holds size of screen
    drawSurface(targetImage, screen, 0, 0);
}




SDL_Surface *duplicateSurface(SDL_Surface *source, Uint32 flags){
    if( source == NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "shadowSurface(): source is NULL";
        return source;
    }
    /*return createSurface( source->w,
                          source->h,
                          source->format->BitsPerPixel,
                          source->flags );*/
    return SDL_ConvertSurface(source, source->format, source->flags|flags);
    //return createSurface(source->w, source->h, source->format->BitsPerPixel, source->flags|flags);
}


SDL_Surface *createSurface(int width, int height, int bpp, Uint32 flags){
    flags |= SDL_RLEACCEL;
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface *temp1 = SDL_CreateRGBSurface(  flags, width, height, bpp,
                                                rmask, gmask, bmask,  amask   );
    SDL_Surface *temp2;
    if(temp1 != NULL){
        if(temp1->flags & SDL_SRCALPHA){
            temp2 = SDL_DisplayFormatAlpha(temp1);
        } else {
            temp2 = SDL_DisplayFormat(temp1);
        }
        if( temp2 == NULL ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to create surface: " << SDL_GetError() << endl;
            return temp1;
        } else {
            SDL_FreeSurface(temp1);
            return temp2;
        }
        return temp2;
    } else{
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to create surface" << ": " << SDL_GetError() << endl;
        return NULL;
    }
}


//taken from SDL mailing list
Uint32 getpixel(SDL_Surface *surface, int x, int y){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

//taken from SDL mailing list
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}



void LOCK(SDL_Surface *dest){
    if( SDL_MUSTLOCK(dest) ){
        if( SDL_LockSurface(dest) == -1 ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to lock surface: " << dest << endl;
            return;
        }
    }
}

void UNLOCK(SDL_Surface *dest){
    if( SDL_MUSTLOCK(dest) ){
        SDL_UnlockSurface(dest);
    }
}


bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2){
    if(
        ((rect1.x > rect2.x) && (rect1.x < rect2.w))    //left
                             ||
        ((rect2.x > rect1.x) && (rect2.w < rect1.x))    //right
                             ||
        ((rect1.y > rect2.y) && (rect1.y < rect2.h))    //top
                             ||
        ((rect2.y > rect1.y) && (rect2.h < rect1.y))    //bottom
    ){
        return 1;
    } else {
        return 0;
    }
}



SDL_Color rgb(Uint8 r, Uint8 g, Uint8 b){
    SDL_Color temp = {r, g, b, 0};
    return temp;
}


void drawSurface(SDL_Surface *src, SDL_Surface *destin, int x, int y){
    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if(SDL_BlitSurface(src, NULL, destin, &dest) != 0){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x << ", " << y << ") failed: " << SDL_GetError() << endl;
    }
}

void drawSurface(SDL_Surface *src, int x, int y, int w, int h, SDL_Surface *destin, int x2, int y2){
    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Rect from;
    from.x = x;
    from.y = y;
    from.w = w;
    from.h = h;
    SDL_Rect dest;
    dest.x = x2;
    dest.y = y2;
    if(SDL_BlitSurface(src,&from,destin,&dest) != 0){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x2 << ", " << y2 << ") failed: " << SDL_GetError() << endl;
    }
}

void drawSurface(SDL_Surface *src, SDL_Rect srcSize, SDL_Surface *destin, int x, int y){
    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if(SDL_BlitSurface(src,&srcSize,destin,&dest) != 0){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x << ", " << y << ") failed: " << SDL_GetError() << endl;
    }
}


void drawSurfacePPAlpha(SDL_Surface *src, SDL_Surface *destin, int x, int y){
    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    //use alpha blit if needed
    //(src->flags&SDL_SRCALPHA) ||
    if( ((destin->flags&SDL_SRCALPHA) && (src->format->BitsPerPixel==32))
        && (destin != SDL_GetVideoSurface()) ){
        SDL_gfxBlitRGBA(src, NULL, destin, &dest);
    } else {
        if(SDL_BlitSurface(src, NULL, destin, &dest) != 0){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x << ", " << y << ") failed: " << SDL_GetError() << endl;
        }
    }
}

void drawSurfacePPAlpha(SDL_Surface *src, int x, int y, int w, int h, SDL_Surface *destin, int x2, int y2){
    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Rect from;
    from.x = x;
    from.y = y;
    from.w = w;
    from.h = h;
    SDL_Rect dest;
    dest.x = x2;
    dest.y = y2;
    //use alpha blit if needed
    //(src->flags&SDL_SRCALPHA) ||
    if( ((destin->flags&SDL_SRCALPHA) && (src->format->BitsPerPixel==32))
        && (destin != SDL_GetVideoSurface()) ){
        SDL_gfxBlitRGBA(src, &from, destin, &dest);
    } else {
        if(SDL_BlitSurface(src,&from,destin,&dest) != 0){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x2 << ", " << y2 << ") failed: " << SDL_GetError() << endl;
        }
    }
}

void drawSurfacePPAlpha(SDL_Surface *src, SDL_Rect srcSize, SDL_Surface *destin, int x, int y){
    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    //use alpha blit if needed
    //(src->flags&SDL_SRCALPHA) ||
    if( ((destin->flags&SDL_SRCALPHA) && (src->format->BitsPerPixel==32))
        && (destin != SDL_GetVideoSurface()) ){
        SDL_gfxBlitRGBA(src, &srcSize, destin, &dest);
        return;
    } else {
        if(SDL_BlitSurface(src,&srcSize,destin,&dest) != 0){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x << ", " << y << ") failed: " << SDL_GetError() << endl;
        }
    }
}


void drawRBGAtoRGBA(SDL_Surface *src, SDL_Rect *srcSize, SDL_Surface *destin, int x, int y){
    cout << "drawRGBAtoRGBA is deprecated" << endl;
/*    if( (src == NULL) || (destin == NULL) ){ return; }
    SDL_Surface *intermediate;
    if( srcSize != NULL ){
        //  The destination surface's bpp, since thats the maximal detail that will be kept
        intermediate = createSurface(srcSize->w, srcSize->h, 32, destin->flags|SDL_SWSURFACE|SDL_SRCALPHA);
    } else {
        intermediate = createSurface(destin->w, destin->h, 32, destin->flags|SDL_SWSURFACE|SDL_SRCALPHA);
    }
    SDL_SetAlpha(intermediate, 0, SDL_ALPHA_OPAQUE);
    //SDL_FillRect(intermediate, NULL, SDL_MapRGBA(intermediate->format,255,0,255,SDL_ALPHA_OPAQUE));
    //SDL_SetColorKey(intermediate, SDL_SRCCOLORKEY, SDL_MapRGBA(intermediate->format,255,0,255,SDL_ALPHA_OPAQUE));

    //store settings
    Uint32 oldSrcFlags = src->flags;
    Uint32 oldDestFlags = destin->flags;
    //turn off per surface alpha bliting
    SDL_SetAlpha(src, 0, SDL_ALPHA_OPAQUE);
    SDL_SetAlpha(destin, 0, SDL_ALPHA_OPAQUE);

    //blit image to intemediate surface
    if(SDL_gfxBlitRGBA(src, srcSize, intermediate, NULL) != 0){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x << ", " << y << ") failed: " << SDL_GetError() << endl;
    }

    //blit image to actual end surface
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if(SDL_gfxBlitRGBA(intermediate, NULL, destin, &dest) != 0){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Blitting " << src << " to " << destin << " at (" << x << ", " << y << ") failed: " << SDL_GetError() << endl;
    }

    //restore settings
    SDL_SetAlpha(src, oldSrcFlags, SDL_ALPHA_OPAQUE);
    SDL_SetAlpha(destin, oldDestFlags, SDL_ALPHA_OPAQUE);
    //free temp suface
    SDL_FreeSurface(intermediate);
    intermediate = NULL;*/
}



void copyPixels(SDL_Surface *src, SDL_Rect srcSize, SDL_Surface *destin, int x, int y){
    if( src == NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "copyPixels: given null source" << endl;
        return;
    }
    if( destin == NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "copyPixels: given null destin" << endl;
        return;
    }

    //  if you cant see the final blited image, doon wooryy!
    //if( x < 0 ){ return; }
    if( x >= destin->w ){ return; }
    //if( y < 0 ){ return; }
    if( y >= destin->h ){ return; }

   // if( srcSize.w < 0 ){ return; }
   // if( srcSize.h < 0 ){ return; }

    if( srcSize.x > src->w ){ return; }
    if( srcSize.y > src->w ){ return; }

    //re clip rect
   // if( srcSize.x < 0 ){ srcSize.x = 0; }
   // if( srcSize.y < 0 ){ srcSize.y = 0; }

    if( srcSize.w+srcSize.x > src->w ){
        srcSize.w = ((srcSize.w+srcSize.x) - src->w);
    }
    if( srcSize.h+srcSize.y > src->h ){
        srcSize.h = ((srcSize.h+srcSize.y) - src->h);
    }

    //lock the surface
    LOCK(src);
    LOCK(destin);

    int destX = 0;
    int destY = 0;
    Uint8 r,b,g,a;
    //put pixels, running down
    for( int srcY = srcSize.y; srcY < srcSize.h; ++srcY ){
        //put pixels, running across
        for( int srcX = srcSize.x; srcX < srcSize.w; ++srcX ){
            //get the rbg values of the pixel in the source
            SDL_GetRGBA( getpixel(src, srcX, srcY), src->format, &r, &g, &b, &a );
            //map the rbg vales from source's format to the dest's format
            putpixel( destin, destX+x, destY+y, SDL_MapRGBA(destin->format,r,g,b,a) );
            ++destX;
        }
        ++destY;
        destX = 0;
    }

    //unlock the surface
    UNLOCK(src);
    UNLOCK(destin);

    return;
}



void drawText(const std::string& text, const std::string& fontFile, int size, short r, short g, short b, SDL_Surface *dest, int x, int y){
    TTF_Font *tempTTF = TTF_OpenFont(fontFile.c_str(), size);
    SDL_Surface *textSurface = TTF_RenderText_Blended(tempTTF, text.c_str(), rgb(r,g,b));
    TTF_CloseFont(tempTTF);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, dest, &destRect);

    SDL_FreeSurface(textSurface);
}
void drawText(const std::string& text, const std::string& fontFile, int size, short r, short g, short b, short a, SDL_Surface *dest, int x, int y){
    TTF_Font *tempTTF = TTF_OpenFont(fontFile.c_str(), size);
    SDL_Surface *textSurface = TTF_RenderText_Blended(tempTTF, text.c_str(), rgb(r,g,b));
    SDL_SetAlpha(textSurface, SDL_SRCALPHA, a);
    TTF_CloseFont(tempTTF);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, dest, &destRect);

    SDL_FreeSurface(textSurface);
}

void drawText(const std::string& text, const std::string& fontFile, int size, const SDL_Color& colour, SDL_Surface *dest, int x, int y){
    TTF_Font *tempTTF = TTF_OpenFont(fontFile.c_str(), size);
    SDL_Surface *textSurface = TTF_RenderText_Blended(tempTTF, text.c_str(), colour);
    TTF_CloseFont(tempTTF);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, dest, &destRect);

    SDL_FreeSurface(textSurface);
}
void drawText(const std::string& text, const std::string& fontFile, int size, const SDL_Color& colour, short a, SDL_Surface *dest, int x, int y){
    TTF_Font *tempTTF = TTF_OpenFont(fontFile.c_str(), size);
    SDL_Surface *textSurface = TTF_RenderText_Blended(tempTTF, text.c_str(), colour);
    SDL_SetAlpha(textSurface, SDL_SRCALPHA, a);
    TTF_CloseFont(tempTTF);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, dest, &destRect);

    SDL_FreeSurface(textSurface);
}

void drawText(const std::string& text, TTF_Font *font, const SDL_Color& colour, SDL_Surface *dest, int x, int y){
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), colour);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, dest, &destRect);

    SDL_FreeSurface(textSurface);
}
void drawText(const std::string& text, TTF_Font *font, const SDL_Color& colour, short a, SDL_Surface *dest, int x, int y){
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), colour);
    SDL_SetAlpha(textSurface, SDL_SRCALPHA, a);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, dest, &destRect);

    SDL_FreeSurface(textSurface);
}




SDL_Surface *optimizeSurface(SDL_Surface *surface, bool freeSurface){
    SDL_Surface *temp = NULL;
    if(surface != NULL){
        if(surface->flags & SDL_SRCALPHA){
            temp = SDL_DisplayFormatAlpha(surface);
        } else {
           temp = SDL_DisplayFormat(surface);
        }
        if( temp == NULL ){
            if( freeSurface ){ SDL_FreeSurface(surface); }
            return surface;
        } else {
            if( freeSurface ){ SDL_FreeSurface(surface); }
            return temp;
        }
    }

    return NULL;
}

SDL_Surface *optimizeSurface(SDL_Surface *surface, const SDL_Color& transKey, bool freeSurface){
    SDL_Surface *temp = NULL;
    if(surface != NULL){
        if(surface->flags & SDL_SRCALPHA){
            temp = SDL_DisplayFormatAlpha(surface);
        } else {
           temp = SDL_DisplayFormat(surface);
        }
        if( temp == NULL ){
            if( freeSurface ){ SDL_FreeSurface(surface); }
            return surface;
        } else {
            SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(temp->format, transKey.r, transKey.g, transKey.b));
            if( freeSurface ){ SDL_FreeSurface(surface); }
            return temp;
        }
    }

    return NULL;
}

SDL_Surface *loadImage(const std::string& fileName){
    return optimizeSurface(IMG_Load(fileName.c_str()), true);
}

SDL_Surface *loadImage(SDL_RWops *src, int freesrc){
    return optimizeSurface(IMG_Load_RW(src, freesrc), true);
}

SDL_Surface *loadImage(const std::string& fileName, const SDL_Color& transKey){
    return optimizeSurface(IMG_Load(fileName.c_str()), transKey, true);
}

SDL_Surface *loadImage(SDL_RWops *src, int freesrc, const SDL_Color& transKey){
    return optimizeSurface(IMG_Load_RW(src, freesrc), transKey, true);
}



unsigned int getStringWidth(TTF_Font *font, const std::string& text){
    Uint16 ch;
    int maxx = 0;
    int totalWidth = 0;

    if ( !text.empty() ){
		for( string::size_type i = 0; i < text.length(); ++i ){
            maxx = 0;
			ch = text.at(i);
			if(TTF_GlyphMetrics(font, ch, NULL, &maxx, NULL, NULL, NULL) == -1){
                cerr << __FILE__ << " " << __LINE__ << ": " << TTF_GetError() << endl;
                return 0;
            } else {
                totalWidth += maxx;
            }
		}
    }

	return totalWidth + text.length() - 1;
}

unsigned int getStringHeight(TTF_Font *font, const std::string& text){
    Uint16 ch;
    int maxy = 0;
    int curCharHeight = 0;

    if ( !text.empty() ){
		for (unsigned int i = 0; i < text.length(); ++i ){
            curCharHeight = 0;
			ch = text.at(i);
			if(TTF_GlyphMetrics(font, ch, NULL, NULL, NULL, &curCharHeight, NULL) == -1){
                cerr << __FILE__ << " " << __LINE__ << ": " << TTF_GetError() << endl;
                return 0;
            } else {
                if (curCharHeight > maxy){
                    maxy = curCharHeight;
                }
            }
		}
    }

	return maxy;
}

void getSDLDetails(){
    const SDL_VideoInfo *info = SDL_GetVideoInfo();

    char blah[100];
    cerr << __FILE__ << " " << __LINE__ << ": " << "driver: " << SDL_VideoDriverName(blah, 100) << endl;

    cerr << __FILE__ << " " << __LINE__ << ": " << "hw_available: " << info->hw_available << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "wm_available: " << info->wm_available << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_hw: " << info->blit_hw << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_hw_CC: " << info->blit_hw_CC << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_hw_A: " << info->blit_hw_A << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_sw: " << info->blit_sw << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_sw_CC: " << info->blit_sw_CC << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_sw_A: " << info->blit_sw_A << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "blit_fill: " << info->blit_fill << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "video_mem: " << info->video_mem << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "bpp: " << int(info->vfmt->BitsPerPixel) << endl;
}



int random(int lowerBound, int upperBound){
    if(lowerBound == upperBound){ return lowerBound; }
    //switch values if needed
    if( upperBound < lowerBound ){
        int tmp = upperBound;
        upperBound = lowerBound;
        lowerBound = tmp;
    }

    //increase upper bound to make it posible to get upperbound itselfs
    ++upperBound;

    //get the total negative offset (wrt 0)
    int deltaZ = 0;
    if( lowerBound < 0 ) {
        deltaZ += lowerBound;
        lowerBound = 0;
    }
    if( upperBound < 0 ){
        deltaZ += upperBound;
        upperBound = 0;
    }

    if( deltaZ < 0 ){ deltaZ *= -1; };

    //generate a new number (positive)
    int num = rand() % (upperBound+deltaZ);

    // rand() makes numbers btween 0-RAND_MAX
    //  we cant use zeros... so we will roll again
    while( num == 0 || num < lowerBound ){
        num = rand() % (upperBound+deltaZ);
    }

    //make number negative if nesecary
    if( deltaZ > 0 ){
        num -= deltaZ;
    }

    return num;
}


bool isSpace(const std::string& text){
    for ( string::size_type i = 0; i < text.length(); ++i ){
        if( !isspace( text.at(i) ) ){ return 0; };
    }
    return 1;
}

void skipTo(ifstream &file, char delimiter){
    while( !file.bad() && !file.eof() ){
        if( file.peek() != delimiter ){
            file.get();
        } else {
            return;
        }
    }
}

void skipWhiteSpace(ifstream &file){
    while( !file.bad() && !file.eof() ){
        //if next char is whitespace
        if (  isspace( file.peek() ) || (file.peek() == '\n')  ){
            //swallow next chartater
            file.get();
        } else {
            //leave
            return;
        }
    };
}

std::string readTo(ifstream &file, char delimtiter){
    string data;
    while( !file.bad() && !file.eof() ){
        if( file.peek() != delimtiter ){
            data += file.get();
        } else {
            return data;
        }
    }
    return "";
}

std::string readToNewline(ifstream &file){
    char ch;
    string data = "";
    while( !file.bad() && !file.eof() ){
        file.get(ch);
        if( ch != '\n'){
            data += ch;
        } else {
            return data;
        }
    }
    return data;
}

std::string& correctNewLine(std::string& text){
    if( text.empty() ){ return text; }
    for( std::string::size_type i = 0; i < text.length(); ++i ){
        std::string::size_type pos = text.find("\\n", i);
        if( pos != std::string::npos ){
            text.replace( pos, 1, "\n" );
            text.erase(pos+1, 1);
            i = pos;
        } else {
            return text;
        }
    }
    return text;
}


bool containsSpace(const std::string& text){
    for( string::size_type i = 0; i < text.length(); ++i ){
        if( isspace( text.at(i) ) ){ return 1; };
    }
    return 0;
}

bool isAlpha(const std::string& text){
    for( string::size_type i = 0; i < text.length(); ++i ){
        if( !isalpha( text.at(i) ) ){ return 0; };
    }
    return 1;
}

bool isNumber(const std::string& input){
    if ( !input.empty() ){
		for( string::size_type i = 0; i < input.length(); ++i ){
			if( !isdigit(  input.at(i) ) ){
                return 0;
            }
		}
		return 1;
	}
	return 0;
}

bool isAlphNum( const std::string& text ){
    if ( !text.empty() ){
		for( string::size_type i = 0; i < text.length(); ++i ){
			if( !isalnum(text.at(i)) ){
                return 0;
            }
		}
		return 1;
	}
	return 0;
}


string upperCase(std::string input){
	if (!input.empty() ){
		for( string::size_type i = 0; i < input.length(); ++i ){
			input.at(i) = toupper(input.at(i));
		}
	}
	return input;
}

string lowerCase(std::string input){
	if (!input.empty() ){
		for( string::size_type i = 0; i < input.length(); ++i ){
			input.at(i) = tolower(input.at(i));
		}
	}
	return input;
}


void trimBack(std::string& text){
    if( text.empty() || (text == "") ){ return; }
    for(std::string::size_type i = text.length()-1; i != 0; --i){
        if( !isspace(text.at(i)) ){
            text.resize(++i);
            return;
        }
    }
}

void trimFront(std::string& text){
    if( text.empty() || (text == "") ){ return; }
    for(std::string::size_type i = 0; i < text.length(); ++i){
        if( !isspace(text.at(i)) ){
            std::string::iterator startItr = text.begin();
            std::string::iterator endItr = text.begin();
            advance(endItr, i);
            text.erase(startItr, endItr);
            return;
        }
    }
}


std::string itos(int num){
    std::stringstream numText;
    numText << num;
    return numText.str();
}


//converts string to int
int atoi(const std::string& text){
    return atoi(text.c_str());
}

//converts string to int
float atof(const std::string& text){
    return atof(text.c_str());
}


//  Returns the X-coordinate of A, neccesary to center A on B
int centerX(int widthA, int widthB){
    if(widthA > widthB){
        return ( -((widthA - widthB)/2) );
    } else {
        return ( (widthB - widthA)/2 );
    }
}

//  Returns the Y-coordinate of A, neccesary to center A on B
int centerY(int heightA, int heightB){
    if(heightA > heightB){
        return ( -((heightA - heightB)/2) );
    } else {
        return ( (heightB - heightA)/2 );
    }
}







int renameFile( std::string oldFilename, std::string newFilename){
    oldFilename = correctFilepath(oldFilename);
    newFilename = correctFilepath(newFilename);
    int rValue = rename( oldFilename.c_str(), newFilename.c_str() );
    if(  rValue != 0 ){ perror( oldFilename.c_str() ); }
    return rValue;
}


int removeFile(std::string filename){
    filename = correctFilepath(filename);
    int rValue = remove( filename.c_str() );
    if(  rValue != 0 ){ perror( filename.c_str() ); }
    return rValue;
}

size_t getFileSize(std::string filename){
    //open file
    filename = correctFilepath(filename);
    std::ifstream file( filename.c_str() );
    if( (!file.good()) || (!file.is_open()) ){
        file.close();
        std::cout << "Unable to open file \"" << filename << "\"" << std::endl;
        return 0;
    }
    ifstream::pos_type size_of_file;
    file.seekg( 0L, std::ios_base::end );       //goto end of file
    size_of_file = file.tellg();                //position
    file.close();                               //close file
    return( size_of_file );
}
size_t getFileSize(ifstream& file){
    ifstream::pos_type cur_pos,size_of_file;
    cur_pos = file.tellg();                     //store position
    file.seekg( 0L, std::ios_base::end );       //goto end of file
    size_of_file = file.tellg();                //position
    file.seekg( cur_pos, std::ios_base::beg );  //restore pos
    return( size_of_file );
}
size_t getFileSize(ofstream& file){
    ifstream::pos_type cur_pos,size_of_file;
    cur_pos = file.tellp();                     //store position
    file.seekp( 0L, std::ios_base::end );       //goto end of file
    size_of_file = file.tellp();                //position
    file.seekp( cur_pos, std::ios_base::beg );  //restore pos
    return( size_of_file );
}


//  Changes the '/' or '\' to correct character corresponding to the curent OS
std::string correctFilepath(std::string filepath){
 	#ifdef _WIN32
        makeWindowsFilePath(filepath);
 	#else
        makeUnixFilePath(filepath);
 	#endif
 	return filepath;
}


//changes all the '/' in a filepath to '\'
//removes double slashes
void makeWindowsFilePath(std::string& filepath){
    makeUnixFilePath(filepath);
/*
    while( filepath.find_first_of('/') != string::npos ){
        filepath.at( filepath.find_first_of('/') ) = '\\';
    }
    while( filepath.find("\\\\") != string::npos ){
        filepath.at( filepath.find("\\\\") ) = '\\';
    }*/
}

std::string makeWindowsFilePath(const std::string& filepath){
    return makeUnixFilePath(filepath);
    /*
    std::string tmp = filepath;
    while( tmp.find_first_of('/') != string::npos ){
        tmp.at( tmp.find_first_of('/') ) = '\\';
    }
    while( tmp.find("\\\\") != string::npos ){
        tmp.at( tmp.find("\\\\") ) = '\\';
    }
    return tmp;*/
}

//changes all the '\' in a filepath to '/'
//removes double slashes
void makeUnixFilePath(std::string& filepath){
    while( filepath.find_first_of('\\') != string::npos ){
        filepath.at( filepath.find_first_of('\\') ) = '/';
    }
    while( filepath.find("//") != string::npos ){
        filepath.erase( filepath.find("//"), 1 );
    }
}

std::string makeUnixFilePath(const std::string& filepath){
    std::string tmp = filepath;
    while( tmp.find_first_of('\\') != string::npos ){
        tmp.at( tmp.find_first_of('\\') ) = '/';
    }
    while( tmp.find("//") != string::npos ){
        tmp.erase( tmp.find("//"), 1 );
    }
    return tmp;
}



bool isDir(const std::string& filepath){
    DIR* dir = opendir( correctFilepath(filepath).c_str() );
    if( dir != NULL ){
        closedir( dir );
        return true;
    }

    return false;
}


bool listFiles( const std::string& filepath,
                std::vector<std::string>& fileList,
                enum DirListMode listmode,
                bool reletivePath )
{
    fileList.clear();
    if( isDir(filepath) ){
        std::string filename = "";
        DIR* dir = opendir( correctFilepath(filepath).c_str() );
        dirent *tmpEnt = readdir( dir );
        while( tmpEnt != NULL ){
            filename = tmpEnt->d_name;
            if( (filename != ".") && (filename != "..")  ){
                if( !reletivePath ){ filename = correctFilepath(filepath+"\\"+filename); }
                switch( listmode ){
                    case FILES_LIST_NO_DIR:
                        if( isDir(filename) ){ break; }
                        fileList.push_back( filename );
                        break;

                    case FILES_LIST_DIR:
                        if( !isDir(filename) ){ break; }
                        fileList.push_back( filename );
                        break;

                    default:
                    case FILES_LIST_ALL:
                        fileList.push_back( filename );
                        break;
                }
            }
            tmpEnt = readdir( dir );
        }
        closedir(dir);
        return true;
    }
    return false;
}

bool listFilesRecusive( const std::string& filepath,
                        std::vector<std::string>& fileList,
                        enum DirListMode listmode ){
    std::vector<std::string> dirs;
    std::vector<std::string> files;
    if( !listFiles(filepath, dirs, FILES_LIST_DIR, false) ){ return false; }
    if( !listFiles(filepath, files, FILES_LIST_NO_DIR, false) ){ return false; }

    std::vector<std::string> dirsTmp;
    std::vector<std::string> filesTmp;
    for( size_t i = 0; i < dirs.size(); ++i ){
        listFilesRecusive( dirs[i], dirsTmp, FILES_LIST_DIR );
        listFilesRecusive( dirs[i], filesTmp, FILES_LIST_NO_DIR );

        dirs.insert(dirs.end(), dirsTmp.begin(), dirsTmp.end());
        files.insert(files.end(), filesTmp.begin(), filesTmp.end());
    }

    fileList.clear();
    switch( listmode ){
        case FILES_LIST_NO_DIR:
            fileList = files;
            break;

        case FILES_LIST_DIR:
            fileList = dirs;
            break;

        default:
        case FILES_LIST_ALL:
            fileList.reserve(dirs.size() + files.size());
            fileList.insert(fileList.end(), dirs.begin(), dirs.end());
            fileList.insert(fileList.end(), files.begin(), files.end());
            break;
    }

    return true;
}

void makeReletivePath(const std::string& root, std::string& path){
    // strip file path
    std::string::size_type loc = path.find( root, 0 );
    if( loc != string::npos ) {
        path.replace( loc, root.length(), "");
    }
}

void makeReletivePaths(const std::string& root, std::vector<std::string>& fileList){
    for( size_t i = 0; i < fileList.size(); ++i ){
        makeReletivePath( root, fileList[i] );
    }
}

