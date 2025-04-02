/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Game state implementation
*/

#include "gameState.hpp"

GameState::GameState()
    : localPlayerId(-1), ready(false), gameStarted(false),
      gameEnded(false), gameResult(0), mapOffset(0), flyStatus(false)
{}

Player *GameState::getLocalPlayer()
{
    for (auto &player : players) {
        if (player.id == localPlayerId) {
            return &player;
        }
    }
    return nullptr;
}

Player *GameState::getPlayerById(int id)
{
    for (auto &player : players) {
        if (player.id == id) {
            return &player;
        }
    }
    return nullptr;
}
