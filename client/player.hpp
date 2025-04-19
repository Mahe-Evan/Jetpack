/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Player class for client
*/

#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
 public:
  Player();
  ~Player() = default;

  int id_;
  float x_;
  float y_;
  int score_;
  bool flying_;
};

#endif  // PLAYER_H_
