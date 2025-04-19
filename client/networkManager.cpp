/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Network manager implementation for client
*/

#include "networkManager.hpp"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <functional>
#include <iostream>
#include <sstream>

NetworkManager::NetworkManager()
    : socket_fd_(-1),
      running_(false),
      connected_(false),
      game_started_(false),
      game_ended_(false),
      client_id_(-1),
      game_result_(0),
      debug_mode_(false) {
  memset(buffer_, 0, kBufferSize);

  response_handlers_["ID"] = [this](const std::string& line) {
    int id;
    if (sscanf(line.c_str(), "ID %d", &id) == 1) {
      client_id_ = id;
      std::cout << "Client ID: " << client_id_ << std::endl;
      SendCommand("OK");
    } else {
      SendCommand("ERROR Invalid ID");
    }
  };

  response_handlers_["MAP"] = [this](const std::string& line) {
    int length;
    size_t start_pos = line.find(" ") + 1;
    if (start_pos != std::string::npos) {
      std::string length_str = line.substr(start_pos);
      size_t second_space = length_str.find(" ");

      if (second_space != std::string::npos) {
        length = std::stoi(length_str.substr(0, second_space));
        std::string raw_map = length_str.substr(second_space + 1);

        std::vector<std::string> new_map;
        for (int i = 0; i < 10; i++) {
          if ((i + 1) * length <= static_cast<int>(raw_map.length())) {
            new_map.push_back(raw_map.substr(i * length, length));
          } else {
            SendCommand("ERROR Map length mismatch");
            return;
          }
        }

        for (const auto& map_line : new_map) {
          for (char c : map_line) {
            if (c != '_' && c != 'c' && c != 'e') {
              std::string error_msg = "ERROR Character not recognized: '";
              error_msg.push_back(c);
              error_msg.push_back('\'');
              SendCommand(error_msg);
              return;
            }
          }
        }

        {
          std::lock_guard<std::mutex> lock(data_mutex_);
          map_data_ = new_map;
        }

        SendCommand("OK");
      } else {
        SendCommand("ERROR Invalid MAP format");
      }
    } else {
      SendCommand("ERROR Invalid MAP format");
    }
  };

  response_handlers_["START"] = [this](const std::string& /*line*/) {
    game_started_ = true;
    SendCommand("OK");
  };

  response_handlers_["PLAYER"] = [this](const std::string& line) {
    int id, score, fly_status;
    float x, y;
    if (sscanf(line.c_str(), "PLAYER %d %f %f %d %d", &id, &x, &y, &score,
               &fly_status) == 5) {
      PlayerPosition pos = {id, x, y, score, fly_status == 1};

      {
        std::lock_guard<std::mutex> lock(data_mutex_);
        bool found = false;
        for (auto& p : player_positions_) {
          if (p.player_id == id) {
            p = pos;
            found = true;
            break;
          }
        }

        if (!found) {
          player_positions_.push_back(pos);
        }
      }

      SendCommand("OK");
    } else {
      SendCommand("ERROR Invalid number of data");
    }
  };

  response_handlers_["COIN"] = [this](const std::string& line) {
    int id, x, y;
    if (sscanf(line.c_str(), "COIN %d %d %d", &id, &x, &y) == 3) {
      {
        std::lock_guard<std::mutex> lock(data_mutex_);
        coins_.push_back({x, y});
      }

      SendCommand("OK");
    } else {
      SendCommand("ERROR Invalid number of data");
    }
  };

  response_handlers_["END"] = [this](const std::string& line) {
    int result;
    if (sscanf(line.c_str(), "END %d", &result) == 1) {
      game_ended_ = true;
      game_result_ = result;

      SendCommand("OK");
    } else {
      SendCommand("ERROR Invalid game result");
    }
  };
}

NetworkManager::~NetworkManager() { Disconnect(); }

bool NetworkManager::Connect(const std::string& host, int port) {
  if (connected_) {
    return true;
  }

  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    return false;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address or address not supported: " << strerror(errno)
              << std::endl;
    close(socket_fd_);
    socket_fd_ = -1;
    return false;
  }

  if (::connect(socket_fd_, (struct sockaddr*)&server_addr,
                sizeof(server_addr)) < 0) {
    std::cerr << "Connection failed: " << strerror(errno) << std::endl;
    close(socket_fd_);
    socket_fd_ = -1;
    return false;
  }

  poll_fd_.fd = socket_fd_;
  poll_fd_.events = POLLIN;

  running_ = true;
  connected_ = true;
  network_thread_handle_ = std::thread(&NetworkManager::NetworkThread, this);

  return true;
}

