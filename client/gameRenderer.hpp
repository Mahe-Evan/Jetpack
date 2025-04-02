/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game renderer for client
*/

#ifndef GAME_RENDERER_HPP_
#define GAME_RENDERER_HPP_

#include "assetManager.hpp"
#include "gameState.hpp"
#include <SFML/Graphics.hpp>

class GameRenderer {
  public:
    GameRenderer(
        sf::RenderWindow *window, AssetManager *assetManager);
    ~GameRenderer() = default;

    void render(const GameState &gameState);

  private:
    sf::RenderWindow *window;
    AssetManager *assetManager;

    void drawBackground(const GameState &gameState);
    void drawMap(const GameState &gameState);
    void drawPlayers(const GameState &gameState);
    void drawUI(const GameState &gameState);
};

#endif /* !GAME_RENDERER_HPP_ */
