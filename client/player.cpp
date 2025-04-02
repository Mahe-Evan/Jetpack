/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Player implementation
*/

#include "player.hpp"

Player::Player() : id(-1), x(0.0f), y(5.0f), score(0), flying(false)
{}

Player::Player(int playerId, float posX, float posY)
    : id(playerId), x(posX), y(posY), score(0), flying(false)
{}
