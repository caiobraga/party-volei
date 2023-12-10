//
// Created by Lucas N. Ferreira on 07/12/23.
//

#pragma once

#include "Scene.h"
#include "../Font.h"
#include "../Actors/Point.h"
#include <vector>
#include <string>

class Single : public Scene
{
public:
    Single(class Game* game);
    ~Single();

    void Load() override;
    void RenderScores();
    void SpawnPointActor();
    void GenerateOutput() override;
   // const Vector2& GetCameraPos() override;

    //class HUD *GetHUD() { return mHUD; }

private:
    void LoadData(const std::string& fileName);
    class Font *mMenuFont;
    Point* mPointActor; // Keep track of the point actor
    bool mPointSpawned; // Flag to track if the point is spawned
    //class Player *mPlayer;
    //class HUD *mHUD;
};
