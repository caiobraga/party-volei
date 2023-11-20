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
    // Function to add score to a player
    int scoreTeam1 = 0 ;
    int scoreTeam2 = 0;

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
    }
};
