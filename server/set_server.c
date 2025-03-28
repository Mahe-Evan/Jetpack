/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** set the serveur
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void set_positions(position_t *positions)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        positions[i].x = 0;
        positions[i].y = 0;
    }
}

static int parse_args(int ac, char **av, server_t *server)
{
    if (ac != 5 && ac != 6)
        return 84;
    if (strcmp(av[1], "-p") != 0 || strcmp(av[3], "-m") != 0) {
        return 84;
    }
    server->debug = false;
    if (ac == 6 && strcmp(av[5], "-d") != 0) {
        return 84;
    }
    return 0;
}

static int verify_server(server_t *server)
{
    if (server->fd_server == -1) {
        close(server->fd_server);
        perror("socket");
        return 84;
    }
    if (bind(server->fd_server, (const struct sockaddr *)&server->server_addr,
        sizeof(server->server_addr)) == -1) {
        close(server->fd_server);
        perror("bind");
        return 84;
    }
    if (listen(server->fd_server, 10) == -1) {
        close(server->fd_server);
        perror("listen");
        return 84;
    }
    return 0;
}

int set_server(int ac, char **av, server_t *server)
{
    char *path = NULL;

    path = strdup(av[4]);
    if (!set_map(path, server) || parse_args(ac, av, server) == 84) {
        return 84;
    }
    server->port = atoi(av[2]);
    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(server->port);
    set_positions(server->game.positions);
    if (server->game.map == NULL) {
        return 84;
    }
    server->fd_server = socket(AF_INET, SOCK_STREAM, 0);
    return verify_server(server);
}
