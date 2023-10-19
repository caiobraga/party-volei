//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include <iostream>

AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h, ColliderLayer layer, int updateOrder)
        :Component(owner, updateOrder)
        ,mOffset(Vector2((float)dx, (float)dy))
        ,mWidth(w)
        ,mHeight(h)
        ,mLayer(layer)
{
    mOwner->GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent()
{
    mOwner->GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const
{
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.1 (1 linha): Calcule (e retorne) o ponto mínimo dessa AABB. A variável `mOffset`
    //  define a posição da AABB com relação a posição do objeto dono do componente. Portanto,
    //  basta somar a posição do objeto dono do componente a esse deslocamento.
    return mOwner->GetPosition() + mOffset;



}

Vector2 AABBColliderComponent::GetMax() const
{
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.2 (1 linha): Calcule (e retorne) o ponto máximo dessa AABB. As variáveis membro
    //  `mWidth` e `mHeight` definem a altura e a largura da AABB, respectivamente. Portanto,
    //  basta somar a largura à coordenada x e a altura à coordenada y do ponto mínimo da AABB
    //  (utilize o método `GetMin` implementado na etapa anterior).
    Vector2 max(GetMin().x + mWidth, GetMin().y + mHeight);

    return max;
}

Vector2 AABBColliderComponent::GetCenter() const
{
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.3 (1 linha): Calcule (e retorne) o centro dessa AABB. Esse ponto pode ser calculado
    //  de forma similar ao ponto máximo, basta somar a metade da largura à coordenada x e a metade da altura
    //  à coordenada y do ponto mínimo da AABB (utilize o método `GetMin` implementado anteriormente).
    Vector2 min = GetMin();


    Vector2 center(min.x + mWidth / 2.0f, min.y + mHeight / 2.0f);

    return center;
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 2.1 (~5 linhas): Verifique se esta AABB está colidindo com a AABB b passada como parâmetro.
    //  Retorne verdadeiro se estiver e falso caso contrário. Utilize os métodos `GetMin` e `GetMax`
    //  para acessar os pontos de mínimo e máximo das duas AABBs.
    Vector2 thisMin = GetMin();
    Vector2 thisMax = GetMax();
    Vector2 bMin = b.GetMin();
    Vector2 bMax = b.GetMax();

    bool xCollision = thisMin.x <= bMax.x && thisMax.x >= bMin.x;
    bool yCollision = thisMin.y <= bMax.y && thisMax.y >= bMin.y;


    return xCollision && yCollision;
}

AABBColliderComponent::Overlap AABBColliderComponent::GetMinOverlap(AABBColliderComponent* b) const
{
    // --------------
    // TODO - PARTE 2
    // --------------
    std::unordered_map<int, Overlap> overlaps;

    // TODO 3.1 (~4 linhas): Armazene no mapa `overlaps` as sobreposições (com sinal -/+) dos quatro lados da
    //  colisão: esquerda, direita, cima e baixo. Utilize os métodos `GetMin` e `GetMax` para acessar os
    //  pontos de mínimo e máximo das duas AABBs.

    Vector2 aMin = this->GetMin();
    Vector2 aMax = this->GetMax();
    Vector2 bMin = b->GetMin();
    Vector2 bMax = b->GetMax();


    overlaps[0] = { bMax.x - aMin.x, CollisionSide::Left, b };
    overlaps[1] = { bMin.x - aMax.x, CollisionSide::Right, b };
    overlaps[2] = { bMax.y - aMin.y, CollisionSide::Top, b };
    overlaps[3] = { bMin.y - aMax.y, CollisionSide::Down, b };


    // TODO 3.2 (~4 linhas): Encontre e retorne a sobreposição mínima. Para isso, utilize
    //  os valores absolutos das sobreposições calculadas na etapa anterior.

    Overlap min = overlaps.begin()->second;

    for (const auto& overlap : overlaps){
        if (std::abs(overlap.second.amount) < std::abs(min.amount)){
            min = overlap.second;
        }
    }

    return min;
    
}

void AABBColliderComponent::ResolveCollisions(RigidBodyComponent *rigidBody, const Overlap& minOverlap)
{
    
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 4.1 (~4 linhas): Verifique se a sobreposição `minOverlap` ocorreu no lado de cima `CollisionSide::Top`
    //  com velocidade vertical negativa ou no lado de baixo `CollisionSide::Down` com velocidade vertical positiva.
    //  Note que a estrutura `minOverlap` já possui o lado onde ocorreu a colisão `minOverlap.side`.
    //  Se um desses dois casos for verdadeiro, some a quantidade de sobreposição `minOverlap.amount`
    //  à posição vertical do dono dessa AABB e reinicialize sua velocidade vertical para zero .
    //  Dica: para verificar, por exemplo, se a colisão foi por cima, basta comparar se minOverlap.side
    //  é igual a CollisionSide::Top.
   
    if (((minOverlap.side == CollisionSide::Top) && (rigidBody->GetVelocity().y < 0)) || ((minOverlap.side == CollisionSide::Down) && (rigidBody->GetVelocity().y > 0))) {
        //this->GetOwner()->SetPosition((rigidBody->GetOwner()->GetPosition() + Vector2(0, minOverlap.amount)));
        rigidBody->GetOwner()->SetPosition((rigidBody->GetOwner()->GetPosition() + Vector2(0, minOverlap.amount)));
        //this->GetOwner()->GetComponent<RigidBodyComponent>()->SetVelocity((Vector2(rigidBody->GetVelocity().x, 0)));
        rigidBody->SetVelocity((Vector2(rigidBody->GetVelocity().x, 0)));
        //std::cout << "COLISAO 1" << std::endl;
      
    }
    

    // TODO 4.2 (~4 linhas): Caso nenhum dos dois casos anteriores sejam verdadeiros, verifique se a sobreposição
    //  `minOverlap` ocorreu no lado esquerdo `CollisionSide::Left` com velocidade horizontal negativa ou no lado direito
    //  `CollisionSide::Right` com velocidade horizontal positiva. Se um desses dois casos for verdadeiro, some a quantidade
    //  de sobreposição `minOverlap.amount` à posição horizontal do dono dessa AABB e reinicialize sua velocidade horizontal
    //  para zero.
    else if (((minOverlap.side == CollisionSide::Left) && (rigidBody->GetVelocity().x < 0)) || ((minOverlap.side == CollisionSide::Right) && (rigidBody->GetVelocity().x > 0))) {
        rigidBody->GetOwner()->SetPosition((rigidBody->GetOwner()->GetPosition() + Vector2( minOverlap.amount, 0)));
        rigidBody->SetVelocity((Vector2(0, rigidBody->GetVelocity().y)));
        //this->GetOwner()->SetPosition((rigidBody->GetOwner()->GetPosition() + Vector2(minOverlap.amount, 0)));
        //this->GetOwner()->GetComponent<RigidBodyComponent>()->SetVelocity((Vector2(0, rigidBody->GetVelocity().y)));
        //std::cout << "COLISAO 2" << std::endl;
    }
    
}

bool compareAABBCollidersByDistance(const AABBColliderComponent* a, const AABBColliderComponent* b) {
    Vector2 aCenter = a->GetCenter();
    Vector2 bCenter = b->GetCenter();

    float distA = ((bCenter.x - aCenter.x) * (bCenter.x - aCenter.x)) + ((bCenter.y - aCenter.y) * (bCenter.y - aCenter.y));
    float distB = ((aCenter.x - bCenter.x) * (aCenter.x - bCenter.x)) + ((aCenter.y - bCenter.y) * (aCenter.y - bCenter.y));

    return distA < distB;  // Comparação de distâncias
}


void AABBColliderComponent::DetectCollision(RigidBodyComponent *rigidBody)
{
    
    // --------------
    // TODO - PARTE 2
    // --------------
    // Sort colliders by amount to the player (center-to-center)

    auto colliders = mOwner->GetGame()->GetColliders();

    // TODO 5.1 (~3 linhas): Utilize a função `std::sort` para ordenar o vetor `colliders` de acordo com a
    //  distância entre o centro dessa AABB e o centro de cada AABB desse vetor. O vetor `colliders` contém
    //  as AABBs de todos os atores do jogo (mario, goombas e blocos). Ordenar esse vetor dessa forma fará
    //  com que as colisões mais próximas sejam resolvidas primeiro, zerando as velocidades dos objetos na
    //  ordem esperada.
    Vector2 thisCenter = this->GetCenter();
    std::sort(colliders.begin(), colliders.end(), compareAABBCollidersByDistance);
    


    std::unordered_map<CollisionSide, Overlap> responses;

    bool verticalCol = false, horizontalCol = false;
    bool stop = false;


    // TODO 5.2: Utilize um laço para percorra o vetor `colliders` ordenado,
    //  verificando colisões com cada AABB alvo. Em cada iteração do laço, execute as seguintes operações.
    for (int i = 0; i < colliders.size(); i++) {
        AABBColliderComponent* col = colliders[i];
        //  - 5.2.1 (~2 linhas): verifique se o elemento corrente é a AABB desse objeto (this). Se for, continue para a próximo
        //    elemento, pois não precisamos verificar colisão de uma AABB com ela mesma.
        if (col == this) continue;

        //  - 5.2.2 (~2 linhas): verifique se o elemento corrente está habilitado (IsEnabled()). Se não estiver, continue para a próximo
       //    elemento, pois não queremos verificar colisão de uma AABB desabilitada.
        if (!(col->IsEnabled())) {
            continue;
        }
       //  - 5.2.3 (~3 linhas): Utilize a função `GetMinOverlap` para obter a sobreposição mínima dessa AABB com o elemento corrente.
       //    Em seguida, utilize a função `ResolveCollisions` para resolver a colisão entre essa AABB e o elemento corrente. Por fim,
       //    armazene os dados dessa sobreposição no mapa `responses`. Essa mapa será utilizado para enviar uma mensagem
       //    de callback `OnCollision` para o objeto dono dessa AABB.
        AABBColliderComponent::Overlap minOverlap = GetMinOverlap(col);
        if(Intersect(*col)) {
            ResolveCollisions(rigidBody, minOverlap);
            responses[minOverlap.side] = minOverlap;
        }

       //  - 5.2.4 (~3 linhas): Verifique se já houve uma colisão vertical e uma horizontal durante o laço.
       //    Se sim, interrompa o laço (break), pois não precisamos verificar mais colisões.
        for (const auto& response : responses) {
            if ((response.first == CollisionSide::Top)||(response.first == CollisionSide::Down)) {
                verticalCol = true;
            }
            else if ((response.first == CollisionSide::Left) || (response.first == CollisionSide::Right)) {
                horizontalCol = true;
            }

            if (horizontalCol && verticalCol) {
                stop = true;
                //std::cout << "Saiu" << std::endl;
                break; 
            }
        }
        if (stop) break;
        
    }

    // Callback only for closest (first) collision
    mOwner->OnCollision(responses);
    
}