#pragma once
#include <string>
#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include "Math.h"

class Font
{
public:
	Font();
	~Font();

	// Load/unload from a file
	bool Load(const std::string& fileName);
	void Unload();

	// Given string and this font, draw to a texture
	class SDL_Texture* RenderText(SDL_Renderer* renderer, const std::string& text, const Vector3& color = Color::White,
							         int pointSize = 30, unsigned wrapLength = 900);

    // Given string and this font, and its background draw to a texture
    class SDL_Texture* RenderText(SDL_Renderer* renderer, const std::string& text, const Vector3& color = Color::White, const Vector3& backgroundcolor = Color::White,
                                  int pointSize = 30, unsigned wrapLength = 900);

    SDL_Surface* GetSurface(){
        return mTextSurface;
    }

    TTF_Font* GetFont(int size) const {
        auto it = mFontData.find(size);
        if (it != mFontData.end()) {
            return it->second; // Return the font pointer if found
        }
        return nullptr; // Return nullptr if size not found
    }

private:
	// Map of point sizes to font data
    SDL_Surface* mTextSurface;
	std::unordered_map<int, TTF_Font*> mFontData;
};
