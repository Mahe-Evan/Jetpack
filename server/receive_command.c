/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** check and call the commands
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void receive_command(server_t *server, client_t *client, client_t **clients)
{
    if (strncasecmp(client->command, "FLY", 3) == 0) {
        player_axys_y(server, client, 0, true);
        printf("FLY\n");
    }
    if (strncasecmp(client->command, "MAP", 3) == 0) {
        printf("MAP\n");
    }
    return;
}
