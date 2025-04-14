/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Command handler for client input
*/

#ifndef COMMAND_HANDLER_HPP_
#define COMMAND_HANDLER_HPP_

#include "networkManager.hpp"
#include <SFML/Graphics.hpp>

class CommandHandler {
  public:
    CommandHandler(NetworkManager *networkManager);
    ~CommandHandler() = default;

    void handleEvent(const sf::Event &event);

    void updateInput(float deltaTime);

  private:
    NetworkManager *networkManager;

    bool isJetpackActive;
    bool isLeftPressed;
    bool isRightPressed;
    bool isReadySent;

    float jetpackCooldown;
    float movementCooldown;

    static constexpr float JETPACK_COMMAND_INTERVAL = 0.1f;
    static constexpr float MOVEMENT_COMMAND_INTERVAL = 0.1f;
};

#endif /* !COMMAND_HANDLER_HPP_ */
