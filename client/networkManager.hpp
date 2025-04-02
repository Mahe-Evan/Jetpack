/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Network manager for client
*/

#ifndef NETWORK_MANAGER_HPP_
#define NETWORK_MANAGER_HPP_

#include <arpa/inet.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <netinet/in.h>
#include <poll.h>
#include <queue>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

enum JetpackCommand {
    ID,
    MAP,
    READY,
    START,
    PLAYER,
    FLY,
    COIN,
    END
};

class NetworkManager {
  public:
    NetworkManager();
    ~NetworkManager();

    bool connect(const std::string &host, int port);

    void disconnect();

    bool sendReady();

    bool sendFly(bool activate);

    std::vector<std::string> getMap();

    struct PlayerPosition {
        int playerId;
        float x;
        float y;
        int score;
        bool flying;
    };

    std::vector<PlayerPosition> getPlayerPositions();

    struct Coin {
        int x;
        int y;
    };

    std::vector<Coin> getCoins();

    bool isConnected() const;

    bool hasGameStarted() const;

    bool hasGameEnded() const;

    int getGameResult() const;

    int getClientId() const;

    void setDebugMode(bool enable);

  private:
    void networkThread();

    void processData(const std::string &data);

    bool sendCommand(const std::string &command);

    void debugPrint(
        const std::string &direction, const std::string &message);

    int socketFd;
    std::thread networkThreadHandle;
    std::atomic<bool> running;
    std::atomic<bool> connected;
    std::atomic<bool> gameStarted;
    std::atomic<bool> gameEnded;
    std::atomic<int> clientId;
    std::atomic<int> gameResult;
    std::atomic<bool> debugMode;

    std::mutex dataMutex;
    std::vector<std::string> mapData;
    std::vector<PlayerPosition> playerPositions;
    std::vector<Coin> coins;

    std::mutex commandMutex;
    std::queue<std::string> commandQueue;
    std::condition_variable commandCV;

    std::map<std::string, std::function<void(const std::string &)>>
        responseHandlers;

    struct pollfd pollFd;

    static constexpr size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    std::string partialMessage;
};

#endif /* !NETWORK_MANAGER_HPP_ */
