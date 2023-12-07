//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"
#include <string>

class Block : public Actor
{
public:
    explicit Block(Game* game, const std::string &texturePath, bool isRede);
    explicit Block(Game* game, const std::string &texturePath, bool isRede, char c);

};
