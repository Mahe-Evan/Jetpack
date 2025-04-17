/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** game logic and events
*/

#include "server.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static void check_player_collision(
    server_t *restrict server, int player_index)
{
    if (server->game.coins_pos == NULL)
        return;
    for (int i = 0; server->game.coins_pos[i].x != -1.; i++) {
        if (server->player[player_index].pos.x ==
                server->game.coins_pos[i].x &&
            server->player[player_index].pos.y ==
                server->game.coins_pos[i].y &&
            server->game.coins_collected[i].players[player_index] ==
                false) {
            server->player[player_index].nb_coins++;
            server->game.coins_collected[i].players[player_index] =
                true;
            server->game.coins_collected[i].pos.x =
                server->game.coins_pos[i].x;
            server->game.coins_collected[i].pos.y =
                server->game.coins_pos[i].y;
            break;
        }
    }
}

static bool should_process_frame(void)
{
    static struct timespec last_time = {0, 0};
    struct timespec current_time;
    double elapsed_ms;
    const double frame_time_ms = 16.67;

    clock_gettime(CLOCK_MONOTONIC, &current_time);
    if (last_time.tv_sec == 0 && last_time.tv_nsec == 0) {
        last_time = current_time;
        return false;
    }
    elapsed_ms =
        (current_time.tv_sec - last_time.tv_sec) * 1000.0 +
        (current_time.tv_nsec - last_time.tv_nsec) / 1000000.0;
    if (elapsed_ms < frame_time_ms) {
        return false;
    }
    last_time = current_time;
    return true;
}

void handle_game_logic(server_t *restrict server, bool ready)
{
    if (!ready) {
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
        check_player_collision(server, i);
    }
}
