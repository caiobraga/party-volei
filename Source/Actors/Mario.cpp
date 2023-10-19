//
// Created by Lucas N. Ferreira on 03/08/23.
//
#include <iostream>
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
    //mDrawComponent = new DrawSpriteComponent(this, "D:\\Henrique\\UFV\\Inf 216\\mario\\mario\\Assets\\Sprites\\Mario\\Idle.png", 32, 32);
    // TODO 1.2 (~1 linha): Para que o jogador possa se mover, crie também um componente `RigidBodyComponent` com massa `1.0` e coeficiente de atrito `5.0`. Note que a
    //  classe `Mario` já tem atributos para armazenar esses componentes.
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.1 (~1 linhas): Crie um componente AABBColliderComponent passando
    //  as dimensões (0,0,32,32) e o tipo ColliderLayer::Player da AABB. Se quiser desenhar a AABB do Mário
    //  para testar as colisões, crie um componente `DrawPolygonComponent` com os vértices da AABB.
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Player);

    std::vector<Vector2> aabbVertices = {
    Vector2(0, 0), Vector2(32, 0), Vector2(32, 32), Vector2(0, 32)
    };
    new DrawPolygonComponent(this, aabbVertices);

    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 1.1 (~1 linhas): Crie um componente DrawAnimatedComponent passando os caminhos da imagem (.png) e
    //  dos dados (.json) do sprite sheet do Mário que você criou com o TexturePacker.
    mDrawComponent = new DrawAnimatedComponent(this, "D:\\Henrique\\UFV\\Inf 216\\mario\\mario\\Assets\\Sprites\\Mario\\MarioSprite.png", "D:\\Henrique\\UFV\\Inf 216\\mario\\mario\\Assets\\Sprites\\Mario\\MarioSprite.json");

    // TODO 1.2 (~4 linhas): Utilize a função `AddAnimation` para adicionar as animações "dead", "idle",
    //  "jump" e "run".
    mDrawComponent->AddAnimation("dead",{0});
    mDrawComponent->AddAnimation("idle", {1});
    mDrawComponent->AddAnimation("jump", {2});
    mDrawComponent->AddAnimation("run", {3,4,5});
    // TODO 1.2 (~2 linhas): Utilize a função `SetAnimation` para   definir a animação inicial como "idle". Em seguida,
    //  utilize a função `SetAnimFPS` para definir a taxa de atualização de quadros da animação para 10.0f.
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Mario::OnProcessInput(const uint8_t* state)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (~2 linhas): Verifique se o jogador pressionou a tecla `D`. Se sim, aplique uma força para
    //  a direita com magnitude `mForwardSpeed` e altere a rotação `mRotation` para `0`. Além disso, altere
    //  a variável `mIsRunning` para verdadeiro. Isso será importante para controlar as animações na Parte 4.

    if (state[SDL_SCANCODE_D]) {
        mRotation = 0;
        mIsRunning = true;
        mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed, 0));
    }
    // TODO 1.2 (~2 linhas): Verifique se o jogador pressionou a tecla `A`. Se sim, aplique uma força para a
    //  esquerda com magnitude `mForwardSpeed` e altere a rotação `mRotation` para `Math::Pi`. Além disso,
    //  altere a variável `mIsRunning` para verdadeiro. Caso o jogador não estiver pressionando nem `D`, nem `A`,
    //  altere essa variável para falso. Isso será importante para controlar as animações na Parte 4.
    else if (state[SDL_SCANCODE_A]) {
        mRotation = Math::Pi;
        mIsRunning = true;
        mRigidBodyComponent->ApplyForce(Vector2(-mForwardSpeed, 0));
    }
    else {
        mIsRunning = false;
    }

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 2.1 (~3 linhas): Verifique se o jogador está no chão (`mIsOnGround`) e se ele pressionou a
    //  tecla `A`. Se sim, altere a velocidade vertical para `mJumpSpeed` e a variável `mIsOnGround`
    //  para falso. Utilize `SetVelocity` ao invés de `ApplyForce` para que o pulo seja mais rápido e preciso.
    if (mIsOnGround && (state[SDL_SCANCODE_SPACE])) {
        mIsOnGround = false;
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed));
    }
}

