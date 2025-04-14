/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game renderer implementation
*/

#include "gameRenderer.hpp"
#include <iostream>

GameRenderer::GameRenderer(
    sf::RenderWindow *window, AssetManager *assetManager)
    : window(window), assetManager(assetManager)
{}

void GameRenderer::render(const GameState &gameState)
{
    if (!window) {
        return;
    }

    std::lock_guard<std::mutex> lock(gameState.stateMutex);

    window->clear(sf::Color(30, 30, 30));

    drawBackground(gameState);
    drawMap(gameState);
    drawPlayers(gameState);
    drawUI(gameState);

    window->display();
}

void GameRenderer::drawBackground(const GameState &gameState)
{
    sf::Sprite backgroundSprite = assetManager->getBackgroundSprite();

    float offset = -static_cast<float>(gameState.mapOffset);

    for (int i = 0; i < 3; i++) {
        backgroundSprite.setPosition(
            offset + i * backgroundSprite.getLocalBounds().width, 0);
        window->draw(backgroundSprite);
    }
}

void GameRenderer::drawMap(const GameState &gameState)
{
    if (gameState.map.empty()) {
        return;
    }

    sf::Sprite coinSprite = assetManager->getCoinSprite();
    sf::Sprite electricSprite = assetManager->getElectricSprite();

    const float TILE_SIZE = 40.0f;

    for (size_t y = 0; y < gameState.map.size(); y++) {
        const std::string &row = gameState.map[y];
        for (size_t x = 0; x < row.length(); x++) {
            float xPos = x * TILE_SIZE - gameState.mapOffset;
            float yPos = y * TILE_SIZE;

            if (xPos < -TILE_SIZE || xPos > window->getSize().x) {
                continue;
            }

            if (row[x] == 'c') {
                coinSprite.setPosition(xPos, yPos);
                window->draw(coinSprite);
            } else if (row[x] == 'e') {
                electricSprite.setPosition(xPos, yPos);
                window->draw(electricSprite);
            }
        }
    }
}

void GameRenderer::drawPlayers(const GameState &gameState)
{
    for (const auto &player : gameState.players) {
        sf::Sprite playerSprite =
            player.flying ? assetManager->getPlayerFlyingSprite()
                          : assetManager->getPlayerNormalSprite();

        playerSprite.setPosition(
            player.x * 40.0f - gameState.mapOffset, player.y * 40.0f);

        sf::Text idText;
        idText.setFont(assetManager->getFont());
        idText.setString("P" + std::to_string(player.id));
        idText.setCharacterSize(14);
        idText.setFillColor(sf::Color::White);
        idText.setPosition(player.x * 40.0f - gameState.mapOffset,
            player.y * 40.0f - 20.0f);

        window->draw(playerSprite);
        window->draw(idText);
    }
}

void GameRenderer::drawUI(const GameState &gameState)
{
    sf::Text statusText;
    statusText.setFont(assetManager->getFont());
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);

    if (!gameState.gameStarted) {
        statusText.setString("Waiting for game to start...");
    } else if (gameState.gameEnded) {
        if (gameState.gameResult == 0) {
            statusText.setString("Game Over: Draw!");
        } else {
            statusText.setString(
                "Game Over: Player " +
                std::to_string(gameState.gameResult) + " wins!");
        }
    } else {
        statusText.setString("Game in progress");
    }

    statusText.setPosition(10, 10);
    window->draw(statusText);

    float yPos = 50.0f;
    for (const auto &player : gameState.players) {
        sf::Text scoreText;
        scoreText.setFont(assetManager->getFont());
        scoreText.setString("Player " + std::to_string(player.id) +
                            ": " + std::to_string(player.score) +
                            " coins");
        scoreText.setCharacterSize(18);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, yPos);
        window->draw(scoreText);
        yPos += 30.0f;
    }

    if (gameState.gameEnded) {
        sf::Text gameOverText;
        gameOverText.setFont(assetManager->getFont());
        gameOverText.setString("Press ESC to exit");
        gameOverText.setCharacterSize(20);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setPosition(
            (window->getSize().x -
                gameOverText.getLocalBounds().width) /
                2,
            window->getSize().y - 100);
        window->draw(gameOverText);
    }
}
