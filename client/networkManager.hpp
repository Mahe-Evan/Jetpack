/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Network manager definition
*/

#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include "commandHandler.hpp"
#include <string>

#define MAX_BUFFER_SIZE 4096

class NetworkManager {
  public:
    NetworkManager(const std::string &host, int port);
    ~NetworkManager();

    void run();

  private:
    bool connect();
    void mainLoop();
    void sendFlyStatus();
    void sendCommand(const std::string &command);
    std::string receiveResponse();

    std::string host;
    int port;
    int sockfd;
    CommandHandler commandHandler;
    bool lastFlyStatus;
};

#endif      // NETWORK_MANAGER_HPP
