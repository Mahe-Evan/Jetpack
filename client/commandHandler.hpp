/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Command handler for client input
*/

#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include <SFML/Graphics.hpp>

#include "networkManager.hpp"

class CommandHandler {
 public:
  CommandHandler(NetworkManager* network_manager);
  ~CommandHandler() = default;

  void HandleEvent(const sf::Event& event);
  void UpdateInput(float delta_time);

 private:
  NetworkManager* network_manager_;

  bool is_jetpack_active_;
  bool is_left_pressed_;
  bool is_right_pressed_;
  bool is_ready_sent_;

  float jetpack_cooldown_;
  float movement_cooldown_;

  static constexpr float kJetpackCommandInterval = 0.1f;
  static constexpr float kMovementCommandInterval = 0.1f;
};

#endif  // COMMAND_HANDLER_H_
