/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** set the serveur
*/

#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "jetpack.h"
#include "server.h"

static void set_poll_fds(server_t *restrict server)
{
    for (int i = 0; i < MAX_CLIENTS + 2; i++) {
        server->fds[i].fd = -1;
        server->fds[i].events = POLLIN;
    }
}

static int set_server_fd(long port_nb)
{
    const struct sockaddr_in server_addr = {AF_INET, htons(port_nb),
        {INADDR_ANY}, {'\0'}};
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        return -1;
    }
    if (bind(server_fd, (const struct sockaddr *)&server_addr,
        sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return -1;
    }
    if (listen(server_fd, 10) == -1) {
        perror("listen");
        close(server_fd);
        return -1;
    }
    return server_fd;
}

static void set_ready_players(game_t *restrict game)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        game->ready_player[i] = false;
    }
    return;
}

static size_t get_nb_coins(char *map[static MAP_LEN])
{
    size_t nb_coins = 0;
    int j = 0;

    for (int i = 0; i < MAP_LEN;) {
        if (map[i][j] == '\0') {
            i++;
            j = 0;
            continue;
        }
        if (map[i][j] == 'c') {
            nb_coins++;
        }
        j++;
    }
    return nb_coins;
}

static coin_t *set_coins(char *map[static MAP_LEN])
{
    size_t nb_coins = get_nb_coins(map);
    coin_t *coin_array = malloc(sizeof(coin_t) * (nb_coins + 1));
    int coin_array_index = 0;
    int j = 0;

    for (int i = 0; i < MAP_LEN;) {
        if (map[i][j] == '\0') {
            i++;
            j = 0;
            continue;
        }
        if (map[i][j] == 'c') {
            coin_array[coin_array_index] = (coin_t){-1, {j, i}};
            coin_array_index++;
        }
        j++;
    }
    coin_array[nb_coins] = (coin_t){-1, {-1, -1}};
    return coin_array;
}

static game_t set_game(char *map[static MAP_LEN])
{
    game_t game;

    for (int i = 0; i < MAP_LEN; i++) {
        game.map[i] = map[i];
    }
    game.coins_pos = set_coins(map);
    set_ready_players(&game);
    game.collected_coins = NULL;
    return game;
}

static void set_players(server_t *restrict server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->player[i] = (player_t){{'\0'}, {0, 0}, false, 0};
    }
    return;
}

server_t set_server(char *av[])
{
    server_t server;
    parsed_t parsed = parse_args(av);

    set_poll_fds(&server);
    if (parsed.port_nb == 0) {
        return server;
    }
    server.debug_mode = parsed.is_in_debug_mode;
    server.fds[0].fd = set_signal();
    server.fds[1].fd = set_server_fd(parsed.port_nb);
    if (server.fds[0].fd == -1 || server.fds[1].fd == -1) {
        close(server.fds[0].fd);
        close(server.fds[1].fd);
        error_parsed(&parsed);
        return server;
    }
    server.game = set_game(parsed.map);
    set_players(&server);
    return server;
}
