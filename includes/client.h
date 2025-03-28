/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** client struct
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <stdbool.h>
    #include "server.h"

typedef struct clients_position {
    int x;
    int y;
} clients_position_t;

typedef struct clients_game {
    char **map;
    position_t positions[MAX_CLIENTS];
} clients_game_t;

typedef struct client {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int data_fd;
    char command[128];
    int status;
    game_t game;
} client_t;

#endif /* !CLIENT_H_ */
