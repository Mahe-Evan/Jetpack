/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Network manager implementation
*/

#include "networkManager.hpp"
#include "gameState.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

NetworkManager::NetworkManager(const std::string &host, int port)
    : host(host), port(port), sockfd(-1), lastFlyStatus(false)
{}

NetworkManager::~NetworkManager()
{
    if (sockfd != -1) {
        close(sockfd);
    }
}

void NetworkManager::run()
{
    if (!connect()) {
        running = false;
        cv.notify_all();
        return;
    }

    sendCommand("READY");
    mainLoop();

    close(sockfd);
    running = false;
    cv.notify_all();
}

bool NetworkManager::connect()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <=
        0) {
        std::cerr << "Invalid address" << std::endl;
        close(sockfd);
        return false;
    }

    if (::connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sockfd);
        return false;
    }

    if (debug_mode) {
        std::cout << "Connected to server " << host << ":" << port
                  << std::endl;
    }

    return true;
}

void NetworkManager::mainLoop()
{
    char buffer[MAX_BUFFER_SIZE];

    while (running) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        ssize_t bytes_read =
            recv(sockfd, buffer, MAX_BUFFER_SIZE - 1, 0);

        if (bytes_read <= 0) {
            if (debug_mode) {
                std::cerr << "Server disconnected" << std::endl;
            }
            break;
        }

        std::string data(buffer, bytes_read);
        if (debug_mode) {
            std::cout << "Received: " << data << std::endl;
        }

        std::istringstream stream(data);
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty()) {
                commandHandler.handleCommand(line);
            }
        }

        sendFlyStatus();
    }
}

void NetworkManager::sendFlyStatus()
{
    std::lock_guard<std::mutex> lock(gameState.stateMutex);

    if (gameState.gameStarted && !gameState.gameEnded &&
        gameState.flyStatus != lastFlyStatus) {
        std::string flyCmd =
            "FLY " + std::to_string(gameState.flyStatus ? 1 : 0);
        sendCommand(flyCmd);
        lastFlyStatus = gameState.flyStatus;
    }
}

void NetworkManager::sendCommand(const std::string &command)
{
    if (debug_mode) {
        std::cout << "Sending: " << command << std::endl;
    }

    std::string cmd = command + "\r\n";
    send(sockfd, cmd.c_str(), cmd.length(), 0);

    std::string response = receiveResponse();

    if (debug_mode) {
        std::cout << "Response: " << response << std::endl;
    }
}

std::string NetworkManager::receiveResponse()
{
    char buffer[MAX_BUFFER_SIZE];
    memset(buffer, 0, MAX_BUFFER_SIZE);

    ssize_t bytes_read = recv(sockfd, buffer, MAX_BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) {
        return "";
    }

    return std::string(buffer, bytes_read);
}
