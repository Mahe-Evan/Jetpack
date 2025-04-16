/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** handle existing clients
*/

#include <stdbool.h>
#include <unistd.h>
#include "server.h"
#include "jetpack.h"

static void remove_newline(char *restrict str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';
        }
    }
    return;
}

static void handle_client_command(server_t *restrict server, int index,
    player_t *restrict player, bool ready)
{
    ssize_t bytes_read = read(server->fds[index].fd,
        player->command, sizeof(player->command));

    if (bytes_read > 0) {
        remove_newline(player->command);
        receive_command(server, player, index - 2, ready);
    }
    return;
}

void handle_existing_clients(server_t *restrict server, bool ready)
{
    handle_game_logic(server, ready);
    for (int i = 2; server->fds[i].fd != -1 && i < MAX_CLIENTS + 2; i++) {
        if (server->fds[i].revents & POLLIN) {
            handle_client_command(server, i, &server->player[i - 2], ready);
        }
        if (ready && server->game.ready_player[i - 2]) {
            send_command(server, i);
        }
    }
    return;
}
