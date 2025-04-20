/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** game logic and events
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "server.h"

static void check_electricity_collision(server_t *restrict server,
    int player_index)
{
    int player_y = (int)server->player[player_index].pos.y;
    int player_x = (int)server->player[player_index].pos.x;

    if (player_y < 10 && player_x < strlen(server->game.map[player_y]) &&
        server->game.map[player_y][player_x] == 'e') {
        server->is_alive[player_index] = false;
        server->player[player_index].pos.x = -1;
        server->player[player_index].pos.y = -1;
        return;
    }
    if (player_y == 10 && player_x < strlen(server->game.map[9]) &&
        server->game.map[9][player_x] == 'e') {
        server->is_alive[player_index] = false;
        server->player[player_index].pos.x = -1;
        server->player[player_index].pos.y = -1;
    }
}

static bool check_coin_collision(
    server_t *restrict server, int player_index, int coin_index)
{
    int player_y = (int)server->player[player_index].pos.y;
    int player_x = (int)server->player[player_index].pos.x;
    int coin_y = (int)server->game.coins_pos[coin_index].y;
    int coin_x = (int)server->game.coins_pos[coin_index].x;

    if (server->game.coins_collected[coin_index].players[player_index])
        return false;
    if (player_x == coin_x && player_y == coin_y) {
        server->game.coins_collected[coin_index].players[player_index] = true;
        server->game.coins_collected[coin_index].pos.x = coin_x;
        server->game.coins_collected[coin_index].pos.y = coin_y;
        return true;
    }
    if (player_y == 10 && coin_y == 9 && player_x == coin_x) {
        server->game.coins_collected[coin_index].players[player_index] = true;
        server->game.coins_collected[coin_index].pos.x = coin_x;
        server->game.coins_collected[coin_index].pos.y = coin_y;
        return true;
    }
    return false;
}

static void check_player_collision(server_t *restrict server,
    int player_index)
{
    if (server->game.coins_pos == NULL)
        return;
    for (int i = 0; server->game.coins_pos[i].x != -1.; i++) {
    if (check_coin_collision(server, player_index, i))
        server->player[player_index].nb_coins++;
    }
}

void handle_game_logic(server_t *restrict server, bool ready)
{
    if (!ready || server->is_finished) {
        return;
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->player[i].is_flying) {
            server->player[i].pos.y -= 0.1;
        } else {
            server->player[i].pos.y += 0.1;
        }
        if (server->player[i].pos.y < 0) {
            server->player[i].pos.y = 0;
        }
        if (server->player[i].pos.y > 10) {
            server->player[i].pos.y = 10;
        }
        server->player[i].pos.x += 0.05;
        check_electricity_collision(server, i);
        check_player_collision(server, i);
    }
}
