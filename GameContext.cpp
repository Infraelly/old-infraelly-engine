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


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "globalFunc.hpp"

#include "Screen.hpp"

#include "GameContext.hpp"

#include "InfraellyMixer.hpp"
#include "GameConfig.hpp"
#include "colours.hpp"
#include "fonts.hpp"

using namespace inp;


GameContext::GameContext(Connection* serverCon) :
       chatSenderCallBack(this),
       serverConnection(serverCon),
       pingRequested(false)
{
    //quick check to make sure a bad connection wasn't passed in
    if( serverCon == NULL ){
        //just make one so that the context wont crash from acessing null ptr
        serverConnection = new Connection;
    } else {
        player.setName(serverCon->getId());
    }

    // Set window caption
    SDL_WM_SetCaption("Infraelly", NULL);
    //to control fps
    FpsManager fpsManager(GameConfig::fps);

    // set chatbox call back
    gui = new GameGui(screen, &player);
    gui->getChatBox()->setInputCallBack(&chatSenderCallBack);
    gui->getChatBox()->setUsername( serverConnection->getId() );
    gui->getChatBox()->setFlags(ChatBox::DO_CALLBACK|ChatBox::PUT_USERNAME);

    //start theme music, infinite loop
    InfraellyMixer::stopMusic();
    InfraellyMixer::playMusic("audio/walking_on_air.ogg", -1);

    // init player (form socket later)
    player.setX(Screen::getWidth()/2);
    player.setY(Screen::getHeight()/2);
    player.setName("The one who needs not a name");

    //character object behaves badly with repeats enabled
    //the chat will enable it when its needed
    SDL_EnableKeyRepeat(0, 0);

    movingUpdateTimer.start();
    staticUpdateTimer.start();
    //start
    run();
}

GameContext::~GameContext(){
    serverConnection->disconnect();
    delete gui;
    InfraellyMixer::stopMusic();
    InfraellyMixer::popFrontMusicQueue();
    InfraellyMixer::playMusic();
}

void GameContext::postToServer(const std::string& data){
    INFPacket packet;
    packet << DataTypeByte::USERNAME << serverConnection->getId();
    packet << DataTypeByte::CHAT << data;
    serverConnection->send(packet);
    // User chat animate
    player.speechBubble.setText(data);
}

