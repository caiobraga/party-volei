#pragma once
#include <SDL.h>
#include "./PlayerProcessor.h"

class InputProcess {
public:
    PlayerProcessor *mPlayerProcessor;

    InputProcess(PlayerProcessor *playerProcessor);
    ~InputProcess();

    void Process(const uint8_t* state);

};
