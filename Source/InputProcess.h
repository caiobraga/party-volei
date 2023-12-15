#pragma once
#include <SDL.h>
#include "./PlayerProcessor.h"
#include "./Game.h"

class InputProcess {
public:
    PlayerProcessor *mPlayerProcessor;
    Game *mGame;

    InputProcess(PlayerProcessor *playerProcessor, Game *game);
    ~InputProcess();

    void Process(const uint8_t* state);

};
