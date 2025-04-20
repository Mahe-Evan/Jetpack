/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** check and call the commands
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "jetpack.h"
#include "server.h"

void receive_command(server_t *restrict server,
    player_t *restrict player, int client_index, bool ready)
{
    char is_flying = '0';

    if (sscanf(player->command, "FLY %c", &is_flying) == 1) {
        player->is_flying = is_flying - '0';
        if (server->debug_mode)
            printf("FLY %c FROM %d\n", is_flying, client_index);
        write(server->fds[client_index + 2].fd, "OK\r\n", 4);
        return;
    }
    if (!ready && strcmp(player->command, "READY") == 0) {
        server->game.ready_player[client_index] = true;
        if (server->debug_mode)
            printf("READY FROM %d\n", client_index);
        write(server->fds[client_index + 2].fd, "OK\r\n", 4);
        return;
    }
    if (server->debug_mode)
        printf("[SERVER]: %s\n", player->command);
    return;
}
