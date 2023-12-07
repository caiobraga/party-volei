// Ball.h
#pragma once

#include "Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "./Ball.h"


class Ball : public Actor
{
public:
    explicit Ball(Game* game, float speed);

    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    RigidBodyComponent* GetRigidBody(){
        return mRigidBodyComponent;
    }
    void Freeze();
    void Unfreeze();
    bool IsFrozen() const;
    ~Ball() {
        if (mRigidBodyComponent) {
            delete mRigidBodyComponent;
            mRigidBodyComponent = nullptr;
        }
        if (mColliderComponent) {
            delete mColliderComponent;
            mColliderComponent = nullptr;
        }
        if (mDrawComponent) {
            delete mDrawComponent;
            mDrawComponent = nullptr;
        }
    }

private:
    bool mIsFrozen;
    float mSpeed;
    class RigidBodyComponent* mRigidBodyComponent;
    class AABBColliderComponent* mColliderComponent;
    //class DrawSpriteComponent* mDrawComponent;
    class DrawAnimatedComponent* mDrawComponent;
};