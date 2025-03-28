/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** send commands
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void send_command(server_t *server, client_t **client, int i)
{
    if (server->game.positions[i].y != 0) {
        for (int j = 0; j < MAX_CLIENTS; j++) {
            write(client[j]->client_fd,
                client[j]->game.map[server->game.positions[i].y],
                strlen(client[j]->game.map[server->game.positions[i].y]));
        }
    }
}
