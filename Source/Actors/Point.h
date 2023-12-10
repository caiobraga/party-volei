//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"
#include <string>


class Point : public Actor
{
public:
    explicit Point(Game* game, const std::string &texturePath, int mlevelWidth, int mlevelHeight);
    void ReSpawnPointActor();
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    int mlevelWidth;
    int mlevelHeight;
    //class DrawAnimatedComponent* mDrawComponent;
    class DrawSpriteComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;




};
