//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include <algorithm>
#include <random>

AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h, ColliderLayer layer, int updateOrder)
        :Component(owner, updateOrder)
        , mOffset(Vector2((float)dx, (float)dy))
        , mWidth(w)
        , mHeight(h)
        , mLayer(layer)
{
    mOwner->GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent()
{
    mOwner->GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const
{

    return mOwner->GetPosition() + mOffset;
}

Vector2 AABBColliderComponent::GetMax() const
{
    Vector2 max(GetMin().x + mWidth, GetMin().y + mHeight);

    return max;
}

Vector2 AABBColliderComponent::GetCenter() const
{
    Vector2 center(GetMin().x + mWidth / 2.0f, GetMin().y + mHeight / 2.0f);

    return center;
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{

    bool notcolidding = (GetMax().x < b.GetMin().x) || (b.GetMax().x < GetMin().x) ||
                        (GetMax().y < b.GetMin().y) || (b.GetMax().y < GetMin().y);


    return !notcolidding;

}

AABBColliderComponent::Overlap AABBColliderComponent::GetMinOverlap(AABBColliderComponent* b) const
{
    std::unordered_map<int, Overlap> overlaps;

    overlaps[0] = { b->GetMin().x - GetMax().x, CollisionSide::Right, b};
    overlaps[1] = { b->GetMax().x - GetMin().x, CollisionSide::Left, b};
    overlaps[2] = { b->GetMax().y - GetMin().y, CollisionSide::Top, b};
    overlaps[3] = { b->GetMin().y - GetMax().y, CollisionSide::Down, b};

    Overlap minimo = overlaps.begin()->second;

    for (const auto& overlap : overlaps) {
        if (std::abs(overlap.second.amount) < std::abs(minimo.amount)) {
            minimo = overlap.second;
        }
    }

    return minimo;

}

void AABBColliderComponent::ResolveCollisions(RigidBodyComponent* rigidBody, const Overlap& minOverlap)
{


    if (((minOverlap.side == CollisionSide::Top) && (rigidBody->GetVelocity().y < 0)) || ((minOverlap.side == CollisionSide::Down) && (rigidBody->GetVelocity().y > 0))) {
        auto pos_rigid = rigidBody->GetOwner()->GetPosition();
        rigidBody->GetOwner()->SetPosition((pos_rigid + Vector2(0, minOverlap.amount)));
        rigidBody->SetVelocity((Vector2(rigidBody->GetVelocity().x, 0)));


    }

    else if (((minOverlap.side == CollisionSide::Left) && (rigidBody->GetVelocity().x < 0)) || ((minOverlap.side == CollisionSide::Right) && (rigidBody->GetVelocity().x > 0))) {
        auto pos_rigid = rigidBody->GetOwner()->GetPosition();
        rigidBody->GetOwner()->SetPosition((pos_rigid + Vector2(minOverlap.amount, 0)));
        rigidBody->SetVelocity((Vector2(0, rigidBody->GetVelocity().y)));
    }

}


void AABBColliderComponent::DetectCollision(RigidBodyComponent* rigidBody)
{


    auto colliders = mOwner->GetGame()->GetColliders();

    Vector2 thisCenter = this->GetCenter();
    std::sort(colliders.begin(), colliders.end(), [this](AABBColliderComponent* a, AABBColliderComponent* b) {
        return (a->GetCenter() - this->GetCenter()).Length() < (b->GetCenter() - this->GetCenter()).Length();
    });



    std::unordered_map<CollisionSide, Overlap> responses;

    bool verl = false, horl = false;
    bool parar = false;



    for (auto &i :colliders) {

        if (i == this) continue;

        if (!(i->IsEnabled())) {
            continue;
        }
        AABBColliderComponent::Overlap minOverlap = GetMinOverlap(i);
        if (Intersect(*i)) {
            ResolveCollisions(rigidBody, minOverlap);
            responses[minOverlap.side] = minOverlap;
        }

        for (const auto& response : responses) {
            if ((response.first == CollisionSide::Top) || (response.first == CollisionSide::Down)) {
                verl = true;
            }
            else if ((response.first == CollisionSide::Left) || (response.first == CollisionSide::Right)) {
                horl = true;
            }

            if (horl && verl) {
                parar = true;
                break;
            }
        }
        if (parar) break;

    }

    mOwner->OnCollision(responses);

}