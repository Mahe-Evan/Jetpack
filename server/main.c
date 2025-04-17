/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** epitech ftp
*/

#include "jetpack.h"
#include "server.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac[[gnu::unused]], char *av[])
{
    server_t server = set_server(av);
    int ret_val = EXIT_SUCCESS;

    if (server.fds[1].fd == -1) {
        return EXIT_FAILURE;
    }
    ret_val = loop(&server);
    for (int i = 0; i < MAP_LEN; i++) {
        free(server.game.map[i]);
    }
    free(server.game.coins_pos);
    for (int i = 0; i < MAX_CLIENTS + 2; i++) {
        if (server.fds[i].fd != -1) {
            close(server.fds[i].fd);
        }
    }
    return ret_val;
}
