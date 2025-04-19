/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Command handler implementation
*/

#include "commandHandler.hpp"

#include <iostream>

CommandHandler::CommandHandler(NetworkManager* nmgr)
    : network_manager_(nmgr),
      is_jetpack_active_(false),
      is_left_pressed_(false),
      is_right_pressed_(false),
      is_ready_sent_(false),
      jetpack_cooldown_(0.0f),
      movement_cooldown_(0.0f) {}

void CommandHandler::HandleEvent(const sf::Event& event) {
  if (!is_ready_sent_ && network_manager_->IsConnected() &&
      network_manager_->GetClientId() >= 0) {
    if (event.type == sf::Event::KeyPressed) {
      network_manager_->SendReady();
      is_ready_sent_ = true;
      return;
    }
  }

  if (!network_manager_->HasGameStarted() || network_manager_->HasGameEnded()) {
    return;
  }

  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
      case sf::Keyboard::Space:
        if (!is_jetpack_active_) {
          is_jetpack_active_ = true;
          network_manager_->SendFly(true);
        }
        break;
      case sf::Keyboard::Left:
        is_left_pressed_ = true;
        break;
      case sf::Keyboard::Right:
        is_right_pressed_ = true;
        break;
      default:
        break;
    }
  }

  else if (event.type == sf::Event::KeyReleased) {
    switch (event.key.code) {
      case sf::Keyboard::Space:
        is_jetpack_active_ = false;
        network_manager_->SendFly(false);
        break;
      case sf::Keyboard::Left:
        is_left_pressed_ = false;
        break;
      case sf::Keyboard::Right:
        is_right_pressed_ = false;
        break;
      case sf::Keyboard::R:
        if (!network_manager_->HasGameStarted()) {
          network_manager_->SendReady();
          is_ready_sent_ = true;
        }
        break;
      default:
        break;
    }
  }
}

void CommandHandler::UpdateInput(float delta_time) {
  if (!network_manager_->HasGameStarted() || network_manager_->HasGameEnded()) {
    return;
  }

  jetpack_cooldown_ -= delta_time;
  movement_cooldown_ -= delta_time;

  if (is_jetpack_active_ && jetpack_cooldown_ <= 0.0f) {
    network_manager_->SendFly(true);
    jetpack_cooldown_ = kJetpackCommandInterval;
  }

  if ((is_left_pressed_ || is_right_pressed_) && movement_cooldown_ <= 0.0f) {
    movement_cooldown_ = kMovementCommandInterval;
  }
}
