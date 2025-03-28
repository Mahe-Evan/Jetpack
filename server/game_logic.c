/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** game logic and events
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void player_axys_y(server_t *server, client_t *client, int i, bool fly)
{
    if (fly == true) {
        client->game.positions[i].y -= 10;
    }
    if (client->game.positions[i].y < 0) {
        client->game.positions[i].y += 0;
    }
}
