/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Command handler implementation
*/

#include "commandHandler.hpp"
#include "gameState.hpp"
#include <iostream>
#include <sstream>

void CommandHandler::handleCommand(const std::string &command)
{
    std::istringstream ss(command);
    std::string cmd;
    ss >> cmd;

    std::lock_guard<std::mutex> lock(gameState.stateMutex);

    if (cmd == "ID") {
        handleID(ss);
    } else if (cmd == "MAP") {
        handleMAP(ss);
    } else if (cmd == "START") {
        handleSTART();
    } else if (cmd == "PLAYER") {
        handlePLAYER(ss);
    } else if (cmd == "COIN") {
        handleCOIN(ss);
    } else if (cmd == "END") {
        handleEND(ss);
    }
}

void CommandHandler::handleID(std::istringstream &ss)
{
    int id;
    ss >> id;
    gameState.localPlayerId = id;

    if (gameState.getPlayerById(id) == nullptr) {
        Player newPlayer(id);
        gameState.players.push_back(newPlayer);
    }

    if (debug_mode) {
        std::cout << "Set player ID to " << id << std::endl;
    }
}

void CommandHandler::handleMAP(std::istringstream &ss)
{
    int length;
    std::string rawMap;
    ss >> length;
    ss >> rawMap;

    gameState.map.clear();
    for (int i = 0; i < 10; i++) {
        if (i * length >= (int)rawMap.size()) {
            break;
        }
        gameState.map.push_back(rawMap.substr(i * length, length));
    }

    if (debug_mode) {
        std::cout << "Received map of length " << length << std::endl;
        for (const auto &line : gameState.map) {
            std::cout << line << std::endl;
        }
    }
}

void CommandHandler::handleSTART()
{
    gameState.gameStarted = true;
    gameState.gameEnded = false;

    if (debug_mode) {
        std::cout << "Game started" << std::endl;
    }
}

void CommandHandler::handlePLAYER(std::istringstream &ss)
{
    int id;
    float x, y;
    int score;
    int flyStatus;
    ss >> id >> x >> y >> score >> flyStatus;

    Player *player = gameState.getPlayerById(id);
    if (player == nullptr) {
        Player newPlayer(id, x, y);
        newPlayer.score = score;
        newPlayer.flying = flyStatus != 0;
        gameState.players.push_back(newPlayer);
    } else {
        player->x = x;
        player->y = y;
        player->score = score;
        player->flying = flyStatus != 0;
    }

    if (debug_mode) {
        std::cout << "Updated player " << id << " at position (" << x
                  << ", " << y << ") with score " << score
                  << " and flying status " << flyStatus << std::endl;
    }
}

void CommandHandler::handleCOIN(std::istringstream &ss)
{
    int playerId, coinX, coinY;
    ss >> playerId >> coinX >> coinY;

    if (debug_mode) {
        std::cout << "Player " << playerId << " collected coin at ("
                  << coinX << ", " << coinY << ")" << std::endl;
    }
}

void CommandHandler::handleEND(std::istringstream &ss)
{
    int result;
    ss >> result;

    gameState.gameEnded = true;
    gameState.gameResult = result;

    if (debug_mode) {
        if (result == 0) {
            std::cout << "Game ended in a draw" << std::endl;
        } else {
            std::cout << "Player " << result << " won the game"
                      << std::endl;
        }
    }
}
