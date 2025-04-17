/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Network manager implementation for client
*/

#include "networkManager.hpp"
#include <errno.h>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string.h>

NetworkManager::NetworkManager()
    : socketFd(-1), running(false), connected(false),
      gameStarted(false), gameEnded(false), clientId(-1),
      gameResult(0), debugMode(false)
{
    memset(buffer, 0, BUFFER_SIZE);

    responseHandlers["ID"] = [this](const std::string &line) {
        int id;
        if (sscanf(line.c_str(), "ID %d", &id) == 1) {
            clientId = id;
            sendCommand("OK");
        } else {
            sendCommand("ERROR Invalid ID");
        }
    };

    responseHandlers["MAP"] = [this](const std::string &line) {
        int length;
        size_t startPos = line.find(" ") + 1;
        if (startPos != std::string::npos) {
            std::string lengthStr = line.substr(startPos);
            size_t secondSpace = lengthStr.find(" ");

            if (secondSpace != std::string::npos) {
                length = std::stoi(lengthStr.substr(0, secondSpace));
                std::string rawMap =
                    lengthStr.substr(secondSpace + 1);

                std::vector<std::string> newMap;
                for (int i = 0; i < 10; i++) {
                    if ((i + 1) * length <=
                        static_cast<int>(rawMap.length())) {
                        newMap.push_back(
                            rawMap.substr(i * length, length));
                    } else {
                        sendCommand("ERROR Map length mismatch");
                        return;
                    }
                }

                for (const auto &mapLine : newMap) {
                    for (char c : mapLine) {
                        if (c != '_' && c != 'c' && c != 'e') {
                            std::string errorMsg =
                                "ERROR Character not recognized: '";
                            errorMsg.push_back(c);
                            errorMsg.push_back('\'');
                            sendCommand(errorMsg);
                            return;
                        }
                    }
                }

                {
                    std::lock_guard<std::mutex> lock(dataMutex);
                    mapData = newMap;
                }

                sendCommand("OK");
            } else {
                sendCommand("ERROR Invalid MAP format");
            }
        } else {
            sendCommand("ERROR Invalid MAP format");
        }
    };

    responseHandlers["START"] = [this](const std::string & /*line*/) {
        gameStarted = true;
        sendCommand("OK");
    };

    responseHandlers["PLAYER"] = [this](const std::string &line) {
        int id, score, flyStatus;
        float x, y;
        if (sscanf(line.c_str(), "PLAYER %d %f %f %d %d", &id, &x, &y,
                &score, &flyStatus) == 5) {
            PlayerPosition pos = {id, x, y, score, flyStatus == 1};

            {
                std::lock_guard<std::mutex> lock(dataMutex);
                bool found = false;
                for (auto &p : playerPositions) {
                    if (p.playerId == id) {
                        p = pos;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    playerPositions.push_back(pos);
                }
            }

            sendCommand("OK");
        } else {
            sendCommand("ERROR Invalid number of data");
        }
    };

    responseHandlers["COIN"] = [this](const std::string &line) {
        int id, x, y;
        if (sscanf(line.c_str(), "COIN %d %d %d", &id, &x, &y) == 3) {
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                coins.push_back({x, y});
            }

            sendCommand("OK");
        } else {
            sendCommand("ERROR Invalid number of data");
        }
    };

    responseHandlers["END"] = [this](const std::string &line) {
        int result;
        if (sscanf(line.c_str(), "END %d", &result) == 1) {
            gameEnded = true;
            gameResult = result;

            sendCommand("OK");
        } else {
            sendCommand("ERROR Invalid game result");
        }
    };
}

NetworkManager::~NetworkManager()
{
    disconnect();
}

bool NetworkManager::connect(const std::string &host, int port)
{
    if (connected) {
        return true;
    }

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        std::cerr << "Error creating socket: " << strerror(errno)
                  << std::endl;
        return false;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported: "
                  << strerror(errno) << std::endl;
        close(socketFd);
        socketFd = -1;
        return false;
    }

    if (::connect(socketFd, (struct sockaddr *)&serverAddr,
            sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed: " << strerror(errno)
                  << std::endl;
        close(socketFd);
        socketFd = -1;
        return false;
    }

    pollFd.fd = socketFd;
    pollFd.events = POLLIN;

    running = true;
    connected = true;
    networkThreadHandle =
        std::thread(&NetworkManager::networkThread, this);

    return true;
}

