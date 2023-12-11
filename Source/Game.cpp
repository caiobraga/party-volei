// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include "SDL_image.h"
#include "Random.h"
#include "Game.h"
#include "Actors/Actor.h"
#include "Actors/Mario.h"
#include "Actors/Block.h"
#include "Actors/Goomba.h"
#include "Actors/Spawner.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "InputProcess.h"
#include "./Actors/Ball.h"
#include "Timer.h"
#include "Music.h"
#include "MusicManager.h"
#include "Scenes/Menu.h"
#include "Scenes/Overworld.h"
 #include "Scenes/Single.h"
#include <chrono>
#include <thread>
#include <SDL2/SDL_mixer.h>

using namespace std;
using namespace chrono;

const int LEVEL_WIDTH = 213;
const int LEVEL_HEIGHT = 14;
const int TILE_SIZE = 32;
const float SPAWN_DISTANCE = 600.0f;
const int SCORE_LIMIT = 5;

Game::Game(int windowWidth, int windowHeight)
    : mWindow(nullptr)
    , mRenderer(nullptr)
    , mTicksCount(0)
    , mIsRunning(true)
    , mUpdatingActors(false)
    , mWindowWidth(windowWidth)
    , mWindowHeight(windowHeight)
    , mProcessInput(nullptr)
    , mCamera(mRenderer)
    {
        mBall = nullptr;


    }

bool Game::Initialize()
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("P4: Super Mario Bros", 0, 0, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF initialization error: " << TTF_GetError() << std::endl;
        // Handle error
    }

    TTF_Font* font = TTF_OpenFont("../Arial.ttf", 20);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        // Handle error
    }


    Random::Init();

    mTicksCount = SDL_GetTicks();

  //  mMenuController->DisplayMenu();

    // Init all game actors
    mplayerProcessor = PlayerProcessor();
    mProcessInput = InputProcess(&mplayerProcessor);
    mGameState = GameState::Menu;
    mGameScene = GameScene::Menu;
    InitializeActors();

    //restartLevel();
   // LoadLevel("../Assets/Levels/Level1.txt", LEVEL_WIDTH, LEVEL_HEIGHT);
    //mMusicManaget.mMusic.PlayMusic(Music::mMENU1);

    return true;
}

void Game::StartNewGame(){
    InitializeActors();

    restartLevel();
}

void Game::InitializeActors() {
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 2.1 (~1 linha): Crie um objeto do tipo Mario e armazene-o na variável membro mMario.
    switch (mGameScene)
    {
        case GameScene::Menu:
            mScene = new Menu(this);
            mScene->Load();
            break;

        case GameScene::Overworld:
            mScene = new Overworld(this);
            mScene->Load();
            break;

        case GameScene::single:
            mScene = new Single(this);
            mScene->Load();
            break;
        default:
            break;
    }


}

void Game::SetScene(GameScene gameState)
{
    // Stop all sounds
    //mAudio->StopAllSounds();

    // Handle scene transition
    std::cout << "change scene "<< std::endl;
    UnloadActors();
    mGameScene = gameState;

    InitializeActors();
}

void Game::restartLevel() {

    if (mBall == nullptr ) {
        //delete mBall;
        mBall = new Ball(this, 40);
    }

    mBall->SetPosition(Vector2(GetWindowWidth() / 2, GetWindowHeight() / 2 - 150));

    mBall->Freeze();
    if(mplayerProcessor.players.size() > 1){
        if(mplayerProcessor.GetScore(1) >=5 || mplayerProcessor.GetScore(2) >= 5){
            SetGameState(GameState::Finishing);
            if(mplayerProcessor.GetScore(1) >=5){
                finishGame(mplayerProcessor.players[0]);
            }else{
                finishGame(mplayerProcessor.players[1]);
            }
        }else{




            mBall->Unfreeze();
            mBall->GetRigidBody()->SetVelocity(Random().GetVector(Vector2(-80, -10), Vector2(80, -80)));
        }
    }else{

        mBall->Unfreeze();
        if(mplayerProcessor.GetLives(mplayerProcessor.players[0]) <= 0){
            SetGameState(GameState::Finishing);
            finishGame(mplayerProcessor.players[0]);
        }




    }


}

