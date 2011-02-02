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


#include "init.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_net.h>

#include "guichan.hpp"
#include "guichan/sdl.hpp"

#include "InfraellyImageLoader.hpp"

#include "Screen.hpp"
#include "InfraellyMixer.hpp"

#include "GameConfig.hpp"

#include "fonts.hpp"
#include "colours.hpp"
#include "dataPacks.hpp"

using std::cerr;
using std::cout;
using std::endl;



static bool inited = 0;


void init(){
    if( !inited ){
        inited = true;
    } else {
        return;
    }

    //allows random numbers
    srand( time(NULL) );
    //set exit function
    atexit(closeInfraelly);

    //decorate/prepare the out put log
    cerr << "                         ____                                                  " << endl;
    cerr << "      __                / __ \\                                /\\     /\\        " << endl;
    cerr << "     /_/               / /  \\/                               / /    / /        " << endl;
    cerr << "     __    ____       / /_    ____     ____       ____      / /    / /         " << endl;
    cerr << "    / /   / __ \\     / ___\\  / __ \\   / __ \\     / __ \\    / /    / /   /\\  /\\ " << endl;
    cerr << "   / /   / /  \\ \\   / /     / /  \\/  / /  \\ \\   / ____/   / /    / /   / / / / " << endl;
    cerr << "  / /_  / /   / /  / /     / /       \\ \\__/ /_  \\  \\___  / /_   / /_   \\ \\/ /  " << endl;
    cerr << "  \\__/  \\/    \\/   \\/      \\/         \\______/   \\____/  \\__/   \\__/    \\  /   " << endl;
    cerr << "                                                                        / /    " << endl;
    cerr << " ______________________________________________________________________/ /     " << endl;
    cerr << "/   ____________________________________________________________________/      " << endl;
    cerr << "\\__/                                                                           " << endl;
    cerr << "                                           Copyright Infraelly Team 2007-2010  " << endl;
    cerr << endl << endl;


    /*---------------------------------------------
                    Load user settings
    ---------------------------------------------*/
    #ifndef DEBUG
    cout << "Load Infraelly settings (delete \"infraelly.cfg\" to restore defaults)" << endl;
    if( !GameConfig::loadXml("infraelly.cfg") ){
        GameConfig::saveXml("infraelly.cfg");
    }
    #else
        cout << "Infraelly settings not loading from file. (DEBUG is enabled)" << endl;
    #endif

    //putenv("SDL_VIDEODRIVER=x11");


    /********************************************************

                        initialize SDL

    *********************************************************/
    /*------------------------------------------
             initialize SDL_main
    -------------------------------------------*/
    cerr << __FILE__ << " " << __LINE__ << ": " << "Initialising SDL" << endl;
    //try initiate everything
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
        cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialise: SDL_INIT_EVERYTHING" << endl;
        Uint32 initiated = SDL_WasInit(SDL_INIT_EVERYTHING);
        bool continueAnyway = 1;
        //check if video was initiated
        if ( !(initiated & SDL_INIT_VIDEO) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialise: SDL_INIT_VIDEO" << endl;
            continueAnyway = 0;
        }
        //check if the timer systems were initiated properly
        if ( !(initiated & SDL_INIT_TIMER) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialise: SDL_INIT_TIMER" << endl;
            continueAnyway = 0;
        }
        //check i the audio systems were initiated properly
        if ( !(initiated & SDL_INIT_AUDIO) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialise: SDL_INIT_AUDIO" << endl;
            GameConfig::sound = 0;
            GameConfig::music = 0;
        }
        //check if CD-Rom capabilities were initiated properly
        /*if ( !(initiated & SDL_INIT_CDROM) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialise: SDL_INIT_CDROM" << endl;
            continueAnyway = 1;
        }
        //check if the joystick capabilities were initiated properly
        if ( !(initiated & SDL_INIT_JOYSTICK) ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialise: SDL_INIT_JOYSTICK" << endl;
            continueAnyway = 1;
        }*/
        //depending on what was succesful, continue or not
        cerr << __FILE__ << " " << __LINE__ << ": " << SDL_GetError() << endl;
        if (continueAnyway){
            cerr << __FILE__ << " " << __LINE__ << ": " << "Continuing anyway..." << endl << endl;
        } else {
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to continue" << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Initialised: SDL_INIT_EVERYTHING" << endl << endl;
    }

    /*************************************************************

                        Splash Screen

    *************************************************************/
    //make splash appear in center of screen
    putenv((char*)"SDL_VIDEO_CENTERED=1");

    //load splash image
    SDL_Surface *splashBG;
    {   //makes SPLASH_xpm structure temp, saves mem
        #include "splash_png.h"
        SDL_RWops *splash_png_Rwop = SDL_RWFromMem(splash_png, SPLASH_PNG_LEN);
        splashBG = IMG_Load_RW(splash_png_Rwop, 1);
    }
    if(!splashBG) {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to load splash" << endl;
       cerr << __FILE__ << " " << __LINE__ << ": " << "IMG_Load_RW(splash_png_Rwop, 1): " << IMG_GetError() << endl;
    } else {
        //make window
        cerr << __FILE__ << " " << __LINE__ << ": " << "Creating splash" << endl << endl;
        Screen::init(splashBG->w, splashBG->h, splashBG->format->BitsPerPixel, SDL_SWSURFACE|SDL_ANYFORMAT|SDL_NOFRAME);
    }
    //display Splash Image
    drawSurface(splashBG, Screen::getSurface(), 0, 0);
    Screen::flip();

    const SDL_VideoInfo *info = Screen::getVideoInfo();
    char dName[16];
    cerr << __FILE__ << " " << __LINE__ << ": " << "SDL reports:" << endl;
    cerr << __FILE__ << " " << __LINE__ << ": " << "Driver: " << SDL_VideoDriverName(dName, 16) << endl;
    if( info != NULL ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "Possible to create hardware surfaces: " << info->hw_available << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Window manager available: " << info->wm_available << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated hardware to hardware blits: " << info->blit_hw << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated hardware to hardware colorkey blits: " << info->blit_hw_CC << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated hardware to hardware alpha blits: " << info->blit_hw_A << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated software to hardware blits: " << info->blit_sw << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated Are software to hardware colorkey blits: " << info->blit_sw_CC << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated software to hardware alpha blits: " << info->blit_sw_A << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Accelerated color fills: " << info->blit_fill << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Total video memory (Kb): " << info->video_mem << endl;
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to get video info." << endl;
    }
    cerr << endl;

    /*************************************************************

                        SDL addons
ů
    *************************************************************/
    /*------------------------------------------
                initiallise SDL_mixer
    -------------------------------------------*/
    cerr << __FILE__ << " " << __LINE__ << ": " << "Initialising SDL_mixer" << endl;
    if (Mix_OpenAudio(GameConfig::audioRate, MIX_DEFAULT_FORMAT, 2, GameConfig::aduioBuffSize) == -1){
        cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialised: SDL_mixer" << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << Mix_GetError() << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Continue anyway" << endl;
        GameConfig::sound = 0;
        GameConfig::music = 0;
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Initialised: SDL_mixer" << endl << endl;
    }
    Screen::flip();
    //load default value into infraelly mixer
    InfraellyMixer::setSoundEnabled(GameConfig::sound);
    InfraellyMixer::setMusicEnabled(GameConfig::music);

    /*------------------------------------------
                initiallise SDL_ttf
    ------------------------------------------*/
    cerr << __FILE__ << " " << __LINE__ << ": " << "Initialising SDL_ttf" << endl;
    if (TTF_Init() == -1){
        cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialised: SDL_ttf" << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << TTF_GetError() << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to continue" << endl;
        exit(EXIT_FAILURE);
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Initialised: SDL_ttf" << endl << endl;
    }
    Screen::flip();

    /*------------------------------------------
                initiallise SDL_net
    ------------------------------------------*/
    cerr << __FILE__ << " " << __LINE__ << ": " << "Initialising SDL_net" << endl;
    if( SDLNet_Init() == -1 ){
        cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialised: SDLNet_Init" << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << SDLNet_GetError() << endl;
        cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to continue" << endl;
        exit(EXIT_FAILURE);
    } else {
        cerr << __FILE__ << " " << __LINE__ << ": " << "Initialised: SDL_net" << endl << endl;
    }
    Screen::flip();

    /*------------------------------------------
                setup window icon
    ------------------------------------------*/
    //load title bar icon
    SDL_Surface *icon;

    {   //this makes ICON_xpm[] structure temp to save mem
        #include "ICON_xpm.xpm"
        icon = IMG_ReadXPMFromArray(ICON_xpm);
    }
    if(!icon) {
       cerr << __FILE__ << " " << __LINE__ << ": " << "IMG_ReadXPMFromArray: " << IMG_GetError() << endl;
    }
    //setwindow icon
    Screen::setIcon(icon);
    SDL_FreeSurface(icon);
    //set window caption
    Screen::setCaption("Infraelly: loading...");
    Screen::flip();

    /*--------------------------------------------
                  load main resources
    --------------------------------------------*/
    packs::loadPacks();
    InfraellyMixer::playMusic("audio/loader.ogg", -1);
    font::loadFonts();
    Screen::flip();

    /*-------------------------------
         Unload splash resources
    -------------------------------*/
    //free spalsh bg
    SDL_FreeSurface(splashBG);


    /*************************************************************************

                            Initialise screen for game

    **************************************************************************/
    cerr << __FILE__ << " " << __LINE__ << ": " << "Initialising screen for main game" << endl;
    if( GameConfig::startFullscreen ){
        if( Screen::init( GameConfig::fScreenWidth,
                          GameConfig::fScreenHeight,
                          GameConfig::fScreenBpp,
                          SDL_HWSURFACE|SDL_HWACCEL|SDL_FULLSCREEN|SDL_SRCALPHA|
                          SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_RESIZABLE) == 0 ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialised: screen surface, on hardware surface" << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << SDL_GetError() << endl << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to continue" << endl << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        if( Screen::init( GameConfig::wScreenWidth,
                          GameConfig::wScreenHeight,
                          GameConfig::wScreenBpp,
                          SDL_HWSURFACE|SDL_HWACCEL|SDL_SRCALPHA|
                          SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_RESIZABLE) == 0 ){
            cerr << __FILE__ << " " << __LINE__ << ": " << "SDL failed to initialised: screen surface, on hardware surface" << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << SDL_GetError() << endl << endl;
            cerr << __FILE__ << " " << __LINE__ << ": " << "Unable to continue" << endl << endl;
            exit(EXIT_FAILURE);
        }
    }

    cerr << __FILE__ << " " << __LINE__ << ": " << "Screen initialised: " << Screen::getWidth() << "x" << Screen::getHeight() << "x" << Screen::getBpp() << endl;


    //enable unicode input
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    //create an image loader, to load sdl images into guichan
    InfraellyImageLoader *imageLoader = new InfraellyImageLoader();
    //set guichan to use the image loader afore created
    gcn::Image::setImageLoader(imageLoader);
    //set the defualt font to font loaded.
    gcn::Widget::setGlobalFont(font::guiFont.at(font::stdGuiFontSize));
    //text to display in window bar
    Screen::setCaption( "Infraelly" );
    InfraellyMixer::stopMusic();
    InfraellyMixer::clearMusicQueue();

    // so that when the window is resized it wont snap to the center of the
    // screen, i find it annoying when it does that.
    putenv((char*)"SDL_VIDEO_CENTERED=0");
}







void closeInfraelly(){
    cerr << __FILE__ << " " << __LINE__ << ": " << "Infraelly Shutdown." << endl << endl;

    //clear sounds and music
    InfraellyMixer::freeAudio();

    //free fonts
    font::freeFonts();

    cerr << __FILE__ << " " << __LINE__ << ": " << "Closing SDL_TTF..." << endl;
    TTF_Quit();
    cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_TTF closed" << endl << endl;

    cerr << __FILE__ << " " << __LINE__ << ": " << "Closing SDL_mixer..." << endl;
    Mix_CloseAudio();
    cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_mixer closed" << endl << endl;

    cerr << __FILE__ << " " << __LINE__ << ": " << "Closing SDL_net..." << endl;
    SDLNet_Quit();
    cerr << __FILE__ << " " << __LINE__ << ": " << "SDL_net closed" << endl << endl;

    cerr << __FILE__ << " " << __LINE__ << ": " << "Closing SDL..." << endl;
    SDL_Quit();
    cerr << __FILE__ << " " << __LINE__ << ": " << "SDL closed" << endl << endl;
}
