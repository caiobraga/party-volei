//
// Created by Lucas N. Ferreira on 07/12/23.
//

#pragma once

#include "Scene.h"
#include "../Game.h"

class Menu : public Scene
{
public:
    Menu(class Game* game);

    void Load() override;
    void ProcessInput(const Uint8* keyState) override;
    void GenerateOutput() override;
    void UpdateText();

    void HandleMenuSelection(int selectedOption){
        //std::cout << selectedOption ;
        if(selectedOption == 0){
            std::cout<< "Go to next scene, multiplayer" << std::endl;
            this->mGame->SetGameState(Game::GameState::Normal);
            //StartNewGame();
            mGame->SetScene(Game::GameScene::Overworld);

        }
        if(selectedOption == 1){
            std::cout<< "Go to next scene, single player" << std::endl;
            this->mGame->SetGameState(Game::GameState::Normal);
            //StartNewGame();
            mGame->SetScene(Game::GameScene::single);

        }
    }

    int selectedMenuOption = 0;


private:
    class Font *mMenuFont;
};
