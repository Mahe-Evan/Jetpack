/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game renderer for client
*/

#ifndef GAME_RENDERER_H_
#define GAME_RENDERER_H_

#include <SFML/Graphics.hpp>

#include "assetManager.hpp"
#include "gameState.hpp"

class GameRenderer {
 public:
  GameRenderer(sf::RenderWindow& window, AssetManager& asset_manager);
  ~GameRenderer() = default;

  void Render(const GameState& game_state);

 private:
  sf::RenderWindow& window_;
  AssetManager& asset_manager_;

  void DrawBackground(const GameState& game_state);
  void DrawMap(const GameState& game_state);
  void DrawPlayers(const GameState& game_state);
  void DrawUI(const GameState& game_state);
};

#endif  // GAME_RENDERER_H_