void Game::LoadLevel(const std::string& levelPath, const int width, const int height)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 3.1 (~12 linhas): Leia o arquivo texto `levelPath` com `height` linhas e `width` colunas
    //  para carregar uma fase do jogo. Uma fase é representada por um grid com largura `width`
    //  e altura `height`, onde cada célula tem tamanho 32x32. Para cara caractere entre `A` e `I`, crie um
    //  objeto do tipo `Block` utilizando a textura correspondente. Para cara caractere `Y`, crie um
    //  um objeto do tipo `Spawner` utilizando a distância SPAWN_DISTANCE como parâmetro de criação.
    std::ifstream file(levelPath);

    if (!file) {
        std::cerr << "Erro Abrir Arquivo: " << levelPath << std::endl;
        return;
    }


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char c;
            file >> c;
          //  std::cout << "create new actor" << i << " " << j << std::endl;

            if (c >= 'A' && c <= 'R') {
                std::string path = "../Assets/Sprites/Blocks/Block";
                path+=c;
                path += ".png";

                if(c == 'D' || c == 'M' || c == 'N'){
                    new Block(this, path, true, c);
                }else{
                    new Block(this, path, false);
                }

                int y = mWindowHeight - 14 * 32;
                mActors.back()->SetPosition(Vector2(32 * j, 32 * i) + Vector2(0,y));
            }else if (c == 'Y')
            {
                int y = mWindowHeight - 14 * 32;
                new Spawner(this, SPAWN_DISTANCE);
                mActors.back()->SetPosition(Vector2(32 * (j), 32 * i) + Vector2(0, y));
            }
        }
    }
}

void Game::ScorePoint(const Vector2& position) {
    int screenWidth = GetWindowWidth();
    int screenHeight = GetWindowHeight();

    // Check if the ball's position is on the left or right side of the screen
    if(mplayerProcessor.players.size() > 1){
        if (position.x <= screenWidth / 2) {
            // If the ball is on the left side, give a point to Player 2 (assuming Player 2 is on the right)
            mplayerProcessor.AddScore(2);
        } else {
            // If the ball is on the right side, give a point to Player 1 (assuming Player 1 is on the left)
            mplayerProcessor.AddScore(1);
        }

        // Add additional conditions if needed for top or bottom sides of the screen
        // Example: if (position.y <= 0) for the top side

        // Restart the level or handle game state as needed
        // Restart the level if one of the players reaches a certain score limit
        if (mplayerProcessor.GetScore(1) >= SCORE_LIMIT || mplayerProcessor.GetScore(2) >= SCORE_LIMIT) {
            // Set the game state to restarting or handle game end logic

            // Perform actions like restarting the level, displaying a score screen, etc.
            restartLevel();
        }
    }else{
        mplayerProcessor.DecreaseLife(mplayerProcessor.players[0]);

    }



}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
       // Music().PlayMusic(Music::mOVERWORLD);
    }
}

void Game::ProcessInput()
{
    SDL_Event event;

    mProcessInput.Process(SDL_GetKeyboardState(nullptr));

            while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors)
    {
        //actor->ProcessInput(state);
    }

    mScene->ProcessInput(state);
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));


    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    if (mGameState == GameState::Finishing) {
        mBall->Freeze();
        static float elapsedTime = 0.0f;
        elapsedTime += deltaTime; // deltaTime is the time passed since the last frame

        const float zoomDuration = 3.0f; // Duration for zoom effect
        const float maxZoom = 2.0f; // Maximum zoom level

        // Get the winning player's position (modify this part to fit your logic)
        Mario* winningPlayer = nullptr;
        if(mplayerProcessor.players.size() > 1){
            if(mplayerProcessor.GetScore(1) >=5 || mplayerProcessor.GetScore(2) >= 5){
                SetGameState(GameState::Finishing);
                if(mplayerProcessor.GetScore(1) >=5){
                    winningPlayer = mplayerProcessor.players[0];
                }else{
                    winningPlayer = mplayerProcessor.players[1];
                }
            }
        }else{
            winningPlayer = mplayerProcessor.players[0];
            mBall->Freeze();
            static float elapsedTime = 0.0f;
            elapsedTime += deltaTime; // deltaTime is the time passed since the last frame

            const float zoomDuration = 3.0f; // Duration for zoom effect
            const float maxZoom = 2.0f; // Maximum zoom level






        }


        Vector2 winnerPosition = winningPlayer->GetPosition();

        // Calculate the zoom level based on elapsed time and duration
        float t = std::min(elapsedTime / zoomDuration, 1.0f); // Normalize time between 0 and 1
        float targetZoom = t * maxZoom; // Gradually increase zoom to maxZoom

        // Set camera position (optional - depends on your game logic)
       // mCamera.SetPosition(winnerPosition);

        // Set the zoom level for the camera
        mCamera.SetZoom(targetZoom);

        // Optionally, perform rendering using the updated camera parameters
        // RenderScene(); // Render your scene with the updated camera settings

        // Check if the zoom effect is completed
        if (t >= 1.0f) {
            // Zoom effect finished, reset variables or proceed to the next state
            mGameState = GameState::Restarting;
            mBall->Unfreeze();
            elapsedTime = 0.0f; // Reset elapsed time for future use
        }
    }


    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if (GetGameState() == GameState::Restarting) {
        mplayerProcessor.ZerarScore();
        mGameState = GameState::Normal;
    }

        if(GetGameState() == GameState::Normal){
            UpdateActors(deltaTime);
        }


    // Update all actors and pending actors
    // Update camera position
    //UpdateCamera();
}

