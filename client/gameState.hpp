/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game state for client
*/

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include "networkManager.hpp"
#include "player.hpp"
#include <mutex>
#include <string>
#include <vector>

// Game map objects enum - renamed COIN to avoid conflict with
// JetpackCommand
enum MapObject {
    EMPTY = '_',
    COIN_OBJECT = 'c',      // Renamed from COIN to avoid conflict
    ELECTRIC = 'e'
};

class GameState {
  public:
    GameState();
    ~GameState() = default;

    // Method to update state from network data
    void updateFromNetwork(const std::vector<std::string> &map,
        const std::vector<NetworkManager::PlayerPosition>
            &playerPositions,
        bool gameStarted, bool gameEnded, int gameResult);

    // Game state data
    std::vector<std::string> map;
    std::vector<Player> players;
    bool gameStarted;
    bool gameEnded;
    int gameResult;
    float mapOffset;

    // Mutex for thread-safe access
    mutable std::mutex stateMutex;
};

#endif /* !GAME_STATE_HPP_ */
