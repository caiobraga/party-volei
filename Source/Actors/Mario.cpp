//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Mario.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Mario::Mario(Game* game,
           const float forwardSpeed,
           const float jumpSpeed)
        : Actor(game)
        , mIsRunning(false)
        , mIsDead(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (~1 linha): Até que você termine a implementação do componente de animação `DrawAnimatedComponent`, utilize o componente
    //  para desenho de sprites estáticos `DrawSpriteComponent`. Crie um desses
    //  componentes com a textura `Assets/Sprites/Mario/Idle.png` e dimensões (32,32).

    // TODO 1.2 (~1 linha): Para que o jogador possa se mover, crie também um componente `RigidBodyComponent` com massa `1.0` e coeficiente de atrito `5.0`. Note que a
    //  classe `Mario` já tem atributos para armazenar esses componentes.

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.1 (~1 linhas): Crie um componente AABBColliderComponent passando
    //  as dimensões (0,0,32,32) e o tipo ColliderLayer::Player da AABB. Se quiser desenhar a AABB do Mário
    //  para testar as colisões, crie um componente `DrawPolygonComponent` com os vértices da AABB.

    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 1.1 (~1 linhas): Crie um componente DrawAnimatedComponent passando os caminhos da imagem (.png) e
    //  dos dados (.json) do sprite sheet do Mário que você criou com o TexturePacker.

    // TODO 1.2 (~4 linhas): Utilize a função `AddAnimation` para adicionar as animações "dead", "idle",
    //  "jump" e "run".

    // TODO 1.2 (~2 linhas): Utilize a função `SetAnimation` para definir a animação inicial como "idle". Em seguida,
    //  utilize a função `SetAnimFPS` para definir a taxa de atualização de quadros da animação para 10.0f.
}

void Mario::OnProcessInput(const uint8_t* state)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (~2 linhas): Verifique se o jogador pressionou a tecla `D`. Se sim, aplique uma força para
    //  a direita com magnitude `mForwardSpeed` e altere a rotação `mRotation` para `0`. Além disso, altere
    //  a variável `mIsRunning` para verdadeiro. Isso será importante para controlar as animações na Parte 4.

    // TODO 1.2 (~2 linhas): Verifique se o jogador pressionou a tecla `A`. Se sim, aplique uma força para a
    //  esquerda com magnitude `mForwardSpeed` e altere a rotação `mRotation` para `Math::Pi`. Além disso,
    //  altere a variável `mIsRunning` para verdadeiro. Caso o jogador não estiver pressionando nem `D`, nem `A`,
    //  altere essa variável para falso. Isso será importante para controlar as animações na Parte 4.

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 2.1 (~3 linhas): Verifique se o jogador está no chão (`mIsOnGround`) e se ele pressionou a
    //  tecla `A`. Se sim, altere a velocidade vertical para `mJumpSpeed` e a variável `mIsOnGround`
    //  para falso. Utilize `SetVelocity` ao invés de `ApplyForce` para que o pulo seja mais rápido e preciso.
}

void Mario::OnUpdate(float deltaTime)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (~2 linhas): Para evitar que o jogador ultrapasse o limite inferior (esquerdo) da câmera,
    //  limite sua posição horizontal para ser sempre maior ou igual a posição horizontal da câmera.

    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 2.1 (~2 linhas): Verifique se a posição vertical do jogador é maior do que o tamanho da tela.
    //  Se for, chame o método `Kill`.

    ManageAnimations();
}

void Mario::ManageAnimations()
{
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 5.1 (~7 linhas): para implementar a troca de animação, basta utilizar os atributos `mIsDead` para
    //  verificar se o jogador está morto, `mIsOnGround` se o jogador está no chão e `mIsRunning` se o jogador está correndo.
    //  -5.1.1: Se ele estiver morto, altere a animação para `idle`
    //  -5.1.2: Se estiver vivo, no chão e correndo, altere a animação para `run`
    //  -5.1.3: Se estiver vivo, no chão e não estiver correndo, altere a animação para `idle`
    //  -5.1.4: Se estiver vivo e não estiver no chão, altere a animação para `jump`
}

void Mario::Kill()
{
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 3.1 (~4 linhas): altere a animação para "dead" e o valor da variável `mIsDead` para verdadeiro.
    //  Além disso, desabilite `SetEnabled(false)` os componentes `mRigidBodyComponent` e  `mColliderComponent`
}

void Mario::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
{
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 4.1 (~14 linhas):  O mapa `responses` contém os dados de cada colisão desse objeto nesse quadro.
    //  Cada elemento de  `responses` é uma estrutura do tipo `Overlap`, que contém o lado da colisão `side`,
    //  o tamanho da sobreposição `amount` (com sinal -/+) e um ponteiro `target` para a AABB que colidiu com o objeto.
    //  Percorra essa mapa atualizando o estado do jogador bom base no tipo de colisão:

    //  TODO 4.1.1: Se a colisão ocorreu com um objeto do tipo `ColliderLayer::Blocks` pelo lado de
    //    baixo `CollisionSide::Down`, significa que o jogador aterrizou no chão. Nesse caso, altere o valor da variável
    //    `mIsOnGround` para verdadeiro.

    //  TODO 4.1.2: Se a colisão ocorreu com um objeto do tipo `CollisionSide::Enemy` pelo lado de baixo,
    //    significa que o jogador acertou um goomba no ar. Nesse caso, mate esse goomba e altere a velocidade do jogador
    //    para dar um "meio pulo" (`mJumpSpeed/1.5f`). Utilize o ponteiro `target` do elemento corrente para acessar
    //    o ponteiro para esse goomba e o método `Kill` do goomba para matá-lo `target->GetOwner()->Kill()`.

    //  TODO 4.1.3: Se a colisão ocorreu com um objeto do tipo `CollisionSide::Enemy` pelo lado direito ou
    //   esquerdo e o jogador está no chão, significa que o goomba acertou o jogador. Nesse caso, utilize a função
    //   `Kill` para matar o jogador.

    //  TODO 4.1.4: Se a colisão ocorreu com um objeto do tipo `CollisionSide::Enemy` pelo lado direito ou
    //   esquerdo e o jogador não está no chão, também significa que o jogador acertou um goomba no ar. Nesse caso,
    //   mate o goomba como no caso 2.
}
