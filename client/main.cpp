/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Client main entry point
*/

// Project headers
#include "assetManager.hpp"
#include "commandHandler.hpp"
#include "gameRenderer.hpp"
#include "gameState.hpp"
#include "networkManager.hpp"

// System headers
#include <cstdlib>
#include <cstring>
#include <iostream>

// Library headers
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Standard library
#include <chrono>
#include <thread>

void PrintUsage(const char* program_name) {
  std::cerr << "Usage: " << program_name << " -h <host> -p <port> [-d]"
            << std::endl;
  std::cerr << "  -h <host>   Server IP address" << std::endl;
  std::cerr << "  -p <port>   Server port" << std::endl;
  std::cerr << "  -d          Enable debug mode" << std::endl;
}

bool ParseCommandLineArgs(int argc, char* argv[], std::string* host, int* port,
                          bool* debug_mode) {
  *host = "127.0.0.1";
  *port = 8080;
  *debug_mode = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
      *host = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      *port = std::atoi(argv[++i]);
    } else if (strcmp(argv[i], "-d") == 0) {
      *debug_mode = true;
    } else {
      PrintUsage(argv[0]);
      return false;
    }
  }

  if (host->empty() || *port <= 0) {
    PrintUsage(argv[0]);
    return false;
  }

  return true;
}

bool InitializeResources(NetworkManager& network_manager,
                         AssetManager& asset_manager, bool debug_mode,
                         const std::string& host, int port) {
  if (debug_mode) {
    network_manager.SetDebugMode(true);
    std::cout << "Debug mode enabled" << std::endl;
  }

  if (!asset_manager.LoadAssets()) {
    std::cerr << "Failed to load assets" << std::endl;
    return false;
  }

  std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;
  if (!network_manager.Connect(host, port)) {
    std::cerr << "Failed to connect to server" << std::endl;
    return false;
  }
  std::cout << "Connected to server" << std::endl;

  return true;
}

bool WaitForPlayerId(NetworkManager& network_manager) {
  std::cout << "Waiting for player ID..." << std::endl;

  while (network_manager.IsConnected() && network_manager.GetClientId() < 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if (!network_manager.IsConnected()) {
    std::cerr << "Connection lost while waiting for player ID" << std::endl;
    return false;
  }

  std::cout << "Assigned player ID: " << network_manager.GetClientId()
            << std::endl;
  std::cout << "Press any key to send READY signal" << std::endl;

  return true;
}

void UpdateWindowTitle(sf::RenderWindow& window,
                       NetworkManager& network_manager) {
  if (!network_manager.HasGameStarted()) {
    window.setTitle("Jetpack Client - Waiting for Start");
    return;
  }

  if (!network_manager.HasGameEnded()) {
    window.setTitle("Jetpack Client - Game Running");
    return;
  }

  int result = network_manager.GetGameResult();
  if (result == 0) {
    window.setTitle("Jetpack Client - Game Over: Draw");
  } else if (result == network_manager.GetClientId()) {
    window.setTitle("Jetpack Client - Game Over: You Win!");
  } else {
    window.setTitle("Jetpack Client - Game Over: You Lose!");
  }
}

void RunGameLoop(NetworkManager& network_manager, GameState& game_state,
                 AssetManager& asset_manager, sf::RenderWindow& window) {
  GameRenderer game_renderer(window, asset_manager);
  CommandHandler command_handler(network_manager);

  sf::Clock clock;
  sf::Clock ready_clock;
  bool ready_sent = false;

  while (window.isOpen()) {
    if (!ready_sent && !network_manager.HasGameStarted() &&
        ready_clock.getElapsedTime().asSeconds() > 3.0f) {
      network_manager.SendReady();
      ready_sent = true;
      std::cout << "Automatically sent READY signal" << std::endl;
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      command_handler.HandleEvent(event);
    }

    command_handler.UpdateInput(clock.restart().asSeconds());

    if (network_manager.IsConnected()) {
      game_state.UpdateFromNetwork(
          network_manager.GetMap(), network_manager.GetPlayerPositions(),
          network_manager.HasGameStarted(), network_manager.HasGameEnded(),
          network_manager.GetGameResult());

      UpdateWindowTitle(window, network_manager);
    } else {
      window.close();
      std::cerr << "Disconnected from server" << std::endl;
      break;
    }

    game_renderer.Render(game_state);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main(int argc, char* argv[]) {
  std::string host;
  int port;
  bool debug_mode;

  if (!ParseCommandLineArgs(argc, argv, &host, &port, &debug_mode)) {
    return 84;
  }

  NetworkManager network_manager;
  GameState game_state;
  AssetManager asset_manager;

  if (!InitializeResources(network_manager, asset_manager, debug_mode, host,
                           port)) {
    return 84;
  }

  if (!WaitForPlayerId(network_manager)) {
    return 84;
  }

  sf::RenderWindow window(sf::VideoMode(800, 600), "Jetpack Client");
  window.setFramerateLimit(60);

  RunGameLoop(network_manager, game_state, asset_manager, window);

  network_manager.Disconnect();

  return 0;
}
