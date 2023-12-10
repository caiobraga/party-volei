// Ball.cpp
#include "Ball.h"
#include <iostream>
#include "../Game.h"

Ball::Ball(Game* game, float speed)
        : Actor(game), mSpeed(speed)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 8.0f, 0.10f);
    mRigidBodyComponent->SetVelocity(Vector2(this->mSpeed, this->mSpeed));

    mRigidBodyComponent->SetAcceleration(Vector2(speed, speed));
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Ball); // Adjust dimensions as needed
    //mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/ball.png", 32, 32);
    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/ball.png", "../Assets/Sprites/ball.json");
    mDrawComponent->AddAnimation("idle", {0, 1,2,3});
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);


}

void Ball::OnUpdate(float deltaTime)
{
    // Move the ball based on its velocity
    SetPosition(GetPosition() + mRigidBodyComponent->GetVelocity() * deltaTime);

    // Bounce off the walls
    /*if (GetPosition().x < 0 || GetPosition().x > GetGame()->GetWindowWidth())
    {
        mRigidBodyComponent->SetVelocity(Vector2(-mRigidBodyComponent->GetVelocity().x, mRigidBodyComponent->GetVelocity().y));
    }

    if (GetPosition().y < 0 || GetPosition().y > GetGame()->GetWindowHeight())
    {
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, -mRigidBodyComponent->GetVelocity().y));
    }*/

}
void Ball::Freeze()
{
    mIsFrozen = true;
    mRigidBodyComponent->SetVelocity(Vector2(0,0));  // Stop the ball
}

void Ball::Unfreeze()
{
    mIsFrozen = false;
    // Set the velocity to the original speed or any desired velocity
    mRigidBodyComponent->SetVelocity(Vector2(mSpeed, mSpeed));
}

bool Ball::IsFrozen() const
{
    return mIsFrozen;
}

void Ball::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
{
    // Handle collisions if needed
    for (auto& i : responses) {

        auto target = i.second.target;

        if (target->GetLayer() == ColliderLayer::Point){
            std::cout << "Point" << std::endl;
            target->DetectCollision(this->mRigidBodyComponent);
        }

        if (target->GetLayer() == ColliderLayer::Blocks && i.first == CollisionSide::Down) {
             //std::cout <<  this->GetPosition().y << " " << this->GetGame()->GetWindowHeight() << std::endl;

                GetGame()->ScorePoint(this->GetPosition());
                GetGame()->restartLevel();



        }

        if (target->GetLayer() == ColliderLayer::Blocks && i.first == CollisionSide::Left) {
            // std::cout <<  "colisão left" << std::endl;
            auto vel = mRigidBodyComponent->GetVelocity();
            mRigidBodyComponent->SetVelocity(Vector2(50, vel.y));
        }

        if (target->GetLayer() == ColliderLayer::Blocks && i.first == CollisionSide::Right) {
            //std::cout <<  "colisão rigth" << std::endl;
            auto vel = mRigidBodyComponent->GetVelocity();
            mRigidBodyComponent->SetVelocity(Vector2(-50, vel.y));        }




    }
}