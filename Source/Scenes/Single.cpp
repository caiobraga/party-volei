//
// Created by Lucas N. Ferreira on 07/12/23.
//

#include "Single.h"
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
 #include "../Game.h"
#include "../Actors/Mario.h"
#include "../Music.h"
#include "../Components/DrawComponents/DrawTextComponent.h"
#include "../Actors/Point.h"
#include "../Random.h"

Single::Single(Game* game)
          :Scene(game)

{
    mMenuFont = new Font();
    mMenuFont->Load("../Assets/Fonts/Arial.ttf");

}

Single::~Single()
{
    //delete mHUD;
    //delete mHUD;
}

const int LEVEL_WIDTH = 20;
const int LEVEL_HEIGHT = 14;

void Single::Load()
{

    this->mGame->LoadLevel("../Assets/Levels/Level1.txt", LEVEL_WIDTH,LEVEL_HEIGHT);
    std::cout << "Load Overword Level" << std::endl;

    Mario* p1 =  new Mario(this);
    p1->SetPosition(Vector2(mGame->mWindowWidth / 2, mGame->mWindowHeight / 2 + 100));
    this->mGame->mplayerProcessor.AddPlayer(p1);

    this->mGame->mplayerProcessor.SetLives(p1, 5);
    //Mario* p2 =  new Mario(this);
   // p2->SetPosition(Vector2(432, 120));
   // this->mGame->mplayerProcessor.AddPlayer(p2);

    mGame->restartLevel();




    /* ScorePoint(Vector2(432, 120));
     ScorePoint(Vector2(432, 120));
     ScorePoint(Vector2(432, 120));
     ScorePoint(Vector2(432, 120));*/
    // ScorePoint(Vector2(432, 120));


    //mMario = new Mario(this);
    //mMario->SetPosition(Vector2(64, 120));




    this->mGame->mMusicManaget.mMusic.PlayMusic(Music::mMENU2);

    // Generate random coordinates within the screen dimensions
    int randomX =  Random().GetFloatRange(5, LEVEL_WIDTH - 5);   // Random X-coordinate
    int randomY = Random().GetFloatRange(5, LEVEL_HEIGHT - 5 );  // Random Y-coordinate

    if(randomX < LEVEL_WIDTH + 4 && randomX > LEVEL_WIDTH - 4 ){
        randomX = (randomX + randomY) % LEVEL_WIDTH;
    }


    // Create a point actor at the random coordinates
    std::string pointTexture = "../Assets/Sprites/Blocks/BlockD.png";
    mPointActor  = new Point(this->mGame,  pointTexture, LEVEL_WIDTH, LEVEL_HEIGHT);
    // int y = mGame->mWindowHeight - 14 * 32;

    mPointActor->SetPosition(Vector2(randomX * 32, randomY * 32 ) );


    mPointSpawned = false; // Set the flag to false initially

}



void Single::GenerateOutput() {
    RenderScores();
    if(mGame->mplayerProcessor.GetLives(mGame->mplayerProcessor.players[0] ) == 0){
        mGame->restartLevel();
    }

}


void Single::RenderScores() {

    // Assuming you have a PlayerProcessor class handling the player scores
    /*std::string player1ScoreStr = "Score: " + std::to_string(mGame->mplayerProcessor.GetScore(1));
    std::string player2ScoreStr = "Score: " + std::to_string(mGame->mplayerProcessor.GetScore(2));

    auto *pressSpace = new Actor(this);
    pressSpace->SetPosition(Vector2(100, 100 ));
    auto mtext =  new DrawTextComponent(pressSpace, player1ScoreStr, mMenuFont, 120, 28, 20);

    auto *pressSpace2 = new Actor(this);
    pressSpace2->SetPosition(Vector2(mGame->mWindowWidth - 100,100 ));
    auto mtext2 =  new DrawTextComponent(pressSpace2, player2ScoreStr, mMenuFont, 120, 28, 20);


*/
    std::string player1ScoreStr = "Score: " + std::to_string(mGame->mplayerProcessor.GetScore(1));
    std::string player1LifeStr = "Lifes: " + std::to_string(mGame->mplayerProcessor.GetLives(mGame->mplayerProcessor.players[0]));


    TTF_Font* font = TTF_OpenFont("../Arial.ttf", 20);
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color for text

    if (!font) {
        std::cout << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* surface1 = TTF_RenderText_Solid(font, player1ScoreStr.c_str(), textColor);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, player1LifeStr.c_str(), textColor);

    if (!surface1 || !surface2) {
        std::cout << "Error rendering text: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* player1ScoreTexture = SDL_CreateTextureFromSurface(mGame->GetRenderer(), surface1);
    SDL_Texture* player1LifeTexture = SDL_CreateTextureFromSurface(mGame->GetRenderer(), surface2);

    if (!player1ScoreTexture || !player1LifeTexture) {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface1);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect player1Rect = { 80, 80, surface1->w, surface1->h }; // Position for Player 1 score
    SDL_Rect player1LifeRect = { mGame->mWindowWidth -  150, 80, surface2->w, surface2->h }; // Position for Player 1 score

    SDL_RenderCopy(mGame->GetRenderer(), player1ScoreTexture, nullptr, &player1Rect);
    SDL_RenderCopy(mGame->GetRenderer(), player1LifeTexture, nullptr, &player1LifeRect);

    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_DestroyTexture(player1ScoreTexture);
    SDL_DestroyTexture(player1LifeTexture);
     TTF_CloseFont(font);

}
/*
void Overworld::LoadData(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("Failed to load paths: %s", fileName.c_str());
    }

    int row = 0;

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);

        if(!line.empty())
        {
            auto tiles = CSVHelper::Split(line);

            if(tiles[0] == "Type") {
                continue;
            }

            int x = std::stoi(tiles[1]);
            int y = std::stoi(tiles[2]);
            int width = std::stoi(tiles[3]);
            int height = std::stoi(tiles[4]);

            if(tiles[0] == "Player")
            {
                mPlayer = new Player(this);
                mPlayer->SetPosition(Vector2(x + width/2.0f, y + height/2.0));
            }
            else if(tiles[0] == "Collider")
            {
                auto *wall = new Wall(this, width, height);
                wall->SetPosition(Vector2(x + width/2.0f, y + height/2.0));
            }
            else if(tiles[0] == "Bush")
            {
                auto *bush = new Bush(this);
                bush->SetPosition(Vector2(x + width/2.0f, y + height/2.0));
            }
            else if(tiles[0] == "Soldier")
            {
                auto *soldier = new Soldier(this, 10.0f);
                soldier->SetPosition(Vector2(x + width/2.0f, y + height/2.0));
            }
        }
    }
}*/
/*
const Vector2& Overworld::GetCameraPos()
{
    return Vector2(0, 0);
}*/