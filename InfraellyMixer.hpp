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


#ifndef INFRAELLYMIXER_HPP_INCLUDED
#define INFRAELLYMIXER_HPP_INCLUDED

//  InfraellyMixer.hpp


#include <string>
#include <map>
#include <deque>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


class InfraellyMixer{
    public:
        //  Pre-load a sound into cache
        static void loadSound(const std::string& filename);
        //  Pre-loads music into cache
        static void loadMusic(const std::string& filename);

        //  (Loads a sound into cache) and plays the sound
        static void playSound(const std::string& filename);
        //  Stop all sounds
        static void stopSounds();

        //  Add file to back of playlist
        static void pushBackMusicQueue(const std::string& filename, int loops = -1);
        //  Add file to front of playlist
        static void pushFrontMusicQueue(const std::string& filename, int loops = -1);
        //  Removes front item in playlist
        static void popFrontMusicQueue();
        //  Removes  last item in playlist
        static void popBackMusicQueue();
        //  Removes first item in playlist
        //static void popFrontMusicQueue();
        //  Removes all items in playlist
        static void clearMusicQueue();

        // Add file to front of playlist and plays it.
        //  This will stop any already playing music
        static void playMusic(const std::string& filename, int loops = -1,int ms=1000);
        //  Start playback of music playlist
        static void playMusic(int ms=1000);
        //  Stop playback of music playlist, ms is fade out amount
        static void stopMusic(int ms=0);
        //  Pause playback of music playlist
        static void pauseMusic();
        //  Resume playback of music playlist
        static void resumeMusic();


        //  Returns true if music is active (playing/paused)
        static bool isActive();
        //  Returns true if music is playing
        static bool isPlaying();
        //  Returns true if music is paused
        static bool isPaused();


        //toggles use of mixer
        static void enableSound();
        static void disableSound();
        static void setSoundEnabled(bool value);
        static bool soundEnabled();

        static void enableMusic();
        static void disableMusic();
        static void setMusicEnabled(bool value);
        static bool musicEnabled();



        //  Performs Mixers logic functions, like playlist control
        static void logic();


        //  Unloads all music and sounds
        static void freeAudio();



    private:
        //  This is a playlist for music
        static std::deque< std::pair<std::string, int> > musicQueue;

        //  This is a cache for music
        static std::map<std::string, Mix_Music*> musicCache;

        //  This is a cache for sound effects
        static std::map<std::string, Mix_Chunk*> soundCache;

        #ifdef USE_PACKED_RESOURCES
            static std::vector<SDL_RWops*> dataRwops;
        #endif
};




#endif // INFRAELLYMIXER_HPP_INCLUDED
