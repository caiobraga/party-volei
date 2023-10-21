//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawAnimatedComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include <fstream>

DrawAnimatedComponent::DrawAnimatedComponent(class Actor* owner, const std::string &spriteSheetPath, const std::string &spriteSheetData, int drawOrder)
        :DrawSpriteComponent(owner, spriteSheetPath, 0, 0, drawOrder)
{
    LoadSpriteSheet(spriteSheetPath, spriteSheetData);
}

DrawAnimatedComponent::~DrawAnimatedComponent()
{
    for (const auto& rect : mSpriteSheetData)
    {
        delete rect;
    }
    mSpriteSheetData.clear();
}

void DrawAnimatedComponent::LoadSpriteSheet(const std::string& texturePath, const std::string& dataPath)
{
    // Load sprite sheet texture
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);

    // Load sprite sheet data
    std::ifstream spriteSheetFile(dataPath);
    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    SDL_Rect* rect = nullptr;
    for(const auto& frame : spriteSheetData["frames"]) {

        int x = frame["frame"]["x"].get<int>();
        int y = frame["frame"]["y"].get<int>();
        int w = frame["frame"]["w"].get<int>();
        int h = frame["frame"]["h"].get<int>();
        rect = new SDL_Rect({x, y, w, h});

        mSpriteSheetData.emplace_back(rect);
    }
}

void DrawAnimatedComponent::Draw(SDL_Renderer *renderer)
{

    int id_sprite = int(mAnimTimer);
    auto currentAnimation = mAnimations[mAnimName];

    if (id_sprite >= currentAnimation.size()) {
        id_sprite = currentAnimation.size() - 1;
    }


    int id_frame = currentAnimation[id_sprite];



    int x = GetOwner()->GetPosition().x - GetOwner()->GetGame()->GetCameraPos().x;
    int y = GetOwner()->GetPosition().y - GetOwner()->GetGame()->GetCameraPos().y;

    SDL_Rect srcRect = *(mSpriteSheetData[id_frame]);
    SDL_Rect destRect = {x, y,  srcRect.w, srcRect.h };
    SDL_Point point = { x,y};


    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (GetOwner()->GetRotation() == 0) {
        flip = SDL_FLIP_NONE;
    }
    else {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, &srcRect, &destRect, 0.0, &point, flip);

}

void DrawAnimatedComponent::Update(float deltaTime)
{
    if (mIsPaused) return;

    mAnimTimer += deltaTime * mAnimFPS;
    int tamAnim = mAnimations[mAnimName].size();
    while (mAnimTimer >= tamAnim) {
        mAnimTimer -= tamAnim;
    }

}

void DrawAnimatedComponent::SetAnimation(const std::string& name)
{
    mAnimName = name;
    Update(0.0f);
}

void DrawAnimatedComponent::AddAnimation(const std::string& name, const std::vector<int>& spriteNums)
{
    mAnimations.emplace(name, spriteNums);
}
