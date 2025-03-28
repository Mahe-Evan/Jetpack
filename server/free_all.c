/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** free function
*/

#include <stdlib.h>
#include <stdio.h>
#include "../includes/server.h"
#include "../includes/client.h"

void free_all(server_t *server, client_t **client)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        for (int j = 0; client[i]->game.map != NULL && j < 10 &&
            client[i]->game.map[j] != NULL; j++) {
            free(client[i]->game.map[j]);
        }
        free(client[i]->game.map);
        free(client[i]);
    }
    free(server);
}
