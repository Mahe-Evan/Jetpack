/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game state implementation
*/

#include "gameState.hpp"

GameState::GameState()
    : gameStarted(false), gameEnded(false), gameResult(0),
      mapOffset(0.0f)
{}

void GameState::updateFromNetwork(
    const std::vector<std::string> &newMap,
    const std::vector<NetworkManager::PlayerPosition>
        &playerPositions,
    bool started, bool ended, int result)
{
    std::lock_guard<std::mutex> lock(stateMutex);

    if (!newMap.empty()) {
        map = newMap;
    }

    players.clear();
    for (const auto &pos : playerPositions) {
        Player player;
        player.id = pos.playerId;
        player.x = pos.x;
        player.y = pos.y;
        player.score = pos.score;
        player.flying = pos.flying;
        players.push_back(player);
    }

    gameStarted = started;
    gameEnded = ended;
    gameResult = result;

    if (gameStarted && !gameEnded && !players.empty()) {
        for (const auto &player : players) {
            if (player.id == 1) {
                float targetOffset = player.x * 40.0f - 400.0f;
                mapOffset = targetOffset > 0 ? targetOffset : 0;
                break;
            }
        }
    }
}