void Game::UpdateCamera()
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (~4 linhas): Calcule a posição horizontal da câmera subtraindo a posição horizontal do
    //  jogador (i.e., do Mário) da metade da largura da janela. Isso fará com que a câmera fique sempre
    //  centralizada no jogador. No SMB, o jogador não pode voltar no nível, portanto, antes de atualizar
    //  a posição da câmera, verifique se a posição calculada é maior do que a posição anterior. Além disso,
    //  limite a posição para que a câmera fique entre 0 e o limite superior do nível. Para calcular o
    //  limite superior do nível, utilize as constantes `LEVEL_WIDTH` e `TILE_SIZE`.
    /*float camerapos =  mMario->GetPosition().x - mWindowWidth / 2.0;
    if (camerapos > (LEVEL_WIDTH * TILE_SIZE - mWindowWidth - 32))
        camerapos = LEVEL_WIDTH * TILE_SIZE - mWindowWidth - 32;
    else if (camerapos < GetCameraPos().x)
        camerapos = GetCameraPos().x;
    else if (camerapos < 0)
        camerapos = 0;

    SetCameraPos(Vector2(camerapos,GetCameraPos().y));*/

}

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent* collider)
{
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent* collider)
{
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput()
{


    // Display scores on the screen


    // Clear back buffer


    SDL_RenderClear(mRenderer);
    if(GetGameState() == GameState::Finishing){
        if(mplayerProcessor.players.size() > 1){
            if(mplayerProcessor.GetScore(1) >=5){
                renderFinishText(mplayerProcessor.players[0]);
            }else{
                renderFinishText(mplayerProcessor.players[1]);
            }
        }

    }

    /*if(GetGameState() == GameState::Normal){
        RenderScores(mRenderer);
    }*/

    if(GetGameState() == GameState::Menu){
       // RenderMenuOptions(mRenderer);
       // HandleMenuInput();
    }

    mScene->GenerateOutput();


   // DisplayScores();
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);

    for (auto drawable : mDrawables)
    {
        if (drawable->IsVisible())
        {
            drawable->Draw(mRenderer);
        }
    }




    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);


}

SDL_Texture* Game::LoadTexture(const std::string& texturePath) {
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (~4 linhas): Utilize a função `IMG_Load` para carregar a imagem passada como parâmetro
    //  `texturePath`. Esse função retorna um ponteiro para `SDL_Surface*`. Retorne `nullptr` se a
    //  imagem não foi carregada com sucesso.
    SDL_Surface* img = IMG_Load(texturePath.c_str());
    if (!img) {
        return nullptr;
    }


    // TODO 1.2 (~4 linhas): Utilize a função `SDL_CreateTextureFromSurface` para criar uma textura a partir
    //  da imagem carregada anteriormente. Essa função retorna um ponteiro para `SDL_Texture*`. Logo após criar
    //  a textura, utilize a função `SDL_FreeSurface` para liberar a imagem carregada. Se a textura foi carregada
    //  com sucesso, retorne o ponteiro para a textura. Caso contrário, retorne `nullptr`.


    auto tx = SDL_CreateTextureFromSurface(mRenderer, img);
    SDL_FreeSurface(img);
    if (tx)
    {
        return tx;
    }
    else
    {
        std::cout << "nao foi possivel criar a textura" << std::endl;
        return nullptr;
    }
}
void Game::UnloadActors()
{
    std::cout << "Unload actors" << std::endl;


    while (!mActors.empty())
    {
        delete mActors.back();
    }

   // delete mScene;


}


void Game::Shutdown()
{

    UnloadActors();
    delete mScene;
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