int GameContext::handleIncomingNetwork(){
    //  Check for activity
    inp::INFPacket packet;
    int numReady = serverConnection->checkSet(10);
    if( numReady == -1 ) {
        serverConnection->disconnect();
        return ERR;
    } else if( numReady > 0 ){
        //  Recieve data and store "packet"
        if( serverConnection->recv(packet) != -1 ){
          //  std::cout << packet << std::endl;
         //   std::cout << std::endl;

            inp::NetCode code;
            Character* user = NULL;
            std::string username = "";
            std::string recvText = "";
            Sint32 recvVal = 0;
            //  Recursively read all data in packet
            while( !packet.readDone() ){
                packet >> code;

                //  If theres nothing left, or corupt packet
                if( code == ControlByte::NOTHING ){ break; }

                //  Recieving chat relay from server
                if( code == DataTypeByte::CHAT ){
                    packet >> recvText;
                    if( recvText != "" ){
                        if( user != &player ){
                            gui->getChatBox()->push_back(username+": "+recvText, ChatBox::DO_NOTHING);
                            user->speechBubble.setText(recvText);
                        }
                    }
                } else
                //  A player has joined
                if( code == DataTypeByte::USER_JOIN ){
                    packet >> recvText;
                    if( recvText != "" ){
                        if( recvText != serverConnection->getId() ){
                           remotePlayers[recvText] = Character();
                        }
                    }
                }else
                //  A player has left
                if( code == DataTypeByte::USER_LEFT ){
                    packet >> recvText;
                    if( recvText != "" ){
                        remotePlayers.erase(recvText);
                    }
                }else
                //  Server is trying to tell us something about another user
                if( code == DataTypeByte::USERNAME ){
                    packet >> recvText;
                    if( recvText != "" ){
                        username = recvText;
                        if( recvText == serverConnection->getId() ){
                            user = &player;
                        } else
                        if( !remotePlayers.empty() && (remotePlayers.find(username) != remotePlayers.end()) ){
                            user = &remotePlayers[username];
                        } else {
                            // dunno who server is talking about =/
                            break;
                        }
                    }
                } else
                //  Server is telling us a user's name
                if( code == DataTypeByte::CHAR_NAME ){
                    if( user == NULL ){ break; }
                    packet >> recvText;
                    if( recvText != "" ){ user->setName(recvText); }
                } else
                //  Server is telling us a user's x-pos
                if( code == DataTypeByte::CHAR_X ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    int velocity = user->getXVel();
                    user->setX(recvVal);
                    user->setXVel(velocity);
                } else
                //  Server is telling us a user's y-pos
                if( code == DataTypeByte::CHAR_Y ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    int velocity = user->getYVel();
                    user->setY(recvVal);
                    user->setYVel(velocity);
                } else
                //  Server is telling us a user's class
                if( code == DataTypeByte::CHAR_CLASS ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    if( Character::validJob(recvVal) ){
                        user->setClass( static_cast<Character::Jobs>(recvVal) );
                    }
                } else
                //  Server is telling us a user's gender
                if( code == DataTypeByte::CHAR_GENDER ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    if( Character::validGender(recvVal) ){
                        user->setGender( static_cast<Character::Genders>(recvVal) );
                    }
                } else
                //  Server is telling us a user's direction (facing)
                if( code == DataTypeByte::CHAR_DIR ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    if( Character::validDirection(recvVal) && user!=&player ){
                        user->setDirection( static_cast<Directions>(recvVal) );
                    }
                } else
                //  Server is telling us a user's State (still, attacking etc)
                if( code == DataTypeByte::CHAR_STATE ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    if( Character::validState(recvVal) && user!=&player){
                        user->setState( static_cast<Character::CharacterStates>(recvVal) );
                    }
                } else
                //  Server is telling us a user's Stat (hp,mp etc)
                if( code == DataTypeByte::CHAR_STAT_TYPE ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                } else
                if( code == DataTypeByte::CHAR_STAT_VAL ){
                    if( user == NULL ){ break; }
                    if( Stats::validStat(recvVal) ){
                        Stats::StatObjects statT = static_cast<Stats::StatObjects>(recvVal);
                        packet >> recvVal;
                        player.stats.setValue(statT, recvVal);
                    }
                } else
                //  The server commands thee to move!
                if( code == DataTypeByte::CHAR_MOVE ){
                    if( user == NULL ){ break; }
                    packet >> recvVal;
                    if( Character::validDirection(recvVal) ){
                        user->move( Directions(recvVal) );
                    }
                } else
                //  The server commands thee to stop!
                if( code == DataTypeByte::CHAR_STOP ){
                    if( user == NULL ){ break; }
                        user->stop();
                } else

                //  The server has kicked us =[
                if( code == DataTypeByte::KICK ){
                    packet >> recvText;
                    if( recvText != "" ){
                        gui->getChatBox()->push_back("SERVER: "+recvText, ChatBox::DO_NOTHING);
                    }
                    return DC;
                } else
                //  A message from the server
                if( code == DataTypeByte::SERVER_MSG ){
                    packet >> recvText;
                    if( recvText != "" ){
                        gui->getChatBox()->push_back("SERVER: "+recvText, ChatBox::DO_NOTHING);
                    }
                } else
                //  The server has requested our ping.
                //  obtain ping to send =O
                if( code == DataTypeByte::PING_REQ ){
                    pingRequested = true;
                    INFPacket pingPack;
                    pingPack << DataTypeByte::PING;
                    serverConnection->send(pingPack);
                    pingRequestTimer.stop();
                    pingRequestTimer.clear();
                    pingRequestTimer.start();
                    break;
                } else
                //  The server has echoed our ping
                if( code == DataTypeByte::PONG ){
                    if( pingRequested ){
                        pingRequested = false;
                        pingRequestTimer.stop();
                        int ping  = pingRequestTimer.getTime();
                        INFPacket pingPack;
                        pingPack << DataTypeByte::USERNAME << serverConnection->getId()
                                 << DataTypeByte::PING_TIME << ping;
                        serverConnection->send(pingPack);
                    } else {
                        pingTimer.stop();
                        int ping = pingTimer.getTime();
                        std::ostringstream pingStr;
                        pingStr << "Ping: " << ping;
                        gui->getChatBox()->push_back( pingStr.str(), ChatBox::DO_NOTHING );
                    }
                    break;
                } else
                //  The server has requested our INP version
                if( code == DataTypeByte::REQ_INP_VER ){
                    packet.clear();
                    packet << DataTypeByte::INP_VER << inp::inp_ver;
                    serverConnection->send(packet);
                    break;
                } else {
                    gui->getChatBox()->push_back( "Unhandled Packet:" + itos(code.getVal()) );
                }
            }//end while(packet.readDone());
            return GOOD;
        } else {
            //been disconected
            return DC;
        }
    }
    return GOOD;
}


