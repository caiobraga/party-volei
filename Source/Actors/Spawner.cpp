//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "Mario.h"
#include "Goomba.h"

const float GOOMBA_FORWARD_SPEED = 100.0f;

Spawner::Spawner(Game* game, float spawnDistance)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
{

}

void Spawner::OnUpdate(float deltaTime)
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 1.1 (~6 linhas): Verifique se a distância horizontal entre o jogador (`GetGame()->GetMario()`) e esse
    //  objeto spawner é menor do que `mSpawnDistance`. Se for, crie um novo goomba com velocidade
    //  `GOOMBA_FORWARD_SPEED`. Altere a posição do goomba para ser igual a posição desse spawner.
    //  Em seguida, altere a velocidade do goomba para que ele se mova para a esquerda com velocidade
    //  `GOOMBA_FORWARD_SPEED`. Por fim, destrua esse objeto spawner.
    int dist = std::abs(GetGame()->GetMario()->GetPosition().x - this->GetPosition().x);
    if (dist < mSpawnDistance) {
       Goomba *aux = new Goomba(this->GetGame(), GOOMBA_FORWARD_SPEED);
       aux->SetPosition(this->GetPosition());
       aux->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2(-GOOMBA_FORWARD_SPEED, 0));
       this->SetState(ActorState::Destroy);
       //delete this;

    }
}