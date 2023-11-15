#pragma once
#include "./Actors/Mario.h"  // Include the necessary player class
#include "vector"
class PlayerProcessor {
public:
    std::vector<Mario*> players;

    PlayerProcessor();

    ~PlayerProcessor();

    void AddPlayer(Mario *player);
    void ManagePlayers();
    // You can add more member functions as needed
};
