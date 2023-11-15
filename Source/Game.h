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