void Mario::OnUpdate(float deltaTime)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (~2 linhas): Para evitar que o jogador ultrapasse o limite inferior (esquerdo) da câmera,
    //  limite sua posição horizontal para ser sempre maior ou igual a posição horizontal da câmera.
    if (this->GetPosition().x <= this->GetGame()->GetCameraPos().x)
        this->SetPosition(Vector2(this->GetGame()->GetCameraPos().x, this->GetPosition().y));
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 2.1 (~2 linhas): Verifique se a posição vertical do jogador é maior do que o tamanho da tela.
    //  Se for, chame o método `Kill`.
    if (this->GetPosition().y > this->GetGame()->GetWindowHeight()) this->Kill();

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
    if (mIsDead) mDrawComponent->SetAnimation("dead");
    else {
        if (mIsOnGround) {
            //  -5.1.2: Se estiver vivo, no chão e correndo, altere a animação para `run`
            if (mIsRunning)
                mDrawComponent->SetAnimation("run");
            //  -5.1.3: Se estiver vivo, no chão e não estiver correndo, altere a animação para `idle`
            else
                mDrawComponent->SetAnimation("idle");
        }
        //  -5.1.4: Se estiver vivo e não estiver no chão, altere a animação para `jump`
        else
            mDrawComponent->SetAnimation("jump"); 
    }
}

void Mario::Kill()
{
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 3.1 (~4 linhas): altere a animação para "dead" e o valor da variável `mIsDead` para verdadeiro.
    //  Além disso, desabilite `SetEnabled(false)` os componentes `mRigidBodyComponent` e  `mColliderComponent`
    mDrawComponent->SetAnimation("dead");
    //std::cout << "Mario Morreu" << std::endl;
    
    mColliderComponent->SetEnabled(false);
    mRigidBodyComponent->SetEnabled(false);
   // mRigidBodyComponent->SetVelocity(Vector2(0, +1000));
    mIsDead = true;

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

    for (auto& entry : responses) {
        AABBColliderComponent::Overlap overlap = entry.second;
        CollisionSide side = entry.first;
        AABBColliderComponent* target = overlap.target;

        //  TODO 4.1.1: Se a colisão ocorreu com um objeto do tipo `ColliderLayer::Blocks` pelo lado de
        //    baixo `CollisionSide::Down`, significa que o jogador aterrizou no chão. Nesse caso, altere o valor da variável
        //    `mIsOnGround` para verdadeiro.
        if (target->GetLayer() == ColliderLayer::Blocks && side == CollisionSide::Down) {
            mIsOnGround = true;
        }

        //  TODO 4.1.2: Se a colisão ocorreu com um objeto do tipo `CollisionSide::Enemy` pelo lado de baixo,
        //    significa que o jogador acertou um goomba no ar. Nesse caso, mate esse goomba e altere a velocidade do jogador
        //    para dar um "meio pulo" (`mJumpSpeed/1.5f`). Utilize o ponteiro `target` do elemento corrente para acessar
        //    o ponteiro para esse goomba e o método `Kill` do goomba para matá-lo `target->GetOwner()->Kill()`.
        if (target->GetLayer() == ColliderLayer::Enemy && side == CollisionSide::Down) {
            this->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2(this->GetComponent<RigidBodyComponent>()->GetVelocity().x, mJumpSpeed / 1.5f));
            target->GetOwner()->Kill();
        }

        //  TODO 4.1.3: Se a colisão ocorreu com um objeto do tipo `CollisionSide::Enemy` pelo lado direito ou
        //   esquerdo e o jogador está no chão, significa que o goomba acertou o jogador. Nesse caso, utilize a função
        //   `Kill` para matar o jogador.
        if (target->GetLayer() == ColliderLayer::Enemy && (side == CollisionSide::Right || side == CollisionSide::Left) && mIsOnGround) {
            this->Kill();
        }

        //  TODO 4.1.4: Se a colisão ocorreu com um objeto do tipo `CollisionSide::Enemy` pelo lado direito ou
        //   esquerdo e o jogador não está no chão, também significa que o jogador acertou um goomba no ar. Nesse caso,
        //   mate o goomba como no caso 2.
        if (target->GetLayer() == ColliderLayer::Enemy && (side == CollisionSide::Right || side == CollisionSide::Left) && !mIsOnGround) {
            this->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2(this->GetComponent<RigidBodyComponent>()->GetVelocity().x, mJumpSpeed / 1.5f));
            target->GetOwner()->Kill();

        }
    }
}
