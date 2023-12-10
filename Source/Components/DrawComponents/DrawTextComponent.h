//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include "DrawComponent.h"
#include <string>
#include "../../Font.h"

class DrawTextComponent : public DrawComponent
{
public:
    // (Lower draw order corresponds with further back)
    const char *mtext;
    DrawTextComponent(class Actor* owner, const std::string &text, class Font *font, int width, int height, int fontSize = 24, int drawOrder = 100);

    ~DrawTextComponent();

    void SetText(const std::string &text);
    SDL_Surface* GetSurface(){
        return mFont->GetSurface() ;
    }
    void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
        mBackgroundColor = { r, g, b, 255 }; // Set the background color with alpha 255 (fully opaque)
    }



    void Draw(SDL_Renderer* renderer) override;


protected:
    // Map of textures loaded
    SDL_Surface* mTextSurface;
    SDL_Texture* mTextTexture;
    class Font *mFont;

    int mSize;
    int mWidth;
    int mHeight;
    SDL_Color mBackgroundColor;

};
