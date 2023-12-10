//
// Created by Lucas N. Ferreira on 08/12/23.
//

#include "DrawTextComponent.h"
#include "../../Font.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawTextComponent::DrawTextComponent(class Actor* owner, const std::string &text, Font *font, int width, int height, int size, int drawOrder)
        :DrawComponent(owner, drawOrder)
        ,mWidth(width)
        ,mHeight(height)
        ,mSize(size)
        ,mFont(font)
 {
    mtext = text.c_str();
    mTextTexture = mFont->RenderText(owner->GetGame()->GetRenderer(), text, Vector3(1.0, 1.0, 1.0), size, 500);
}

DrawTextComponent::~DrawTextComponent()
{
    SDL_DestroyTexture(mTextTexture);
}

void DrawTextComponent::SetText(const std::string &text)
{
    SDL_DestroyTexture(mTextTexture);
    mTextTexture = mFont->RenderText(mOwner->GetGame()->GetRenderer(), text, Vector3(1.0, 1.0, 1.0), mSize, 500);
    /*if (i == selectedMenuOption) {
        // Highlight the selected option (you can change the color or style here)
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Yellow color
        SDL_RenderFillRect(renderer, &textRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset to default color
    }*/

}



void DrawTextComponent::Draw(SDL_Renderer *renderer)
{
    Vector2 pos = mOwner->GetPosition();
    int textWidth, textHeight;
    SDL_QueryTexture(mTextTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect backgroundRect = {
            static_cast<int>(pos.x - textWidth / 2.0f),
            static_cast<int>(pos.y - textHeight / 2.0f),
            textWidth,
            textHeight
    };

    if (mBackgroundColor.a > 0) {
        SDL_SetRenderDrawColor(renderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
        SDL_RenderFillRect(renderer, &backgroundRect);

        SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255);
     }

    SDL_Rect renderQuad = {
            static_cast<int>(pos.x - mWidth / 2.0f),
            static_cast<int>(pos.y - mHeight / 2.0f),
            mWidth,
            mHeight
    };

    SDL_RenderCopy(renderer, mTextTexture, nullptr, &renderQuad);
}