void GameContext::events(SDL_Event &event){
     if( event.type == SDL_VIDEORESIZE ){
        gui->resetPositions();
     }

    bool feedGui = 1;
    // A key has been pressed
    if( event.type == SDL_KEYDOWN ){
        switch( event.key.keysym.sym ){
            case SDLK_ESCAPE:
                //exit
                serverConnection->disconnect();
                done = 1;
                break;

            case SDLK_F4:
                //fullscreen toggle
                Screen::toggleFullscreen();
                gui->resetPositions();
                feedGui = 0;
                break;

            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                //enable chat box
                if( !gui->getChatBox()->isTyping() ){
                    feedGui = 0;
                    gui->getChatBox()->focus();
                    player.setState(Character::TYPING);
                } else {
                    player.lockState(false);
                    player.setState(Character::STILL);
                }
                break;

            //direction keys
            case SDLK_UP:
                if( !gui->getChatBox()->isTyping() ){
                    if(player.getY() > 0){
                        player.move( UP );
                    }
                }
                break;

            case SDLK_LEFT:
                if( !gui->getChatBox()->isTyping() ){
                    if(player.getX() > 0){
                        player.move( LEFT );
                    }
                }
                break;

            case SDLK_RIGHT:
                if( !gui->getChatBox()->isTyping() ){
                    if(player.getX() < screen->w-40){
                        player.move( RIGHT );
                    }
                }
                break;

            case SDLK_DOWN:
                if( !gui->getChatBox()->isTyping() ){
                    if(player.getY() < screen->h-70){
                        player.move( DOWN );
                    }
                }
                break;

            default:
                break;
        }// end key down
    }


    // A key has been released
    if( event.type == SDL_KEYUP ){
        switch( event.key.keysym.sym ){
            //direction keys
            case SDLK_UP:
                if( !gui->getChatBox()->isTyping() ){
                    player.setYVel(0);
                }
                break;
            case SDLK_DOWN:
                //stop vert motion
                if( !gui->getChatBox()->isTyping() ){
                    player.setYVel(0);
                }
                break;

            case SDLK_LEFT:
                //stop hor motion
                if( !gui->getChatBox()->isTyping() ){
                    player.setXVel(0);
                }
                break;
            case SDLK_RIGHT:
                //stop hor motion
                if( !gui->getChatBox()->isTyping() ){
                    player.setXVel(0);
                }
                break;

            default:
                break;
        }// end key up
    }//feed gui if its allowed

    if( feedGui ){ gui->pushInput(event); }
}


void GameContext::input(Uint8 *keys){
    // Attack if the control key is down
    if( keys[KMOD_LCTRL] ){ player.attack(); }

    // If no directional buttons are pressed, stop player
    if( !keys[SDLK_UP]   && !keys[SDLK_DOWN] &&
        !keys[SDLK_LEFT] && !keys[SDLK_RIGHT] ){
        if( player.getCharacterState() == Character::MOVING ){
           //stop animation
           player.stop();
        }
    }

    //if not chatting
    if( !gui->getChatBox()->isTyping() ){
        //some debug values
        if ( keys[SDLK_h] ) { player.stats.addValue(Stats::HP, 1); }
        if ( keys[SDLK_g] ) { player.stats.addValue(Stats::HP, -1); }

        if ( keys[SDLK_m] ) { player.stats.addValue(Stats::MP, 1); }
        if ( keys[SDLK_n] ) { player.stats.addValue(Stats::MP, -1); }

        if ( keys[SDLK_l] ) { player.stats.addValue(Stats::LVL, 10); }
        if ( keys[SDLK_k] ) { player.stats.addValue(Stats::LVL, -10); }

        if ( keys[SDLK_s] ) { player.stats.addValue(Stats::STAM, 1); }
        if ( keys[SDLK_a] ) { player.stats.addValue(Stats::STAM, -1); }
    }
}


void GameContext::logic(){
    //handle network events
    if( serverConnection->isActive() ){
        switch( handleIncomingNetwork() ){
            case GOOD:
                break;

            case DC:
                gui->alert("Server Disconnected =[");
                gui->getChatBox()->push_back("Server Disconnected", ChatBox::DO_NOTHING);
                serverConnection->disconnect();
                break;

            case ERR:
            default:
                done = 1;
                break;
        }
        //update xy
        if( player.getXVel() || player.getYVel() ){
            if( movingUpdateTimer.getTime() > movingUpdateDelta ){
                INFPacket pack;
                pack << DataTypeByte::USERNAME << serverConnection->getId();
                pack << DataTypeByte::CHAR_X << player.getX();
                pack << DataTypeByte::CHAR_Y << player.getY();
                pack << DataTypeByte::CHAR_DIR << player.getDirection();
                pack << DataTypeByte::CHAR_STATE << player.getState();
                serverConnection->send(pack);
                movingUpdateTimer.clear();
            }
        } else {
            if( staticUpdateTimer.getTime() > staticUpdateDelta ){
                INFPacket pack;
                pack << DataTypeByte::USERNAME << serverConnection->getId();
                pack << DataTypeByte::CHAR_X << player.getX();
                pack << DataTypeByte::CHAR_Y << player.getY();
                pack << DataTypeByte::CHAR_DIR << player.getDirection();
                pack << DataTypeByte::CHAR_STATE << player.getState();
                serverConnection->send(pack);
                staticUpdateTimer.clear();
            }
        }
    }

    //  mixer logic \O.o/
    InfraellyMixer::logic();
    // let giuchan do its thang!
    gui->logic();
}

