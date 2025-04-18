/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Game renderer implementation
*/

#include "gameRenderer.hpp"
#include <iostream>

GameRenderer::GameRenderer(sf::RenderWindow *win, AssetManager *am)
    : window(win), assetManager(am)
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
    float windowHeight = window->getSize().y;
    float scale =
        windowHeight / backgroundSprite.getLocalBounds().height;
    backgroundSprite.setScale(scale, scale);

    for (int i = 0; i < 3; i++) {
        backgroundSprite.setPosition(
            offset +
                i * backgroundSprite.getLocalBounds().width * scale,
            0);
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

    coinSprite.setTextureRect(sf::IntRect(0, 0, 200, 200));
    electricSprite.setTextureRect(sf::IntRect(0, 0, 100, 100));

    coinSprite.setScale(1.0f / 3.0f, 1.0f / 3.0f);
    electricSprite.setScale(1.0f / 3.0f, 1.0f / 3.0f);

    const float TILE_SIZE = 55.0f;
    const float windowWidth = window->getSize().x;
    const float windowHeight = window->getSize().y;

    size_t startX = gameState.mapOffset / TILE_SIZE;
    size_t endX =
        startX + static_cast<size_t>(windowWidth / TILE_SIZE) + 2;

    for (size_t y = 0; y < gameState.map.size(); y++) {
        float yPos = y * TILE_SIZE + 5;
        if (yPos < -TILE_SIZE || yPos > windowHeight) {
            continue;
        }

        const std::string &row = gameState.map[y];
        for (size_t x = startX; x < endX && x < row.length(); x++) {
            float xPos = x * TILE_SIZE - gameState.mapOffset;

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
    const float windowWidth = window->getSize().x;
    const float windowHeight = window->getSize().y;
    const float TILE_SIZE = 40.0f;

    for (const auto &player : gameState.players) {
        float xPos = player.x * TILE_SIZE - gameState.mapOffset;
        float yPos = player.y * TILE_SIZE;

        if (xPos < -TILE_SIZE || xPos > windowWidth ||
            yPos < -TILE_SIZE || yPos > windowHeight) {
            continue;
        }

        sf::Sprite playerSprite =
            player.flying ? assetManager->getPlayerFlyingSprite()
                          : assetManager->getPlayerNormalSprite();

        playerSprite.setTextureRect(sf::IntRect(0, 0, 130, 140));
        playerSprite.setPosition(xPos, yPos);

        sf::Text idText;
        idText.setFont(assetManager->getFont());
        idText.setString("P" + std::to_string(player.id));
        idText.setCharacterSize(14);
        idText.setFillColor(sf::Color::White);
        idText.setPosition(xPos, yPos * (windowHeight / 10));

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
