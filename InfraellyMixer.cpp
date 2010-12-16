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


//  InfraellyMixer.cpp

#include "InfraellyMixer.hpp"

#include <SDL/SDL.h>
#include <iostream>
#include "GameConfig.hpp"
#include "globalFunc.hpp"

#ifdef USE_PACKED_RESOURCES
    #include "dataPacks.hpp"
#endif



//  Music play list
std::deque< std::pair<std::string, int> > InfraellyMixer::musicQueue;
//  Sound effects cache
std::map<std::string, Mix_Chunk*> InfraellyMixer::soundCache;
//  Music cache
std::map<std::string, Mix_Music*> InfraellyMixer::musicCache;

//rwop data
#ifdef USE_PACKED_RESOURCES
    std::vector<SDL_RWops*> InfraellyMixer::dataRwops;
#endif


void InfraellyMixer::loadSound(const std::string& filename){
    std::string fixedFn = correctFilepath(filename);
    if( soundCache.find(fixedFn) == soundCache.end() ){
        Mix_Chunk *sound;
        #ifdef USE_PACKED_RESOURCES
            ResourceFile rcf;
            packs::mainPack.getResource( fixedFn, rcf );
            sound = Mix_LoadWAV_RW( SDL_RWFromMem((void*)&packs::mainPack.getRawData()[rcf.start], rcf.size), true );
        #else
            sound = Mix_LoadWAV(fixedFn.c_str());
        #endif
        if( sound != NULL ){
            soundCache[fixedFn] = sound;
        } else {
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to load: " << fixedFn << ". " << Mix_GetError() << std::endl;
        }
    }
}

void InfraellyMixer::loadMusic(const std::string& filename){
    std::string fixedFn = correctFilepath(filename);
    if( musicCache.find(fixedFn) == musicCache.end() ){
        Mix_Music *music;
        #ifdef USE_PACKED_RESOURCES
            ResourceFile rcf;
            packs::mainPack.getResource( fixedFn, rcf );
            SDL_RWops *rw = SDL_RWFromMem((void*)&packs::mainPack.getRawData()[rcf.start], rcf.size);
            if( rw != NULL ){
                music = Mix_LoadMUS_RW( rw );
            }
            // SDL_mixer cant dupe the rwop and will use the one provided
            // freeing this will cause crashes
            // http://www.gamedev.net/community/forums/topic.asp?topic_id=418244
            dataRwops.push_back(rw);
        #else
            music = Mix_LoadMUS(fixedFn.c_str());
        #endif

        if( music != NULL ){
            musicCache[fixedFn] = music;
        } else {
            std::cerr << __FILE__ << " " << __LINE__ << ": " << "Failed to load: " << fixedFn << ". " << Mix_GetError() << std::endl;
        }
    }
}


void InfraellyMixer::playSound(const std::string& filename){
    std::string fixedFn = correctFilepath(filename);
    //  ensure sound is loaded
    loadSound(fixedFn);
    if( GameConfig::sound ){
        //  play sound
        Mix_PlayChannel(-1, soundCache[fixedFn], 0);
    }
}

void InfraellyMixer::stopSounds(){
    Mix_HaltChannel(-1);
}

void InfraellyMixer::playMusic(const std::string& filename, int loops, int ms){
    std::string fixedFn = correctFilepath(filename);
    stopMusic();
    pushFrontMusicQueue(fixedFn, loops);
    if( GameConfig::music ){
        playMusic(ms);
    }
}

void InfraellyMixer::playMusic(int ms){
    if( GameConfig::music ){
        if( !musicQueue.empty() && !Mix_PlayingMusic() ){
            //  ensure music track is loaded
            loadMusic(musicQueue.front().first);
            //  play music
            Mix_FadeInMusic(musicCache[musicQueue.front().first], musicQueue.front().second, ms);
        }
    }
}

void InfraellyMixer::stopMusic(int ms){
    if( ms ){
        Mix_FadeOutChannel(-1, ms);
    } else {
        Mix_HaltMusic();
    }
}

