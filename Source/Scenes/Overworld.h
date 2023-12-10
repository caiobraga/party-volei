//
// Created by Lucas N. Ferreira on 07/12/23.
//

#pragma once

#include "Scene.h"
#include "../Font.h"
#include <vector>
#include <string>

class Overworld : public Scene
{
public:
    Overworld(class Game* game);
    ~Overworld();

    void Load() override;
    void RenderScores();
    void GenerateOutput() override;
   // const Vector2& GetCameraPos() override;

    //class HUD *GetHUD() { return mHUD; }

private:
    void LoadData(const std::string& fileName);
    class Font *mMenuFont;
    //class Player *mPlayer;
    //class HUD *mHUD;
};
