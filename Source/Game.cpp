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

const int LEVEL_WIDTH = 213;
const int LEVEL_HEIGHT = 14;
const int TILE_SIZE = 32;
const float SPAWN_DISTANCE = 600.0f;

Game::Game(int windowWidth, int windowHeight)
    :mWindow(nullptr)
    , mRenderer(nullptr)
    , mTicksCount(0)
    , mIsRunning(true)
    , mUpdatingActors(false)
    , mWindowWidth(windowWidth)
    , mWindowHeight(windowHeight)
{

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

    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    return true;
}

void Game::InitializeActors()
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 2.1 (~1 linha): Crie um objeto do tipo Mario e armazene-o na variável membro mMario.
    mMario = new Mario(this);
    // TODO 2.2 (~1 linha): Utilize a função LoadLevel para carregar o primeiro nível (Level1.txt) do jogo.
    //  Esse arquivo tem 14 linhas e 213 colunas.
    LoadLevel("../Assets/Levels/Level1.txt", 212, 14);
    //LoadLevel("..\\Assets\\Levels\\Level1.txt", 14, 213);
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

            if (c >= 'A' && c <= 'I') {
                std::string path = "../Assets/Sprites/Blocks/Block";
                path+=c;
                path += ".png";
                new Block(this, path);
                int y = mWindowHeight - 14 * 32;
            

                mActors.back()->SetPosition(Vector2(32 * (j), 32 * i) + Vector2(0, y));
            }

            if (c == 'Y') {
                int y = mWindowHeight - 14 * 32;
                new Spawner(this, SPAWN_DISTANCE);
                mActors.back()->SetPosition(Vector2(32 * (j), 32 * i) + Vector2(0, y));

            }
        }
    }

}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
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
        actor->ProcessInput(state);
    }
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera();
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
    float posX = this->GetMario()->GetPosition().x - mWindowWidth / 2.0;

    
    if (posX > (LEVEL_WIDTH * TILE_SIZE - mWindowWidth - 32))
        posX = LEVEL_WIDTH * TILE_SIZE - mWindowWidth - 32;
    else if (posX < GetCameraPos().x)
        posX = GetCameraPos().x;
    else if (posX < 0)
        posX = 0;

    this->SetCameraPos(Vector2(posX, this->GetCameraPos().y));
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
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

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
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (surface == nullptr) {
        return nullptr;
    }


    // TODO 1.2 (~4 linhas): Utilize a função `SDL_CreateTextureFromSurface` para criar uma textura a partir
    //  da imagem carregada anteriormente. Essa função retorna um ponteiro para `SDL_Texture*`. Logo após criar
    //  a textura, utilize a função `SDL_FreeSurface` para liberar a imagem carregada. Se a textura foi carregada
    //  com sucesso, retorne o ponteiro para a textura. Caso contrário, retorne `nullptr`.
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        return nullptr;
    }
    return texture;
}

void Game::Shutdown()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
