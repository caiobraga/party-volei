#pragma once

#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H	

#include "SDL.h"
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include "Music.h"

class MusicManager
{
private:
	std::vector<Mix_Music*> vMusic;

	std::vector<Mix_Chunk*> vChunk;

public:
	MusicManager(void);
	~MusicManager(void);

	Mix_Music* loadMusic(std::string fileName);

	void PlayMusic();
    Music mMusic = Music();
    void PlayHit();


};

#endif