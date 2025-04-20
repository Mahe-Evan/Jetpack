/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game state implementation
*/

#include "gameState.hpp"

GameState::GameState()
    : game_started_(false),
      game_ended_(false),
      game_result_(0),
      map_offset_(0.0f) {}

void GameState::UpdateFromNetwork(
    const std::vector<std::string>& new_map,
    const std::vector<PlayerPosition>& player_positions, bool started,
    bool ended, int result) {
  std::lock_guard<std::mutex> lock(state_mutex_);

  if (!new_map.empty()) {
    map_ = new_map;
  }

  players_.clear();
  for (const auto& pos : player_positions) {
    Player player;
    player.id_ = pos.player_id;
    player.x_ = pos.x;

    player.y_ = pos.y;

    player.score_ = pos.score;
    player.flying_ = pos.flying;
    players_.push_back(player);
  }

  game_started_ = started;
  game_ended_ = ended;
  game_result_ = result;

  if (game_started_ && !game_ended_ && !players_.empty()) {
    for (const auto& player : players_) {
      if (player.id_ == 1) {
        float target_offset = player.x_ * 40.0f - 400.0f;
        if (target_offset < 0) target_offset = 0;

        float lerp_factor = 0.1f;
        map_offset_ = map_offset_ + (target_offset - map_offset_) * lerp_factor;
        if (map_offset_ < 0) map_offset_ = 0;
        break;
      }
    }
  }
}
