/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Main client entry point
*/

#include "gameRenderer.hpp"
#include "gameState.hpp"
#include "networkManager.hpp"
#include <iostream>
#include <thread>

GameState gameState;
std::atomic<bool> running(true);
std::condition_variable cv;
bool debug_mode = false;

int main(int argc, char **argv)
{
    std::string host = "127.0.0.1";
    int port = 4242;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h" && i + 1 < argc) {
            host = argv[i + 1];
            i++;
        } else if (std::string(argv[i]) == "-p" && i + 1 < argc) {
            port = std::stoi(argv[i + 1]);
            i++;
        } else if (std::string(argv[i]) == "-d") {
            debug_mode = true;
        }
    }

    if (debug_mode) {
        std::cout << "Debug mode enabled" << std::endl;
        std::cout << "Connecting to " << host << ":" << port
                  << std::endl;
    }

    NetworkManager networkManager(host, port);
    GameRenderer renderer;

    std::thread networkThread(&NetworkManager::run, &networkManager);
    std::thread renderThread(&GameRenderer::run, &renderer);

    networkThread.join();
    renderThread.join();

    return 0;
}
