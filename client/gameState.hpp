/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game state for client
*/

#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <mutex>
#include <string>
#include <vector>

#include "networkManager.hpp"
#include "player.hpp"

enum MapObject { kEmpty = '_', kCoinObject = 'c', kElectric = 'e' };

class GameState {
 public:
  GameState();
  ~GameState() = default;

  void UpdateFromNetwork(const std::vector<std::string>& new_map,
                         const std::vector<PlayerPosition>& player_positions,
                         bool game_started, bool game_ended, int game_result);

  std::vector<std::string> map_;
  std::vector<Player> players_;
  bool game_started_;
  bool game_ended_;
  int game_result_;
  float map_offset_;

  mutable std::mutex state_mutex_;
};

#endif  // GAME_STATE_H_