void InfraellyMixer::pauseMusic(){ Mix_PauseMusic(); }

void InfraellyMixer::resumeMusic(){
    if( GameConfig::music ){
        Mix_ResumeMusic();
    }
}




void InfraellyMixer::pushBackMusicQueue(const std::string& filename, int loops){
    musicQueue.push_back(make_pair(correctFilepath(filename), loops));
}

void InfraellyMixer::pushFrontMusicQueue(const std::string& filename, int loops){
    musicQueue.push_front(make_pair(correctFilepath(filename), loops));
}

void InfraellyMixer::popBackMusicQueue(){ musicQueue.pop_back(); }

void InfraellyMixer::popFrontMusicQueue(){ musicQueue.pop_front(); };

void InfraellyMixer::clearMusicQueue(){ musicQueue.clear(); }







bool InfraellyMixer::isActive(){ return Mix_PlayingMusic(); }

bool InfraellyMixer::isPlaying(){
    return Mix_PlayingMusic() && !Mix_PausedMusic();
}

bool InfraellyMixer::isPaused(){ return Mix_PausedMusic(); }




void InfraellyMixer::enableSound(){ GameConfig::sound = true; }

void InfraellyMixer::disableSound(){ GameConfig::sound = false; }

void InfraellyMixer::setSoundEnabled(bool value){ GameConfig::sound = value; }

bool InfraellyMixer::soundEnabled(){ return GameConfig::sound; }


void InfraellyMixer::enableMusic(){ GameConfig::music = true; }

void InfraellyMixer::disableMusic(){ GameConfig::music = false; }

void InfraellyMixer::setMusicEnabled(bool value){ GameConfig::music = value; }

bool InfraellyMixer::musicEnabled(){ return GameConfig::music; }





void InfraellyMixer::logic(){
    if( GameConfig::music ){
        if( !Mix_PlayingMusic() && !musicQueue.empty() ){
            musicQueue.pop_front();
            playMusic();
        }
    }
}



void InfraellyMixer::freeAudio(){
    // fade all sound chanels, and music out over 200ms
    Mix_FadeOutChannel(-1, 150);
    while( (!Mix_FadeOutMusic(150) && Mix_PlayingMusic()) || Mix_Playing(-1) ) {
        // wait for any fades to complete
        SDL_Delay(50);
    }
    Mix_HaltChannel(-1);
    Mix_HaltMusic();

    //  Delete all sound effects
    std::cerr << __FILE__ << " " << __LINE__ << ": " << "Clearing sound effects..." << std::endl;
    std::map<std::string, Mix_Chunk*>::iterator soundIter;
    for(soundIter = soundCache.begin(); soundIter != soundCache.end(); ++soundIter){
        //  free mix chunk
        Mix_FreeChunk( soundIter->second );
        //  set null ptr to mix chunk
        soundIter->second = NULL;
    }
    //  clear the map
    soundCache.clear();
    std::cerr << __FILE__ << " " << __LINE__ << ": " << "Sound effects cleared" << std::endl << std::endl;

    //  Delete all music
    std::cerr << __FILE__ << " " << __LINE__ << ": " << "Clearing music..." << std::endl;
    std::map<std::string, Mix_Music*>::iterator musicIter;
    for(musicIter = musicCache.begin(); musicIter != musicCache.end(); ++musicIter){
        //  free mix chunk
        Mix_FreeMusic( musicIter->second );
        //  set null ptr to mix chunk
        musicIter->second = NULL;
    }
    //free the rwops
    #ifdef USE_PACKED_RESOURCES
        for(size_t i = 0; i < dataRwops.size(); ++i){
            SDL_FreeRW( dataRwops[i] );
            dataRwops[i] = NULL;
        }
        dataRwops.clear();
    #endif
    //  clear the map
    musicCache.clear();
    std::cerr << __FILE__ << " " << __LINE__ << ": " << "Music cleared" << std::endl << std::endl;
}
