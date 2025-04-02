/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Player class for client
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

class Player {
  public:
    Player();
    ~Player() = default;

    int id;
    float x;
    float y;
    int score;
    bool flying;
};

#endif /* !PLAYER_HPP_ */
