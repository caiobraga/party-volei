#include "PlayerProcessor.h"

PlayerProcessor::PlayerProcessor() {}

PlayerProcessor::~PlayerProcessor() {}

void PlayerProcessor::ManagePlayers() {
    // Implement your player management logic here
    // Example:
    // Call the necessary player functions to manage players in the game
    // For example, updating their positions, handling collisions, etc.
}

void PlayerProcessor::AddPlayer(Mario *player) {
    players.push_back(player);
}