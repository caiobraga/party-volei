//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Point.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Random.h"

Point::Point(Game* game, const std::string &texturePath, int mlevelWidth, int mlevelHeight)
        :Actor(game)
        , mlevelHeight(mlevelHeight)
        ,mlevelWidth(mlevelWidth)
{
    // --------------
    // TODO - PARTE 1
    // --------------


     mColliderComponent =  new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Point);
    mDrawComponent  = new DrawSpriteComponent(this, texturePath, 32, 32);
}

void Point::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) {
    for (auto& i : responses) {
        auto target = i.second.target;

        if (target->GetLayer() == ColliderLayer::Ball) {
            if (i.first == CollisionSide::Down || i.first == CollisionSide::Top ||
                i.first == CollisionSide::Left || i.first == CollisionSide::Right) {
                // Assuming you have access to LEVEL_WIDTH and LEVEL_HEIGHT in your Point class
                ReSpawnPointActor();
                mGame->mplayerProcessor.AddScore(1);
                break;
            }
        }
    }
}


void Point::ReSpawnPointActor() {
    // Seed the random number generator


    // Generate random coordinates within the screen dimensions
    int randomX =  Random().GetFloatRange(5, mlevelWidth - 5 );   // Random X-coordinate
    int randomY = Random().GetFloatRange(5, mlevelHeight - 8 );  // Random Y-coordinate


    if(randomX > mlevelWidth / 2 - 1 && randomX < mlevelWidth / 2 + 1){
        randomX = 4;
    }

    // Create a point actor at the random coordinates
    //std::string pointTexture = "../Assets/Sprites/Blocks/BlockD.png";
   // auto point = new Point(this->mGame,  pointTexture);
   // int y = mGame->mWindowHeight - 14 * 32;

    this->SetPosition(Vector2(randomX * 32, randomY * 32 ) );
    // Add the point actor to the scene or your actor management system
    // mGame->AddActor(point); // Uncomment this if your game handles actor management

    // Note: Replace 'PointActor' and update the code to add the actor to your scene or game
}


