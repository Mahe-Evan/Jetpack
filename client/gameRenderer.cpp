/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game renderer implementation
*/

#include "gameRenderer.hpp"

#include <iostream>

GameRenderer::GameRenderer(sf::RenderWindow& win, AssetManager& am)
    : window_(win), asset_manager_(am) {}

void GameRenderer::Render(const GameState& game_state) {
  std::lock_guard<std::mutex> lock(game_state.state_mutex_);

  window_.clear(sf::Color(30, 30, 30));

  DrawBackground(game_state);
  DrawMap(game_state);
  DrawPlayers(game_state);
  DrawUI(game_state);

  window_.display();
}

void GameRenderer::DrawBackground(const GameState& game_state) {
  sf::Sprite background_sprite = asset_manager_.GetBackgroundSprite();

  float offset = -static_cast<float>(game_state.map_offset_);
  float window_height = window_.getSize().y;
  float scale = window_height / background_sprite.getLocalBounds().height;
  background_sprite.setScale(scale, scale);

  for (int i = 0; i < 3; i++) {
    background_sprite.setPosition(
        offset + i * background_sprite.getLocalBounds().width * scale, 0);
    window_.draw(background_sprite);
  }
}

void GameRenderer::DrawMap(const GameState& game_state) {
  if (game_state.map_.empty()) {
    return;
  }
  sf::Sprite coin_sprite = asset_manager_.GetCoinSprite();
  sf::Sprite electric_sprite = asset_manager_.GetElectricSprite();

  coin_sprite.setTextureRect(sf::IntRect(0, 0, 200, 200));
  electric_sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));

  coin_sprite.setScale(0.2f, 0.2f);
  electric_sprite.setScale(0.3f, 0.3f);

  const float kTileSize = 40.0f;
  const float window_width = window_.getSize().x;
  const float window_height = window_.getSize().y;

  size_t start_x = game_state.map_offset_ / kTileSize;
  size_t end_x = start_x + static_cast<size_t>(window_width / kTileSize) + 2;

  bool debug_mode = true;

  for (size_t y = 0; y < game_state.map_.size(); y++) {
    float y_pos = y * kTileSize;
    if (y_pos < -kTileSize || y_pos > window_height) {
      continue;
    }

    const std::string& row = game_state.map_[y];
    for (size_t x = start_x; x < end_x && x < row.length(); x++) {
      float x_pos = x * kTileSize - game_state.map_offset_;

      if (row[x] == 'c') {
        coin_sprite.setPosition(
            x_pos + (kTileSize - coin_sprite.getGlobalBounds().width) / 2,
            y_pos + (kTileSize - coin_sprite.getGlobalBounds().height) / 2);
        window_.draw(coin_sprite);

        if (debug_mode) {
          sf::RectangleShape hitbox(sf::Vector2f(kTileSize, kTileSize));
          hitbox.setPosition(x_pos, y_pos);
          hitbox.setFillColor(sf::Color::Transparent);
          hitbox.setOutlineColor(sf::Color::Red);
          hitbox.setOutlineThickness(1.0f);
          window_.draw(hitbox);
        }
      } else if (row[x] == 'e') {
        electric_sprite.setPosition(
            x_pos + (kTileSize - electric_sprite.getGlobalBounds().width) / 2,
            y_pos + (kTileSize - electric_sprite.getGlobalBounds().height) / 2);
        window_.draw(electric_sprite);

        if (debug_mode) {
          sf::RectangleShape hitbox(sf::Vector2f(kTileSize, kTileSize));
          hitbox.setPosition(x_pos, y_pos);
          hitbox.setFillColor(sf::Color::Transparent);
          hitbox.setOutlineColor(sf::Color::Red);
          hitbox.setOutlineThickness(1.0f);
          window_.draw(hitbox);
        }
      }
    }
  }
}

void GameRenderer::DrawPlayers(const GameState& game_state) {
  const float window_width = window_.getSize().x;
  const float window_height = window_.getSize().y;
  const float kTileSize = 40.0f;

  bool debug_mode = true;

  for (const auto& player : game_state.players_) {
    float x_pos = player.x_ * kTileSize - game_state.map_offset_;
    float y_pos = player.y_ * kTileSize;

    if (x_pos < -kTileSize || x_pos > window_width || y_pos < -kTileSize ||
        y_pos > window_height) {
      continue;
    }

    sf::Sprite player_sprite = player.flying_
                                   ? asset_manager_.GetPlayerFlyingSprite()
                                   : asset_manager_.GetPlayerNormalSprite();

    player_sprite.setTextureRect(sf::IntRect(0, 0, 130, 140));
    player_sprite.setScale(0.3f, 0.3f);

    player_sprite.setPosition(
        x_pos + (kTileSize - player_sprite.getGlobalBounds().width) / 2,
        y_pos + (kTileSize - player_sprite.getGlobalBounds().height) / 2);

    sf::Text id_text;
    id_text.setFont(asset_manager_.GetFont());
    id_text.setString("P" + std::to_string(player.id_));
    id_text.setCharacterSize(14);
    id_text.setFillColor(sf::Color::White);
    id_text.setPosition(x_pos, y_pos - 20);

    window_.draw(player_sprite);
    window_.draw(id_text);

    if (debug_mode) {
      sf::RectangleShape hitbox(sf::Vector2f(kTileSize, kTileSize));
      hitbox.setPosition(x_pos, y_pos);
      hitbox.setFillColor(sf::Color::Transparent);
      hitbox.setOutlineColor(sf::Color::Green);
      hitbox.setOutlineThickness(1.0f);
      window_.draw(hitbox);
    }
  }
}

void GameRenderer::DrawUI(const GameState& game_state) {
  sf::Text status_text;
  status_text.setFont(asset_manager_.GetFont());
  status_text.setCharacterSize(24);
  status_text.setFillColor(sf::Color::White);

  if (!game_state.game_started_) {
    status_text.setString("Waiting for game to start...");
  } else if (game_state.game_ended_) {
    if (game_state.game_result_ == 0) {
      status_text.setString("Game Over: Draw!");
    } else {
      int winner_id = game_state.game_result_ + 1;
      status_text.setString("Game Over: Player " + std::to_string(winner_id) +
                            " wins!");
    }
  } else {
    status_text.setString("Game in progress");
  }

  status_text.setPosition(10, 10);
  window_.draw(status_text);

  float y_pos = 50.0f;
  for (const auto& player : game_state.players_) {
    sf::Text score_text;
    score_text.setFont(asset_manager_.GetFont());
    score_text.setString("Player " + std::to_string(player.id_) + ": " +
                         std::to_string(player.score_) + " coins");
    score_text.setCharacterSize(18);
    score_text.setFillColor(sf::Color::White);
    score_text.setPosition(10, y_pos);
    window_.draw(score_text);
    y_pos += 30.0f;
  }

  if (game_state.game_ended_) {
    sf::Text game_over_text;
    game_over_text.setFont(asset_manager_.GetFont());
    game_over_text.setString("Press ESC to exit");
    game_over_text.setCharacterSize(20);
    game_over_text.setFillColor(sf::Color::White);
    game_over_text.setPosition(
        (window_.getSize().x - game_over_text.getLocalBounds().width) / 2,
        window_.getSize().y - 100);
    window_.draw(game_over_text);
  }
}
