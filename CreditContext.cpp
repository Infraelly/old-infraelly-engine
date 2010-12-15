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


#include "globalFunc.hpp"
#include "colours.hpp"
#include "fonts.hpp"

#include "CreditContext.hpp"
#include "InfraellyMixer.hpp"

CreditContext::CreditContext() :
    previousScreen(duplicateSurface(screen)),
    fontSize(24),
    y( screen->h )
{
    // Set window caption
    SDL_WM_SetCaption("Infraelly Credits", NULL);

    //set fps
    fpsManager.setMaxFps(30);

    // player music
    InfraellyMixer::stopMusic();
    InfraellyMixer::playMusic("audio/always_alone.mp3",-1, 1000);


    /* credit listing. Vector is used opposed to array to avoid having
        to re-number all elements, after each new insertion*/
    creditListing.push_back("-._.-~~-._.-~~-._.-~~-.");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Thank you to the authors of the following libraries, used in Infraelly");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("SDL - Simple Directmedia Layer");
    creditListing.push_back("SDL_mixer");
    creditListing.push_back("SDL_TTF");
    creditListing.push_back("SDL_image");
    creditListing.push_back("SDL_net");
    creditListing.push_back("SDL_gfx");
    creditListing.push_back("Net2");
    creditListing.push_back("Libpng (used by SDL_image)");
    creditListing.push_back("Zlib (used by Libpng)");
    creditListing.push_back("FreeType Project (used by SDL_ttf)");
    creditListing.push_back("guichan");
    creditListing.push_back("TinyXml");
    creditListing.push_back("Crypto++");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Default Fonts");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Thank you to Larabie Fonts to for their excellent free fonts");
    creditListing.push_back("The following fonts were used:");
    creditListing.push_back("Forgotten Futurist (TrueType)");
    creditListing.push_back("Effloresce (TrueType)");
    creditListing.push_back("Credit Valley (TrueType)");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Infraelly Music and Sound effects");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("insanepotato");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Infraelly Graphics and Design");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("mah SiSteR");
    creditListing.push_back("insanepotato");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Infraelly Website");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("xtiaDesign");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Infraelly Programing");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("insanepotato");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("Contacting me");
    creditListing.push_back("~-------------------------~");
    creditListing.push_back("If there are any problems or bugs you would like to discuss with me;");
    creditListing.push_back("or even if you just want to chat, you can contact me via");
    creditListing.push_back("");
    creditListing.push_back("E-mail: insanepotato@hotmail.com");
    creditListing.push_back("MSN-IM: insanepotato@hotmail.com");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("~-----------------------~");
    creditListing.push_back("Thank you");
    creditListing.push_back("~-----------------------~");
    creditListing.push_back("Thank YOU for choosing Infraelly =)");
    creditListing.push_back("");
    creditListing.push_back("A final thanks to the authors and contributors of the afore mentioned libraries");
    creditListing.push_back("I love you all long time <3");
    creditListing.push_back("");
    creditListing.push_back("BloodShed Software for making an awesome IDE");
    creditListing.push_back("");
    creditListing.push_back("The Code::Blocks team for making an even better IDE!");
    creditListing.push_back("");
    creditListing.push_back("CrossFire MMORPG... i had a peek at your server's source code ;D");
    creditListing.push_back("");
    creditListing.push_back("Mr. Jacobs");
    creditListing.push_back("");
    creditListing.push_back("Aloo");
    creditListing.push_back("");
    creditListing.push_back("AsianFace, you showed me hell <3");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("");
    creditListing.push_back("Have a nice day =)");

    //used to draw text down the screen in 'lines'
    line = getStringHeight(font::creditFont.at(fontSize), "IPYHGgyipqQeoSa 0");
    line += 15;

    /*total height of the credit listing. 50 is added to the end,
    after its finished scrolling, there is a delay before it reappears at the bottom*/
    totalHeight = -line*creditListing.size()-3*line;

    //black box so "Press ESC to return" will appears on top of listing
    SDL_Rect escBox;
    escBox.x = 0;
    escBox.y = 0;
    escBox.w = screen->w;
    escBox.h = 15;

    //fade to black (fade into credits)
    fadeTo(screen, colour::black, 1000);

    run();
}

CreditContext::~CreditContext(){
    if( previousScreen != NULL ){
        fadeTo(screen, previousScreen, 1000);
        SDL_FreeSurface(previousScreen);
    }
    if( !oldCap.empty() ){
        SDL_WM_SetCaption(oldCap.c_str(), NULL);
    }

    InfraellyMixer::stopMusic();
    InfraellyMixer::popFrontMusicQueue();
    InfraellyMixer::playMusic();
}


void CreditContext::events(SDL_Event &event){
}

void CreditContext::input(Uint8 *keys){
    //check key input
    //move credits up
    if( keys[SDLK_UP] ){ if( y < screen->h ){ y -= 5; } };
    //move credits down
    if( keys[SDLK_DOWN] ){ if( y > totalHeight ){ y += 5; } };
}

void CreditContext::logic(){
    //if the text is off the screen, put it back on.
    if( --y < totalHeight ){
        y = screen->h;
    }
}

void CreditContext::draw(){
    //clear screen
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

    //draw listing
    int width = 0;
    int x = 0;
    for(size_t i = 0; i < creditListing.size(); ++i ){
        width = getStringWidth(font::creditFont.at(fontSize), creditListing.at(i));
        x = centerX( width, screen->w );
        drawText( creditListing.at(i),  font::creditFont.at(fontSize),
                  colour::white,  screen,  x,  line*i+y );
    }

    //draw black box and "Press Esc to return"
    SDL_FillRect(screen, &escBox, SDL_MapRGB(screen->format,0,0,0));
    drawText( "Press Esc to return", font::mainFont.at(16), colour::yellow,
              screen, 0, 0);
}
