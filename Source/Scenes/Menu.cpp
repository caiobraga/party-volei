//
// Created by Lucas N. Ferreira on 07/12/23.
//

#include "Menu.h"
#include "../Game.h"
#include "../Font.h"
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawTextComponent.h"

Menu::Menu(Game* game)
     :Scene(game)
     ,mMenuFont(nullptr)
{
    mMenuFont = new Font();
    mMenuFont->Load("../Assets/Fonts/Arial.ttf");
}

std::vector<std::string> menuOptions = { "Multiplayer", "Training Mode" };

const int LEVEL_WIDTH = 20;
const int LEVEL_HEIGHT = 14;

void Menu::Load()
{
    // Background and Title
    //auto *background = new Actor(this);
    //new DrawSpriteComponent(background, "../Assets/Sprites/Menu/Background.png", 512, 288);
    //new DrawSpriteComponent(background, "../Assets/Sprites/Menu/Title.png", 348, 172);

    // Press Space label

    this->mGame->LoadLevel("../Assets/Levels/Level1.txt", LEVEL_WIDTH,LEVEL_HEIGHT);

    for (size_t i = 0; i < menuOptions.size(); ++i) {


        auto *pressSpace = new Actor(this);
        pressSpace->SetPosition(Vector2(mGame->GetWindowWidth()/2.0f, mGame->GetWindowHeight()/ 2.0f  + (64 * i) ));
        auto mtext =  new DrawTextComponent(pressSpace, menuOptions[i], mMenuFont, 120, 28, 20);

        if (i == selectedMenuOption) {
            mtext->SetBackgroundColor(255, 215, 0); // Set yellow color



            //SDL_SetRenderDrawColor(mGame->GetRenderer(), 0, 0, 0, 255); // Reset to default color
        }

    }
    this->mGame->mMusicManaget.mMusic.PlayMusic(Music::mMENU1);


    std::cout << "Load menu" << std::endl;

}

void Menu::UpdateText(){
    if(this->GetGame()->mGameState != Game::GameState::Menu) return;
    this->mGame->UnloadActors();
    this->mGame->LoadLevel("../Assets/Levels/Level1.txt", LEVEL_WIDTH,LEVEL_HEIGHT);


    for (size_t i = 0; i < menuOptions.size(); ++i) {


        auto *pressSpace = new Actor(this);
        pressSpace->SetPosition(Vector2(mGame->GetWindowWidth()/2.0f, mGame->GetWindowHeight()/ 2.0f  + (64 * i) ));
        auto mtext =  new DrawTextComponent(pressSpace, menuOptions[i], mMenuFont, 120, 28, 20);

        if (i == selectedMenuOption) {
             mtext->SetBackgroundColor(255, 215, 0); // Set yellow color
        }


    }
}

void Menu::GenerateOutput(){



  //  SDL_SetRenderDrawColor(this->GetGame()->GetRenderer(), 255, 215, 0, 255); // Yellow color



}

void Menu::ProcessInput(const Uint8 *keyState) {
    static bool spacePressed = false;
    static bool upPressed = false;
    static bool downPressed = false;
    static bool returnPressed = false;

    if (keyState[SDL_SCANCODE_SPACE]) {
        if (!spacePressed) {
            spacePressed = true;
            std::cout << "Space" << std::endl;
            HandleMenuSelection(selectedMenuOption);
        }
    } else {
        spacePressed = false;
    }

    if (keyState[SDL_SCANCODE_UP]) {
        if (!upPressed) {
            upPressed = true;
            std::cout << "UP" << std::endl;

            selectedMenuOption = (selectedMenuOption - 1 + menuOptions.size()) % menuOptions.size();
            UpdateText();
            std::cout << "Selected: " << selectedMenuOption <<  std::endl;
        }
    } else {
        upPressed = false;
    }

    if (keyState[SDL_SCANCODE_DOWN]) {
        if (!downPressed) {
            downPressed = true;
            std::cout << "Down" << std::endl;

            selectedMenuOption = (selectedMenuOption + 1) % menuOptions.size();
            UpdateText();
            std::cout << "Selected: " << selectedMenuOption <<  std::endl;
        }
    } else {
        downPressed = false;
    }

    if (keyState[SDL_SCANCODE_RETURN]) {
        if (!returnPressed) {
            returnPressed = true;
            std::cout << "Return" << std::endl;
            HandleMenuSelection(selectedMenuOption);
        }
    } else {
        returnPressed = false;
    }
}
