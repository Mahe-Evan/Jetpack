/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** game logic and events
*/

#include "server.h"
#include <stdio.h>

static void check_electricity_collision(
    server_t *restrict server, int player_index)
{
    if (server->game.map[(int)server->player[player_index].pos.y]
        [(int)server->player[player_index].pos.x] == 'e') {
        server->is_alive[player_index] = false;
        server->player[player_index].pos.x = -1;
        server->player[player_index].pos.y = -1;
        server->player[player_index].is_flying = false;
    }
}

static bool check_coin_collision(
    server_t *restrict server, int player_index, int coin_index)
{
    float p_x = server->player[player_index].pos.x;
    float p_y = server->player[player_index].pos.y;
    float coin_x = server->game.coins_pos[coin_index].x;
    float coin_y = server->game.coins_pos[coin_index].y;
    bool x_matches = (int)p_x == (int)coin_x;
    bool y_matches = (int)p_y == (int)coin_y;
    bool x_r_matches = (p_x - (int)p_x > 0.5) && ((int)p_x + 1 == (int)coin_x);
    bool y_r_matches = (p_y - (int)p_y > 0.5) && ((int)p_y + 1 == (int)coin_y);

    if ((x_matches || x_r_matches) && (y_matches || y_r_matches) &&
        !server->game.coins_collected[coin_index].players[player_index]) {
        server->game.coins_collected[coin_index] .players[player_index] = true;
        server->game.coins_collected[coin_index].pos.x =
            server->game.coins_pos[coin_index].x;
        server->game.coins_collected[coin_index].pos.y =
            server->game.coins_pos[coin_index].y;
        return true;
    }
    return false;
}

static void check_player_collision(
    server_t *restrict server, int player_index)
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