void NetworkManager::Disconnect() {
  if (!connected_) {
    return;
  }

  running_ = false;

  {
    std::lock_guard<std::mutex> lock(command_mutex_);
    command_queue_.push("QUIT");
  }
  command_cv_.notify_one();

  if (network_thread_handle_.joinable()) {
    network_thread_handle_.join();
  }

  if (socket_fd_ >= 0) {
    close(socket_fd_);
    socket_fd_ = -1;
  }

  connected_ = false;
  game_started_ = false;
  game_ended_ = false;
}

bool NetworkManager::SendReady() { return SendCommand("READY"); }

bool NetworkManager::SendFly(bool activate) {
  std::string command = "FLY " + std::to_string(activate ? 1 : 0);
  return SendCommand(command);
}

std::vector<std::string> NetworkManager::GetMap() {
  std::lock_guard<std::mutex> lock(data_mutex_);
  return map_data_;
}

std::vector<PlayerPosition> NetworkManager::GetPlayerPositions() {
  std::lock_guard<std::mutex> lock(data_mutex_);
  return player_positions_;
}

std::vector<Coin> NetworkManager::GetCoins() {
  std::lock_guard<std::mutex> lock(data_mutex_);
  return coins_;
}

bool NetworkManager::IsConnected() const { return connected_; }

bool NetworkManager::HasGameStarted() const { return game_started_; }

bool NetworkManager::HasGameEnded() const { return game_ended_; }

int NetworkManager::GetGameResult() const { return game_result_; }

int NetworkManager::GetClientId() const { return client_id_; }

void NetworkManager::SetDebugMode(bool enable) { debug_mode_ = enable; }

bool NetworkManager::SendCommand(const std::string& command) {
  if (!connected_) {
    return false;
  }

  {
    std::lock_guard<std::mutex> lock(command_mutex_);
    command_queue_.push(command);
  }
  command_cv_.notify_one();

  return true;
}

void NetworkManager::DebugPrint(const std::string& direction,
                                const std::string& message) {
  if (debug_mode_) {
    std::cout << "[" << direction << "] \"" << message << "\"" << std::endl;
  }
}

void NetworkManager::NetworkThread() {
  while (running_) {
    {
      std::unique_lock<std::mutex> lock(command_mutex_);

      command_cv_.wait_for(lock, std::chrono::milliseconds(100), [this] {
        return !command_queue_.empty() || !running_;
      });

      while (!command_queue_.empty()) {
        std::string cmd = command_queue_.front();
        command_queue_.pop();

        if (!running_ && cmd == "QUIT") {
          continue;
        }

        DebugPrint("CLIENT", cmd);

        cmd += "\r\n";

        ssize_t bytes_written = write(socket_fd_, cmd.c_str(), cmd.length());
        if (bytes_written < 0) {
          std::cerr << "Error sending command: " << strerror(errno)
                    << std::endl;
          running_ = false;
          connected_ = false;
          break;
        }
      }
    }

    int poll_result = poll(&poll_fd_, 1, 0);

    if (poll_result < 0) {
      std::cerr << "Poll error: " << strerror(errno) << std::endl;
      break;
    } else if (poll_result > 0 && (poll_fd_.revents & POLLIN)) {
      ssize_t bytes_read = read(socket_fd_, buffer_, kBufferSize - 1);

      if (bytes_read < 0) {
        std::cerr << "Error reading from socket: " << strerror(errno)
                  << std::endl;
        break;
      } else if (bytes_read == 0) {
        std::cout << "Server closed connection" << std::endl;
        connected_ = false;
        break;
      } else {
        buffer_[bytes_read] = '\0';

        partial_message_.append(buffer_, bytes_read);

        size_t pos;
        while ((pos = partial_message_.find("\r\n")) != std::string::npos) {
          std::string message = partial_message_.substr(0, pos);
          partial_message_ = partial_message_.substr(pos + 2);

          DebugPrint("SERVER", message);

          ProcessData(message);
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  if (socket_fd_ >= 0) {
    close(socket_fd_);
    socket_fd_ = -1;
  }

  connected_ = false;
}

void NetworkManager::ProcessData(const std::string& data) {
  if (data.empty()) {
    return;
  }

  std::istringstream iss(data);
  std::string command;
  iss >> command;

  std::cout << "Received command: " << command << std::endl;

  auto it = response_handlers_.find(command);
  if (it != response_handlers_.end()) {
    it->second(data);
  } else {
    if (command == "OK" || command.substr(0, 5) == "ERROR") {
      // Acknowledgment, no need to handle
    } else {
      std::cerr << "Unknown command received: " << command << std::endl;
    }
  }
}
