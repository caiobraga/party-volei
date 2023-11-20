// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include "Math.h"
#include <string>
#include "PlayerProcessor.h"
#include "InputProcess.h"
#include "./Actors/Ball.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>


class Game
{
private:
    // ... Other class members ...

    enum class GameState {
        Normal,
        Restarting,
    };

    GameState mGameState;
public:
    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void InitializeActors();
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Draw functions
    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);

    // Collider functions
    void AddCollider(class AABBColliderComponent* collider);
    void RemoveCollider(class AABBColliderComponent* collider);
    std::vector<class AABBColliderComponent*>& GetColliders() { return mColliders; }

    Vector2& GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2& position) { mCameraPos = position; };

    // Window functions
    int GetWindowWidth() { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }


    SDL_Texture* LoadTexture(const std::string& texturePath);

    // Game-specific
    void restartLevel();
    void setWinner(Mario* player);
    void merkPoint(Mario* player);
    Ball* mBall;

    void SetGameState(GameState state) {
        mGameState = state;
    }

    GameState GetGameState() const {
        return mGameState;
    }
    void ScorePoint(const Vector2& position);
    // Font variables
    TTF_Font* font; // Load and manage your font using SDL_ttf
    SDL_Color textColor = { 0, 0, 0, 255 };

// Texture variables to render scores
    SDL_Texture* player1ScoreTexture = nullptr;
    SDL_Texture* player2ScoreTexture = nullptr;

    // Function to render player scores onto textures
    void RenderScores(SDL_Renderer* renderer) {
        // Assuming you have a PlayerProcessor class handling the player scores
        std::string player1ScoreStr = "Score: " + std::to_string(mplayerProcessor.GetScore(1));
        std::string player2ScoreStr = "Score: " + std::to_string(mplayerProcessor.GetScore(2));

        TTF_Font* font = TTF_OpenFont("../Arial.ttf", 20);
        SDL_Color textColor = { 255, 255, 255, 255 }; // White color for text

        if (!font) {
            std::cout << "Error loading font: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Surface* surface1 = TTF_RenderText_Solid(font, player1ScoreStr.c_str(), textColor);
        SDL_Surface* surface2 = TTF_RenderText_Solid(font, player2ScoreStr.c_str(), textColor);

        if (!surface1 || !surface2) {
            std::cout << "Error rendering text: " << TTF_GetError() << std::endl;
            TTF_CloseFont(font);
            return;
        }

        SDL_Texture* player1ScoreTexture = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Texture* player2ScoreTexture = SDL_CreateTextureFromSurface(renderer, surface2);

        if (!player1ScoreTexture || !player2ScoreTexture) {
            std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface1);
            SDL_FreeSurface(surface2);
            TTF_CloseFont(font);
            return;
        }

        SDL_Rect player1Rect = { 80, 100, surface1->w, surface1->h }; // Position for Player 1 score
        SDL_Rect player2Rect = { mWindowWidth - 80 - surface2->w, 100, surface2->w, surface2->h }; // Position for Player 2 score

        SDL_RenderCopy(renderer, player1ScoreTexture, nullptr, &player1Rect);
        SDL_RenderCopy(renderer, player2ScoreTexture, nullptr, &player2Rect);

        SDL_FreeSurface(surface1);
        SDL_FreeSurface(surface2);
        SDL_DestroyTexture(player1ScoreTexture);
        SDL_DestroyTexture(player2ScoreTexture);
        TTF_CloseFont(font);
    }

// Function to display scores on the screen
    void DisplayScores() {
        // Assuming the scores have been rendered onto textures (call RenderScores before DisplayScores)

        SDL_Rect player1Rect = { 100, 100, 200, 50 }; // Position and size for Player 1 score
        SDL_Rect player2Rect = { mWindowWidth /2 , mWindowHeight/2, 100, 100 }; // Position and size for Player 2 score


        // Render scores onto the screen
        SDL_RenderCopy(mRenderer, player1ScoreTexture, nullptr, &player1Rect);
        SDL_RenderCopy(mRenderer, player2ScoreTexture, nullptr, &player2Rect);
    }

// Call these functions within your game loop or when you need to update/render scores

// Example usage in GenerateOutput function



private:
    void ProcessInput();
    void UpdateGame();
    void UpdateCamera();
    void GenerateOutput();
    int mWindowWidth;

    // Game-specific
    void LoadLevel(const std::string& texturePath, int width, int height);

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // All the collision components
    std::vector<class AABBColliderComponent*> mColliders;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    // Window properties
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    Vector2 mCameraPos;

    // Game-specific
    PlayerProcessor mplayerProcessor;
    InputProcess mProcessInput;


};
