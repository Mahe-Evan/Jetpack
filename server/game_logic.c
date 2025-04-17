/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** game logic and events
*/

#include "server.h"
#include <stdio.h>
#include <unistd.h>

void handle_game_logic(server_t *restrict server, bool ready)
{
    if (!ready) {
        return;
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->game.ready_player[i]) {
            server->player[i].pos.x += 1.0;
            server->player[i].pos.y += 1.0;
            server->player[i].nb_coins++;
            printf("Player %d moved to (%f, %f) and collected %d "
                   "coins\n",
                i, server->player[i].pos.x, server->player[i].pos.y,
                server->player[i].nb_coins);
        }
    }
    sleep(2);
    return;
}
