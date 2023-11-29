/************************
 * @author �ukasz Jakowski
 * @since  02.05.2014 16:13
 * 
 ************************/

#include "MusicManager.h"
#include <SDL2/SDL_mixer.h>
#include "Random.h"

/* ******************************************** */

MusicManager::MusicManager(void) {
	

	
}

MusicManager::~MusicManager(void) {

}

/* ******************************************** */

void MusicManager::PlayMusic() {
	vMusic.push_back(Mix_LoadMUS("../Assets/Sounds/overworld.wav"));
	vMusic.push_back(Mix_LoadMUS("../Assets/Soundsoverworld.wav"));
	Mix_VolumeMusic(100);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_PlayMusic(vMusic[0], -1);
}

/* ******************************************** */

Mix_Music* MusicManager::loadMusic(std::string fileName) {
	fileName = "files/sounds/" + fileName + ".wav";
	return Mix_LoadMUS("files/sounds/overworld.wav");
}

/* ******************************************** */

void MusicManager::PlayHit(){
    int num = Random().GetFloatRange(0, 11);
    if(num > 0  && num < 2){
      mMusic.PlayChunk(Music::cBLOCKHIT);
        return;
    }

    if(num > 2  && num < 4){
      mMusic.PlayChunk(Music::cBLOCKBREAK);
        return;
    }

    if(num > 4  && num < 6){
      mMusic.PlayChunk(Music::cBULLETBILL);
        return;
    }

    if(num > 6  && num < 8){
        mMusic.PlayChunk(Music::cFIREBALL);
        return;
    }
    if(num > 8  && num < 10){
        mMusic.PlayChunk(Music::cBRIDGEBREAK);
        return;
    }

    mMusic.PlayChunk(Music::cFIREBALL);

}