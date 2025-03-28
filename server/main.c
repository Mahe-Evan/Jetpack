/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** epitech ftp
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <stddef.h>
#include <stdlib.h>

static void set_positions(client_t *client)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client->game.positions[i].x = 0;
        client->game.positions[i].y = 0;
    }
}

static void set_client(client_t **client)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client[i] = malloc(sizeof(client_t));
        if (client[i] == NULL) {
            free_all(NULL, client);
            exit(84);
        }
        client[i]->client_fd = -1;
        client[i]->data_fd = 0;
        client[i]->client_addr.sin_family = AF_INET;
        client[i]->client_addr.sin_addr.s_addr = INADDR_ANY;
        client[i]->client_addr.sin_port = 0;
        client[i]->command[0] = '\0';
        client[i]->status = 0;
        client[i]->game.map = NULL;
        set_positions(client[i]);
    }
}

int main(int ac, char **av)
{
    server_t *server = malloc(sizeof(server_t));
    client_t *client[MAX_CLIENTS];

    set_client(client);
    if (server == NULL || (ac != 5 && ac != 6) ||
        set_server(ac, av, server) == 84) {
        free_all(server, client);
        return 84;
    }
    loop(server, client);
    free_all(server, client);
    return 0;
}
