//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Mario.h"
#include "../Game.h"
#include <iostream>
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "./Ball.h"

Mario::Mario(Scene* scene,
             const float forwardSpeed,
             const float jumpSpeed
             )
        : Actor(scene)
        , mIsRunning(false)
        , mIsDead(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
{


    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);
    /*mDrawComponent = new DrawSpriteComponent(this, "D:\\Downloads\\p4-smb-template\\Assets\\Sprites\\Mario\\Idle.png", 32, 32);*/
    mColliderComponent = new AABBColliderComponent(this,0, 0, 32, 32, ColliderLayer::Player);
    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Mario/MarioSprite.png", "../Assets/Sprites/Mario/MarioSprite.json");
    mDrawComponent->AddAnimation("dead", { 0 });
    mDrawComponent->AddAnimation("idle", { 1 });
    mDrawComponent->AddAnimation("jump", { 2 });
    mDrawComponent->AddAnimation("run", { 3,4,5 });
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);

}
void Mario::GoLeft() {
    mRigidBodyComponent->ApplyForce(Vector2(-mForwardSpeed,0));
    mRotation = Math::Pi;
    mIsRunning = true;
    isLookingRigth = false;
}

void Mario::GoRigth() {
    mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed,0));
    mRotation = 0;
    mIsRunning = true;
    isLookingRigth = true;
}

void Mario::Jump() {
    if (mIsOnGround == true ) {
        auto vel = mRigidBodyComponent->GetVelocity();
        mRigidBodyComponent->SetVelocity(Vector2(vel.x, mJumpSpeed));
        mIsOnGround = false;
    }
}

void Mario::isNotRunning(){
    mIsRunning = false;
}

void Mario::OnProcessInput(const uint8_t* state)
{
        /*float pos =  GetPosition().x;
        if (pos >= this->mGame->GetWindowWidth() /2 -32){
            auto vel = mRigidBodyComponent->GetVelocity();
            this->mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed,0));
        }

        if (state[SDL_SCANCODE_D])
        {
            mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed,0));
            mRotation = 0;
            mIsRunning = true;
        }
        else if (state[SDL_SCANCODE_A])
        {
            mRigidBodyComponent->ApplyForce(Vector2(-mForwardSpeed,0));
            mRotation = Math::Pi;
            mIsRunning = true;
        }
        else {
            mIsRunning = false;
        }
        if (mIsOnGround == true && state[SDL_SCANCODE_W]) {
            auto vel = mRigidBodyComponent->GetVelocity();
            mRigidBodyComponent->SetVelocity(Vector2(vel.x, mJumpSpeed));
            mIsOnGround = false;
        }*/

    }




void Mario::OnUpdate(float deltaTime)
{
    auto pos = GetPosition();
    /*if (pos.x < GetGame()->GetCameraPos().x) {
        pos = GetGame()->GetCameraPos();
        SetPosition(pos);
    }*/

    if (pos.y > GetGame()->GetWindowHeight()) {
        Kill();
    }

    ManageAnimations();
}

void Mario::ManageAnimations()
{

    if (mIsDead) mDrawComponent->SetAnimation("dead");

    else {
        if (mIsOnGround)
        {
            if (mIsRunning)
                mDrawComponent->SetAnimation("run");
            else
                mDrawComponent->SetAnimation("idle");
        }
        else
            mDrawComponent->SetAnimation("jump");
    }

}

void Mario::Kill()
{
    mIsDead = true;
    mDrawComponent->SetAnimation("dead");
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);

}

void Mario::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) {

    for (auto &i: responses) {

        auto target = i.second.target;

        if (this->GetGame()->mBall != nullptr) {
            if (target->GetLayer() == ColliderLayer::Ball) {
                if (target->GetLayer() == ColliderLayer::Ball) {
                    this->GetGame()->mMusicManaget.PlayHit();
                }
            }


            if (this->GetGame()->mBall != nullptr)
                if (target->GetLayer() == ColliderLayer::Ball && i.first == CollisionSide::Left) {

                    this->GetGame()->mBall->GetRigidBody()->SetVelocity(
                            Vector2(mJumpSpeed / 1.5f, this->GetGame()->mBall->GetRigidBody()->GetVelocity().y));
                }
            if (this->GetGame()->mBall != nullptr)
                if (target->GetLayer() == ColliderLayer::Ball && i.first == CollisionSide::Right) {
                    if (this->GetGame()->mBall != nullptr)
                        this->GetGame()->mBall->GetRigidBody()->SetVelocity(
                                Vector2(-mJumpSpeed / 1.5f, this->GetGame()->mBall->GetRigidBody()->GetVelocity().y));
                }
            float directionKickSpeed = isLookingRigth ? -mJumpSpeed * 100 : mJumpSpeed * 100;
            if (this->GetGame()->mBall != nullptr)
                if (target->GetLayer() == ColliderLayer::Ball && i.first == CollisionSide::Top) {
                    if (this->GetGame()->mBall != nullptr)
                        this->GetGame()->mBall->GetRigidBody()->ApplyForce(
                                Vector2(directionKickSpeed, mJumpSpeed * 300));
                }
            if (this->GetGame()->mBall != nullptr)
                if (target->GetLayer() == ColliderLayer::Ball && i.first == CollisionSide::Down) {
                    if (this->GetGame()->mBall != nullptr)
                        this->GetGame()->mBall->GetRigidBody()->ApplyForce(
                                ((Vector2(directionKickSpeed, -mJumpSpeed * 300))));
                }


            if (target->GetLayer() == ColliderLayer::Blocks && i.first == CollisionSide::Down) {
                mIsOnGround = true;
            }
            if (target->GetLayer() == ColliderLayer::Enemy && i.first == CollisionSide::Down) {
                mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed / 1.5f));
                target->GetOwner()->Kill();
            }

            if (target->GetLayer() == ColliderLayer::Enemy &&
                (i.first == CollisionSide::Right || i.first == CollisionSide::Left) && mIsOnGround) {
                Kill();
            }
            if (target->GetLayer() == ColliderLayer::Enemy &&
                (i.first == CollisionSide::Right || i.first == CollisionSide::Left) && !mIsOnGround) {
                mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed / 1.5f));
                target->GetOwner()->Kill();
            }
        }
    }
}