/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** send commands
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"

static void send_coins(coin_t *collected_coins, int client_fd,
    server_t *restrict server)
{
    int coin_msg_len = 0;
    char *coin_msg = NULL;

    if (collected_coins != NULL && collected_coins->player_id != 0) {
        coin_msg_len = snprintf(NULL, 0, "COIN %lu %d %d\r\n",
            collected_coins->player_id, (int)collected_coins->pos.x,
            (int)collected_coins->pos.y);
        coin_msg = malloc(coin_msg_len + 1);
        snprintf(coin_msg, coin_msg_len + 1, "COIN %lu %d %d\r\n",
            collected_coins->player_id, (int)collected_coins->pos.x,
            (int)collected_coins->pos.y);
        if (server->debug_mode) {
            printf("COIN %lu %d %d\n", collected_coins->player_id,
                (int)collected_coins->pos.x, (int)collected_coins->pos.y);
        }
        write(client_fd, coin_msg, coin_msg_len);
        free(coin_msg);
    }
    return;
}

static void send_player(
    unsigned long player_id,
    client_t *restrict player,
    int player_fd,
    server_t *restrict server)
{
    int msg_len = snprintf(NULL, 0, "PLAYER %lu %f %f %u %c\r\n",
        player_id, player->pos.x, player->pos.y,
        player->nb_coins, player->is_flying + '0');
    char *msg = malloc(msg_len + 1);

    snprintf(msg, msg_len + 1, "PLAYER %lu %f %f %u %c\r\n",
        player_id, player->pos.x, player->pos.y,
        player->nb_coins, player->is_flying + '0');
    write(player_fd, msg, msg_len);
    if (server->debug_mode) {
        write(STDOUT_FILENO, msg, msg_len);
    }
    free(msg);
    return;
}

static void send_winner(server_t *restrict server)
{
    int msg_len = snprintf(NULL, 0, "END %d\r\n", server->winner);
    char *msg = malloc(msg_len + 1);

    snprintf(msg, msg_len + 1, "END %d\r\n", server->winner);
    for (int i = 2; server->fds[i].fd != -1 && i < MAX_CLIENTS + 2; i++) {
        write(server->fds[i].fd, msg, msg_len);
    }
    if (server->debug_mode) {
        write(STDOUT_FILENO, msg, msg_len);
    }
    free(msg);
    return;
}

void send_command(server_t *restrict server, int index, bool game_finished)
{
    for (int i = 0; server->fds[i + 2].fd != -1 && i < MAX_CLIENTS;
        i++) {
        if (game_finished) {
            return send_winner(server);
        }
        if (server->game.ready_player[i]) {
            send_player(i + 1, &server->player[i],
                server->fds[index].fd, server);
        }
    }
    send_coins(server->game.collected_coins, server->fds[index].fd, server);
    return;
}