void NetworkManager::disconnect()
{
    if (!connected) {
        return;
    }

    running = false;

    {
        std::lock_guard<std::mutex> lock(commandMutex);
        commandQueue.push("QUIT");
    }
    commandCV.notify_one();

    if (networkThreadHandle.joinable()) {
        networkThreadHandle.join();
    }

    if (socketFd >= 0) {
        close(socketFd);
        socketFd = -1;
    }

    connected = false;
    gameStarted = false;
    gameEnded = false;
}

bool NetworkManager::sendReady()
{
    return sendCommand("READY");
}

bool NetworkManager::sendFly(bool activate)
{
    std::string command = "FLY " + std::to_string(activate ? 1 : 0);
    return sendCommand(command);
}

std::vector<std::string> NetworkManager::getMap()
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return mapData;
}

std::vector<NetworkManager::PlayerPosition>
NetworkManager::getPlayerPositions()
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return playerPositions;
}

std::vector<NetworkManager::Coin> NetworkManager::getCoins()
{
    std::lock_guard<std::mutex> lock(dataMutex);
    return coins;
}

bool NetworkManager::isConnected() const
{
    return connected;
}

bool NetworkManager::hasGameStarted() const
{
    return gameStarted;
}

bool NetworkManager::hasGameEnded() const
{
    return gameEnded;
}

int NetworkManager::getGameResult() const
{
    return gameResult;
}

int NetworkManager::getClientId() const
{
    return clientId;
}

void NetworkManager::setDebugMode(bool enable)
{
    debugMode = enable;
}

bool NetworkManager::sendCommand(const std::string &command)
{
    if (!connected) {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(commandMutex);
        commandQueue.push(command);
    }
    commandCV.notify_one();

    return true;
}

void NetworkManager::debugPrint(
    const std::string &direction, const std::string &message)
{
    if (debugMode) {
        std::cout << "[" << direction << "] \"" << message << "\""
                  << std::endl;
    }
}

void NetworkManager::networkThread()
{
    while (running) {
        {
            std::unique_lock<std::mutex> lock(commandMutex);

            commandCV.wait_for(lock, std::chrono::milliseconds(100),
                [this] { return !commandQueue.empty() || !running; });

            while (!commandQueue.empty()) {
                std::string cmd = commandQueue.front();
                commandQueue.pop();

                if (!running && cmd == "QUIT") {
                    continue;
                }

                debugPrint("CLIENT", cmd);

                cmd += "\r\n";

                ssize_t bytesWritten =
                    write(socketFd, cmd.c_str(), cmd.length());
                if (bytesWritten < 0) {
                    std::cerr << "Error sending command: "
                              << strerror(errno) << std::endl;
                    running = false;
                    connected = false;
                    break;
                }
            }
        }

        int pollResult = poll(&pollFd, 1, 0);

        if (pollResult < 0) {
            std::cerr << "Poll error: " << strerror(errno)
                      << std::endl;
            break;
        } else if (pollResult > 0 && (pollFd.revents & POLLIN)) {
            ssize_t bytesRead =
                read(socketFd, buffer, BUFFER_SIZE - 1);

            if (bytesRead < 0) {
                std::cerr << "Error reading from socket: "
                          << strerror(errno) << std::endl;
                break;
            } else if (bytesRead == 0) {
                std::cout << "Server closed connection" << std::endl;
                connected = false;
                break;
            } else {
                buffer[bytesRead] = '\0';

                partialMessage.append(buffer, bytesRead);

                size_t pos;
                while ((pos = partialMessage.find("\r\n")) !=
                       std::string::npos) {
                    std::string message =
                        partialMessage.substr(0, pos);
                    partialMessage = partialMessage.substr(pos + 2);

                    debugPrint("SERVER", message);

                    processData(message);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (socketFd >= 0) {
        close(socketFd);
        socketFd = -1;
    }

    connected = false;
}

void NetworkManager::processData(const std::string &data)
{
    if (data.empty()) {
        return;
    }

    std::istringstream iss(data);
    std::string command;
    iss >> command;

    std::cout << "Received command: " << command << std::endl;

    auto it = responseHandlers.find(command);
    if (it != responseHandlers.end()) {
        it->second(data);
    } else {
        if (command == "OK" || command.substr(0, 5) == "ERROR") {
        } else {
            std::cerr << "Unknown command received: " << command
                      << std::endl;
        }
    }
}
