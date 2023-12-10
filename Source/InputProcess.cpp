#include "InputProcess.h"
#include "Game.h"
InputProcess::InputProcess(PlayerProcessor *playerProcessor) {
    mPlayerProcessor = playerProcessor;
}

InputProcess::~InputProcess() {}

void InputProcess::Process(const uint8_t* state) {
    int numPlayers = mPlayerProcessor->players.size();
    // Implement your input processing logic here
    // Example:

    bool isProcessingPlayerInput = true;
    if(mPlayerProcessor->players.empty()){
        isProcessingPlayerInput = false;
    }

    if(isProcessingPlayerInput){
        bool has2Players = numPlayers > 1;
        if(numPlayers > 0){
            float posP1 =  mPlayerProcessor->players[0]->GetPosition().x;
            if (posP1 >= (mPlayerProcessor->players[0]->GetGame()->GetWindowWidth() / 2 - 32) && has2Players){
                auto vel = mPlayerProcessor->players[0]->GetRigidBodyComponent()->GetVelocity();
                mPlayerProcessor->players[0]->GetRigidBodyComponent()->SetVelocity(Vector2(-mPlayerProcessor->players[0]->GetPlayerVelocity(),0));
            }
        }
        if(has2Players){
            float posP1 =  mPlayerProcessor->players[1]->GetPosition().x;
            if (posP1 <= (mPlayerProcessor->players[1]->GetGame()->GetWindowWidth() / 2 )){
                auto vel = mPlayerProcessor->players[1]->GetRigidBodyComponent()->GetVelocity();
                mPlayerProcessor->players[1]->GetRigidBodyComponent()->SetVelocity(Vector2(mPlayerProcessor->players[0]->GetPlayerVelocity(),0));
            }
        }

        if (state[SDL_SCANCODE_D] && numPlayers > 0 )
        {
            mPlayerProcessor->players[0]->GoRigth();
        }
        else if (state[SDL_SCANCODE_A] && numPlayers > 0)
        {
            mPlayerProcessor->players[0]->GoLeft();
        }
        else {
            mPlayerProcessor->players[0]->isNotRunning();
        }
        if (state[SDL_SCANCODE_W] && numPlayers > 0) {
            mPlayerProcessor->players[0]->Jump();
        }

        if (state[SDL_SCANCODE_RIGHT] && has2Players)
        {
            mPlayerProcessor->players[1]->GoRigth();
        }
        else if (state[SDL_SCANCODE_LEFT] && has2Players)
        {
            mPlayerProcessor->players[1]->GoLeft();
        }
        else {
            if(has2Players){
                mPlayerProcessor->players[1]->isNotRunning();
            }

        }
        if (state[SDL_SCANCODE_UP] && has2Players) {
            mPlayerProcessor->players[1]->Jump();
        }
    }

}
