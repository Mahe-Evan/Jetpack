/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Game state definition
*/

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "player.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr float PLAYER_SPEED = 5.0f;
constexpr float GRAVITY = 0.5f;
constexpr float JETPACK_FORCE = 0.8f;
constexpr float SCROLL_SPEED = 2.0f;

enum MapObjectType {
    EMPTY = '_',
    COIN = 'c',
    ELECTRIC = 'e'
};

class GameState {
  public:
    GameState();

    Player *getLocalPlayer();
    Player *getPlayerById(int id);

    std::vector<Player> players;
    int localPlayerId;
    std::vector<std::string> map;
    bool ready;
    bool gameStarted;
    bool gameEnded;
    int gameResult;
    float mapOffset;
    bool flyStatus;
    std::mutex stateMutex;
};

extern GameState gameState;
extern std::atomic<bool> running;
extern std::condition_variable cv;
extern bool debug_mode;

#endif      // GAME_STATE_HPP
