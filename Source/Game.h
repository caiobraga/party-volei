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
#include "Camera.h"
#include "Music.h"
#include "MusicManager.h"

class Game
{
private:
    // ... Other class members ...


public:
    enum class GameState {
        Menu,
        Normal,
        Restarting,
        Finishing
    };

    enum class GameScene
    {
        Menu,
        Overworld,
        single
    };
    GameScene mGameScene;
    class Scene *mScene;

    GameState mGameState;


    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void StartNewGame();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }
    void SetScene(GameScene gameState);

    SDL_Renderer* GetRenderer() { return mRenderer; }


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

    Vector2 GetCameraPos() { return mCamera.GetPosition(); };
    void SetCameraPos(const Vector2& position) { mCamera.SetPosition(position); };

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

    void UnloadActors();

    void renderFinishText(Mario* winningPlayer){
        Vector2 winnerPosition = winningPlayer->GetPosition();
        std::string winText = "You Win!";
        Vector2 textPosition = Vector2(winnerPosition.x,  winnerPosition.y - 64); // Position for the text

        TTF_Font* font = TTF_OpenFont("../Arial.ttf", 20);
        SDL_Color textColor = { 255, 255, 255, 255 }; // White color for text

        if (!font) {
            std::cout << "Error loading font: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Surface* surface1 = TTF_RenderText_Solid(font, winText.c_str(), textColor);

        if (!surface1  ) {
            std::cout << "Error rendering text: " << TTF_GetError() << std::endl;
            TTF_CloseFont(font);
            return;
        }

        SDL_Texture* player1ScoreTexture = SDL_CreateTextureFromSurface(mRenderer, surface1);

        if (!player1ScoreTexture ) {
            std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface1);
            TTF_CloseFont(font);
            return;
        }
        int x = textPosition.x;
        int y = textPosition.y;
        SDL_Rect player1Rect = {  x, y , surface1->w, surface1->h }; // Position for Player 1 score

        SDL_RenderCopy(mRenderer, player1ScoreTexture, nullptr, &player1Rect);

        SDL_FreeSurface(surface1);
        SDL_DestroyTexture(player1ScoreTexture);
        TTF_CloseFont(font);
    }

    void finishGame(Mario* winningPlayer) {
        SetGameState(GameState::Finishing);
        mBall->Freeze();

        const float zoomSpeed = 0.01f; // Adjust the zoom speed as needed
        const float maxZoom = 2.0f; // Maximum zoom level

        Vector2 winnerPosition = winningPlayer->GetPosition();


        float zoomLevel = 2.0f; // Change this to zoom in or out


      //  mCamera.ZoomTo(Vector2(0,0), 3, 12);






    }

    void ScorePoint(const Vector2& position);
    // Font variables
    TTF_Font* font; // Load and manage your font using SDL_ttf
    SDL_Color textColor = { 0, 0, 0, 255 };

// Texture variables to render scores
    SDL_Texture* player1ScoreTexture = nullptr;
    SDL_Texture* player2ScoreTexture = nullptr;

    // Function to render player scores onto textures
    int selectedMenuOption = 0;
    std::vector<std::string> menuOptions = {"Start", };

    void HandleMenuSelection(int selectedOption){
        std::cout << selectedOption ;
        if(selectedOption == 0){
            SetGameState(GameState::Normal);
            StartNewGame();

        }
    }

    void RenderMenuOptions(SDL_Renderer* renderer) {
        TTF_Font* font = TTF_OpenFont("../Arial.ttf", 20);
        SDL_Color textColor = { 255, 255, 255, 255 }; // White color for text

        if (!font) {
            std::cout << "Error loading font: " << TTF_GetError() << std::endl;
            return;
        }

        int yOffset = mWindowHeight / 2; // Initial Y position for the first menu option

        for (size_t i = 0; i < menuOptions.size(); ++i) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, menuOptions[i].c_str(), textColor);
            if (!surface) {
                std::cout << "Error rendering text: " << TTF_GetError() << std::endl;
                TTF_CloseFont(font);
                return;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!texture) {
                std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
                SDL_FreeSurface(surface);
                TTF_CloseFont(font);
                return;
            }

            int textWidth = surface->w;
            int textHeight = surface->h;

            SDL_Rect textRect = { (mWindowWidth - textWidth) / 2, yOffset, textWidth, textHeight };

            if (i == selectedMenuOption) {
                // Highlight the selected option (you can change the color or style here)
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Yellow color
                SDL_RenderFillRect(renderer, &textRect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset to default color
            }

            yOffset += textHeight + 20; // Increase Y position for the next menu option

            SDL_RenderCopy(renderer, texture, nullptr, &textRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        TTF_CloseFont(font);
    }

// Assume this function handles user input (e.g., in your game loop)
   /* void HandleMenuInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedMenuOption = (selectedMenuOption - 1 + menuOptions.size()) % menuOptions.size();
                        break;
                    case SDLK_DOWN:
                        selectedMenuOption = (selectedMenuOption + 1) % menuOptions.size();
                        break;
                    case SDLK_RETURN:
                        // Handle selection based on selectedMenuOption
                        HandleMenuSelection(selectedMenuOption);
                        break;
                    default:
                        break;
                }
            }
        }
    }*/




// Call these functions within your game loop or when you need to update/render scores

// Example usage in GenerateOutput function



    MusicManager mMusicManaget = MusicManager();

    // Window properties
    int mWindowHeight;
    int mWindowWidth;

    // Game-specific
    void LoadLevel(const std::string& texturePath, int width, int height);

// Game-specific
PlayerProcessor mplayerProcessor;
// All the actors in the game
std::vector<class Actor*> mActors;
private:
    void ProcessInput();
    void UpdateGame();
    void UpdateCamera();
    void GenerateOutput();


    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // All the collision components
    std::vector<class AABBColliderComponent*> mColliders;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;



    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    Camera mCamera;

    InputProcess mProcessInput;


};