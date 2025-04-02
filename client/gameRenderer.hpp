/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Game renderer definition
*/

#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include "assetManager.hpp"
#include <SFML/Graphics.hpp>

class GameRenderer {
  public:
    GameRenderer();
    ~GameRenderer();

    void run();

  private:
    void processEvents();
    void updateGameState(float dt);
    void render();

    void drawBackground();
    void drawMap();
    void drawPlayers();
    void drawUI();

    sf::RenderWindow window;
    sf::Clock clock;
    AssetManager assetManager;

    sf::Text statusText;
    sf::Text scoreText;
    sf::Text resultText;
};

#endif      // GAME_RENDERER_HPP