void GameContext::draw(){
    //clear screen
    Screen::clear(255,255,255);

    //draw remote players
    if( !remotePlayers.empty() ){
        for( std::map<std::string, Character>::iterator itr = remotePlayers.begin();
            itr != remotePlayers.end(); ++itr ){
            itr->second.draw(screen);
        }
    }

    //draw loacal player
    player.draw(screen);
    //draw gui
    gui->draw();

    #ifdef DEBUG
        //draw fps
        fpsManager.draw(screen, 0, 0);
        //draw player co-ords
        std::ostringstream playerCords;
        playerCords << "X: " << player.getX() << " Y: " << player.getY();
        drawText(playerCords.str(), font::mainFont.at(20), colour::black, screen, 0, 20);
    #endif
}



void GameContext::ChatSendCallBack::operator()(const std::string& text){
    if( ob == NULL){
        std::cerr << __FILE__ << __LINE__ << "gmContext call back, bad args" << std::endl;
        return;
    }
    //parse text for commands
    ob->comLine( text );
}


void GameContext::comLine(const std::string& commandString){
    //  Check if text is a command
    if( (commandString.at(0) == '/') && (commandString.length() > 1 ) ){
        //find end of string (exclude whitespace)
        std::string::size_type end;
        for( end = commandString.length()-1; end != -1; --end ){
            if( !isspace(commandString.at(end)) ){ ++end;break; }
        }
        int i = 1;
        std::string temp = "";
        std::string command = "";
        std::vector<std::string> arguments;
        //extract the command
        while( i < end ){
            //check if next character is whitespace
            if( isspace(commandString.at(i)) ){
                //skip the whitespace
                while( isspace(commandString.at(++i)) ){}
                break;
            }
            command += toupper(commandString.at(i));
            ++i;
        }
        //extract the arguments
        while( i < end ){
            //check if next character is whitespace
            if( isspace(commandString.at(i)) ){
                arguments.push_back(temp);
                temp = "";
                //skip the whitespace
                while( isspace(commandString.at(++i)) );
                continue;
            }
            temp += commandString.at(i);
            ++i;

            //last argument
            if( (i == end) && (temp != "") ){
                arguments.push_back(temp);
            }
        }



        //  COMMAND IMPLEMETATION
        if( (command == "PING") || (command == "S") ){
            INFPacket pingPack;
            pingPack << DataTypeByte::PING;
            pingTimer.stop();
            pingTimer.clear();
            pingTimer.start();
            serverConnection->send(pingPack);
        } else

        //chat
        if( (command == "WHISPER") || (command == "W") ){

        } else
        if( (command == "SHOUT") || (command == "S") ){

        } else

        //  Party
        if( (command == "INVITE") || (command == "I") ){

        } else
        if( (command == "KICK") || (command == "K") ){

        } else

        //  Player stats
        if( command == "STATS" ){

        } else

        if( command == "MOTD" ){
            gui->getChatBox()->push_back("o.o", ChatBox::DO_NOTHING);
        } else
        if( command == "CLEAR" ){
            gui->getChatBox()->clear();
        } else
        if( (command == "HELP") || (command == "H") ){
           gui->getChatBox()->push_back(" ", ChatBox::DO_NOTHING);
           gui->getChatBox()->push_back("---------------------");
           gui->getChatBox()->push_back("Infraelly commands");
           gui->getChatBox()->push_back("----------------------");
           gui->getChatBox()->push_back("Commands are not case sensetive");
           gui->getChatBox()->push_back("Brackets indicate short-hand command");
           gui->getChatBox()->push_back("eg. /(s)hout means: /shout and /s do the same thing");
           gui->getChatBox()->push_back(" ");
        } else {
            //unkown command
            postToServer(commandString);
        }
    } else {
        // not a command (no /)
        postToServer(commandString);
    }
}


