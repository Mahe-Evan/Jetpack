/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Client main entry point
*/

#include "assetManager.hpp"
#include "commandHandler.hpp"
#include "gameRenderer.hpp"
#include "gameState.hpp"
#include "networkManager.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

void printUsage(const char *programName)
{
    std::cerr << "Usage: " << programName
              << " -h <host> -p <port> [-d]" << std::endl;
    std::cerr << "  -h <host>   Server IP address" << std::endl;
    std::cerr << "  -p <port>   Server port" << std::endl;
    std::cerr << "  -d          Enable debug mode" << std::endl;
}

int main(int argc, char *argv[])
{
    std::string host = "127.0.0.1";
    int port = 8080;
    bool debugMode = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            host = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-d") == 0) {
            debugMode = true;
        } else {
            printUsage(argv[0]);
            return 84;
        }
    }

    if (host.empty() || port <= 0) {
        printUsage(argv[0]);
        return 84;
    }

    NetworkManager networkManager;
    GameState gameState;
    AssetManager assetManager;

    if (debugMode) {
        networkManager.setDebugMode(true);
        std::cout << "Debug mode enabled" << std::endl;
    }

    if (!assetManager.loadAssets()) {
        std::cerr << "Failed to load assets" << std::endl;
        return 84;
    }

    std::cout << "Connecting to " << host << ":" << port << "..."
              << std::endl;
    if (!networkManager.connect(host, port)) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 84;
    }
    std::cout << "Connected to server" << std::endl;

    sf::RenderWindow window(
        sf::VideoMode(800, 600), "Jetpack Client");
    window.setFramerateLimit(60);

    GameRenderer gameRenderer(&window, &assetManager);

    CommandHandler commandHandler(&networkManager);

    std::cout << "Waiting for player ID..." << std::endl;
    while (networkManager.isConnected() &&
           networkManager.getClientId() < 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!networkManager.isConnected()) {
        std::cerr << "Connection lost while waiting for player ID"
                  << std::endl;
        return 84;
    }

    std::cout << "Assigned player ID: "
              << networkManager.getClientId() << std::endl;
    std::cout << "Press any key to send READY signal" << std::endl;

    sf::Clock clock;
    sf::Clock readyClock;
    bool readySent = false;

    while (window.isOpen()) {

        if (!readySent && !networkManager.hasGameStarted() &&
            readyClock.getElapsedTime().asSeconds() > 3.0f) {
            networkManager.sendReady();
            readySent = true;
            std::cout << "Automatically sent READY signal"
                      << std::endl;
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            commandHandler.handleEvent(event);
        }

        commandHandler.updateInput(clock.restart().asSeconds());

        if (networkManager.isConnected()) {
            gameState.updateFromNetwork(networkManager.getMap(),
                networkManager.getPlayerPositions(),
                networkManager.hasGameStarted(),
                networkManager.hasGameEnded(),
                networkManager.getGameResult());

            if (networkManager.hasGameStarted()) {
                window.setTitle("Jetpack Client - Game Running");
            } else {
                window.setTitle("Jetpack Client - Waiting for Start");
            }

            if (networkManager.hasGameEnded()) {
                int result = networkManager.getGameResult();
                if (result == 0) {
                    window.setTitle(
                        "Jetpack Client - Game Over: Draw");
                } else if (result == networkManager.getClientId()) {
                    window.setTitle(
                        "Jetpack Client - Game Over: You Win!");
                } else {
                    window.setTitle(
                        "Jetpack Client - Game Over: You Lose!");
                }
            }
        } else {
            window.close();
            std::cerr << "Disconnected from server" << std::endl;
            break;
        }

        gameRenderer.render(gameState);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    networkManager.disconnect();

    return 0;
}
