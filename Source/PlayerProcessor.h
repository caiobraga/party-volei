#pragma once

#include <map>
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
    // Function to add score to a player
    int scoreTeam1 = 0 ;
    int scoreTeam2 = 0;

    std::map<Mario*, int> playerLives;

    int GetLives(Mario* player) const {
        auto it = playerLives.find(player);
        if (it != playerLives.end()) {
            return it->second;
        }
        return 0;  // Return 0 if player not found
    }

    void SetLives(Mario* player, int newLives) {
        playerLives[player] = newLives;
    }

    void DecreaseLife(Mario* player) {
        auto it = playerLives.find(player);
        if (it != playerLives.end()) {
            it->second--;
        }
    }

    int GetScore(int playerIndex){
        if(playerIndex == 1){
            return scoreTeam1;
        }else{
            return scoreTeam2;
        }
    }

    void AddScore(int playerIndex) {
       if(playerIndex == 1){
           scoreTeam1++;
       }else{
           scoreTeam2++;
       }
    }

    // Function to reset scores for all players
    void ZerarScore() {
        scoreTeam1 = 0;
        scoreTeam2 = 0;
        SetLives(players[0], 5);
    }
};
