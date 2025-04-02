/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Command handler implementation
*/

#include "commandHandler.hpp"
#include <iostream>

CommandHandler::CommandHandler(NetworkManager *networkManager)
    : networkManager(networkManager), isJetpackActive(false),
      isLeftPressed(false), isRightPressed(false),
      jetpackCooldown(0.0f), movementCooldown(0.0f),
      isReadySent(false)
{}

void CommandHandler::handleEvent(const sf::Event &event)
{
    if (!isReadySent && networkManager->isConnected() &&
        networkManager->getClientId() >= 0) {
        if (event.type == sf::Event::KeyPressed) {
            networkManager->sendReady();
            isReadySent = true;
            return;
        }
    }

    if (!networkManager->hasGameStarted() ||
        networkManager->hasGameEnded()) {
        return;
    }

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Space:
                if (!isJetpackActive) {
                    isJetpackActive = true;
                    networkManager->sendFly(true);
                }
                break;
            case sf::Keyboard::Left:
                isLeftPressed = true;
                break;
            case sf::Keyboard::Right:
                isRightPressed = true;
                break;
            default:
                break;
        }
    }

    else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::Space:
                isJetpackActive = false;
                networkManager->sendFly(false);
                break;
            case sf::Keyboard::Left:
                isLeftPressed = false;
                break;
            case sf::Keyboard::Right:
                isRightPressed = false;
                break;
            case sf::Keyboard::R:
                if (!networkManager->hasGameStarted()) {
                    networkManager->sendReady();
                    isReadySent = true;
                }
                break;
            default:
                break;
        }
    }
}

void CommandHandler::updateInput(float deltaTime)
{
    if (!networkManager->hasGameStarted() ||
        networkManager->hasGameEnded()) {
        return;
    }

    jetpackCooldown -= deltaTime;
    movementCooldown -= deltaTime;

    if (isJetpackActive && jetpackCooldown <= 0.0f) {
        networkManager->sendFly(true);
        jetpackCooldown = JETPACK_COMMAND_INTERVAL;
    }

    if ((isLeftPressed || isRightPressed) &&
        movementCooldown <= 0.0f) {
        movementCooldown = MOVEMENT_COMMAND_INTERVAL;
    }
}
