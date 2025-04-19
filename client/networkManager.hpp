/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Network manager for client
*/

#ifndef NETWORK_MANAGER_H_
#define NETWORK_MANAGER_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

// Enum names are CamelCase in Google style, but enum values use kConstantStyle
enum JetpackCommand { kId, kMap, kReady, kStart, kPlayer, kFly, kCoin, kEnd };

// Nested structures moved outside the class for better organization
struct PlayerPosition {
  int player_id;
  float x;
  float y;
  int score;
  bool flying;
};

struct Coin {
  int x;
  int y;
};

class NetworkManager {
 public:
  NetworkManager();
  ~NetworkManager();

  bool Connect(const std::string& host, int port);
  void Disconnect();
  bool SendReady();
  bool SendFly(bool activate);

  std::vector<std::string> GetMap();
  std::vector<PlayerPosition> GetPlayerPositions();
  std::vector<Coin> GetCoins();

  bool IsConnected() const;
  bool HasGameStarted() const;
  bool HasGameEnded() const;
  int GetGameResult() const;
  int GetClientId() const;

  void SetDebugMode(bool enable);

 private:
  void NetworkThread();
  void ProcessData(const std::string& data);
  bool SendCommand(const std::string& command);
  void DebugPrint(const std::string& direction, const std::string& message);

  int socket_fd_;
  std::thread network_thread_handle_;
  std::atomic<bool> running_;
  std::atomic<bool> connected_;
  std::atomic<bool> game_started_;
  std::atomic<bool> game_ended_;
  std::atomic<int> client_id_;
  std::atomic<int> game_result_;
  std::atomic<bool> debug_mode_;

  std::mutex data_mutex_;
  std::vector<std::string> map_data_;
  std::vector<PlayerPosition> player_positions_;
  std::vector<Coin> coins_;

  std::mutex command_mutex_;
  std::queue<std::string> command_queue_;
  std::condition_variable command_cv_;

  std::map<std::string, std::function<void(const std::string&)>>
      response_handlers_;

  struct pollfd poll_fd_;

  static constexpr size_t kBufferSize = 4096;
  char buffer_[kBufferSize];
  std::string partial_message_;
};

#endif  // NETWORK_MANAGER_H_
