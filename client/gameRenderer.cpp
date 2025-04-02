/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Game renderer implementation
*/

#include "gameRenderer.hpp"
#include "gameState.hpp"
#include <iostream>

GameRenderer::GameRenderer()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
          "Jetpack Client")
{
    window.setFramerateLimit(60);

    const sf::Font &font = assetManager.getFont();

    statusText.setFont(font);
    scoreText.setFont(font);
    resultText.setFont(font);

    statusText.setCharacterSize(24);
    scoreText.setCharacterSize(24);
    resultText.setCharacterSize(36);

    statusText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);
    resultText.setFillColor(sf::Color::Yellow);

    statusText.setPosition(10, 10);
    scoreText.setPosition(10, 40);
    resultText.setPosition(
        WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 18);
}

GameRenderer::~GameRenderer()
{}

void GameRenderer::run()
{
    while (running && window.isOpen()) {
        processEvents();

        float dt = clock.restart().asSeconds();

        updateGameState(dt);
        render();

        sf::sleep(sf::milliseconds(16));
    }
}

void GameRenderer::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            running = false;
        }
    }

    bool flyRequested =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    {
        std::lock_guard<std::mutex> lock(gameState.stateMutex);
        gameState.flyStatus = flyRequested;
    }
}

void GameRenderer::updateGameState(float dt)
{
    std::lock_guard<std::mutex> lock(gameState.stateMutex);

    if (gameState.gameStarted && !gameState.gameEnded) {
        gameState.mapOffset += SCROLL_SPEED * dt * 60.0f;
    }
}

void GameRenderer::render()
{
    window.clear(sf::Color::Black);

    std::lock_guard<std::mutex> lock(gameState.stateMutex);

    drawBackground();
    drawMap();
    drawPlayers();
    drawUI();

    window.display();
}

void GameRenderer::drawBackground()
{
    const sf::Texture &bgTexture =
        assetManager.getBackgroundTexture();
    sf::Sprite backgroundSprite(bgTexture);

    float bgWidth = backgroundSprite.getLocalBounds().width;
    for (int i = 0; i < WINDOW_WIDTH / bgWidth + 2; i++) {
        backgroundSprite.setPosition(
            -static_cast<int>(gameState.mapOffset) %
                    static_cast<int>(bgWidth) +
                i * bgWidth,
            0);
        window.draw(backgroundSprite);
    }
}

void GameRenderer::drawMap()
{
    const sf::Texture &coinTexture = assetManager.getCoinTexture();
    const sf::Texture &electricTexture =
        assetManager.getElectricTexture();

    sf::Sprite coinSprite(coinTexture);
    sf::Sprite electricSprite(electricTexture);

    float tileSize = 32.0f;
    for (size_t y = 0; y < gameState.map.size(); y++) {
        for (size_t x = 0; x < gameState.map[y].size(); x++) {
            float xPos = x * tileSize - gameState.mapOffset;
            float yPos = y * tileSize;

            if (xPos >= -tileSize && xPos <= WINDOW_WIDTH) {
                if (gameState.map[y][x] == COIN) {
                    coinSprite.setPosition(xPos, yPos);
                    window.draw(coinSprite);
                } else if (gameState.map[y][x] == ELECTRIC) {
                    electricSprite.setPosition(xPos, yPos);
                    window.draw(electricSprite);
                }
            }
        }
    }
}

void GameRenderer::drawPlayers()
{
    for (const auto &player : gameState.players) {
        float playerX = 100;
        float playerY = player.y * 50;

        PlayerState state =
            player.flying ? PLAYER_FLYING : PLAYER_NORMAL;
        sf::Sprite playerSprite(assetManager.getPlayerTexture(state));

        playerSprite.setPosition(playerX, playerY);

        if (player.id == gameState.localPlayerId) {
            playerSprite.setColor(sf::Color::White);
        } else {
            playerSprite.setColor(sf::Color(200, 200, 200));
        }

        window.draw(playerSprite);
    }
}

void GameRenderer::drawUI()
{
    std::string statusString;
    if (!gameState.gameStarted) {
        statusString = "Waiting for game to start...";
    } else if (gameState.gameEnded) {
        statusString = "Game over";
    } else {
        statusString = "Playing";
    }

    statusText.setString(statusString);
    window.draw(statusText);

    std::string scoreString = "Scores: ";
    for (const auto &player : gameState.players) {
        scoreString += "Player " + std::to_string(player.id) + ": " +
                       std::to_string(player.score) + "   ";
    }
    scoreText.setString(scoreString);
    window.draw(scoreText);

    if (gameState.gameEnded) {
        std::string resultString;
        if (gameState.gameResult == 0) {
            resultString = "Game ended in a draw!";
        } else if (gameState.gameResult == gameState.localPlayerId) {
            resultString = "You won the game!";
        } else {
            resultString = "Player " +
                           std::to_string(gameState.gameResult) +
                           " won the game!";
        }
        resultText.setString(resultString);
        window.draw(resultText);
    }
}
