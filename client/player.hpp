/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Player definition
*/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

struct Player {
    int id;
    float x;
    float y;
    int score;
    bool flying;
    sf::Sprite sprite;

    Player();
    Player(int playerId, float posX = 0.0f, float posY = 5.0f);
};

#endif      // PLAYER_HPP
