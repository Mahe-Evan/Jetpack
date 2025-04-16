/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** check and call the commands
*/

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include "server.h"
#include "jetpack.h"

void receive_command(server_t *restrict server, player_t *restrict player,
    int client_index, bool ready)
{
    char is_flying = '0';

    if (strlen(player->command) == 5 &&
        sscanf(player->command, "FLY %c", &is_flying) == 1) {
        player->is_flying = is_flying - '0';
        printf("FLY %c FROM %d\n", is_flying, client_index);
    } else if (!ready && strcmp(player->command, "READY") == 0) {
        server->game.ready_player[client_index] = true;
        printf("READY FROM %d\n", client_index + 1);
    }
    return;
}
