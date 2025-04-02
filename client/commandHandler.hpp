/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Command handler definition
*/

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>

class CommandHandler {
  public:
    CommandHandler() = default;

    void handleCommand(const std::string &command);

  private:
    void handleID(std::istringstream &ss);
    void handleMAP(std::istringstream &ss);
    void handleSTART();
    void handlePLAYER(std::istringstream &ss);
    void handleCOIN(std::istringstream &ss);
    void handleEND(std::istringstream &ss);
};

#endif      // COMMAND_HANDLER_HPP
