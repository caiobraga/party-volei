//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

class Mario : public Actor
{
public:
    explicit Mario(Scene* scene,
                  float forwardSpeed = 2500.0f,
                  float jumpSpeed = -750.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    void Kill() override;
    void GoLeft();
    void GoRigth();
    void Jump();
    void Atack();
    void isNotRunning();
    RigidBodyComponent * GetRigidBodyComponent(){
        return mRigidBodyComponent;
    }
    int GetPlayerVelocity(){
        return mForwardSpeed;
    }


private:
    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    bool mIsRunning;
    bool mIsDead;
    bool mIsOnGround;
    bool isLookingRigth;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    //class DrawSpriteComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;


};