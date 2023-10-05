//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Goomba.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

Goomba::Goomba(Game* game, float forwardSpeed, float deathTime)
        : Actor(game)
        , mDyingTimer(deathTime)
        , mIsDying(false)
        , mForwardSpeed(forwardSpeed)
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 1.1 (2 linhas): Crie o `RigidBodyComponent` com massa `1.0f` e coeficiente de atrito `0.0`
    //  (basta omitir esse parâmetro para inicializa-lo com zero). Altere a velocidade horizontal do
    //  goomba para `mForwardSpeed`.

    // TODO 1.2 (1 linha): Crie o `AABBColliderComponent` com dimensões `(0,0,32,32)` e tipo de colisão
    //  `ColliderLayer::Enemy`.

    // TODO 1.3 (1 linha): Crie o componente `DrawAnimatedComponent` passando os caminhos da imagem (.png) e
    //  dos dados (.json) do sprite sheet do goomba que você criou com o TexturePacker.
    //  - 1.3.1 (2 linha): Utilize a função `AddAnimation` para adicionar as animações "walk" e "dead".
    //  - 1.3.2 (2 linhas): Utilize a função `SetAnimation` para definir a animação inicial como "walk".
    //    Em seguida, utilize a função `SetAnimFPS` para definir a taxa de atualização de quadros da animação
    //    para 5.0f.
}

void Goomba::Kill()
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 2.1 (~4 linhas): altere a animação para "dead" e o valor da variável mIsDying para verdadeiro.
    //  Além disso, desabilite `SetEnabled(false)` os componentes `mRigidBodyComponent` e  `mColliderComponent`
}

void Goomba::OnUpdate(float deltaTime)
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 3.1 (~4 linhas): Verifique se a variável `mDyingTimer` é verdadeira. Se for, decremente o cronômetro
    //  `mDyingTimer` pelo delta time. Quando esse cronômetro chegar a zero, altere o estado do goomba para `ActorState::Destroy`

    // TODO 3.2 (~2 linhas): Verifique se a posição vertical do goomba é maior do que o tamanho da tela. Se for,
    //  altere o estado do goomba para `ActorState::Destroy`
}

void Goomba::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 4.1 (~4 linhas): Percorra o mapa `responses` atualizando o estado do jogador bom base no
    //  tipo de colisão ocorrida. Se a colisão foi à esquerda, altere a velocidade horizontal para
    //  mForwardSpeed. Se foi à direita, altera a velocidade horizontal para -mForwardSpeed.
}