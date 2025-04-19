/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** handle existing clients
*/

#include "jetpack.h"
#include "server.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static void process_command_buffer(server_t *restrict server,
    player_t *restrict player, int player_index, char *buffer)
{
    char *command_end;

    command_end = strstr(buffer, "\r\n");
    for (; command_end != NULL; command_end = strstr(buffer, "\r\n")) {
        *command_end = '\0';
        strncpy(player->command, buffer, sizeof(player->command) - 1);
        player->command[sizeof(player->command) - 1] = '\0';
        receive_command(server, player, player_index, server->ready);
        memmove(buffer, command_end + 2, strlen(command_end + 2) + 1);
    }
}

static void handle_client_command(server_t *restrict server,
    int index, player_t *restrict player, bool ready)
{
    char buffer[1024];
    ssize_t bytes_read;
    static char command_buffer[MAX_CLIENTS][1024] = {0};
    int player_index = index - 2;
    size_t buf_size;
    size_t buf_len;

    bytes_read =
        read(server->fds[index].fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        return;
    }
    server->ready = ready;
    buffer[bytes_read] = '\0';
    buf_size = sizeof(command_buffer[player_index]);
    buf_len = strlen(command_buffer[player_index]);
    strncat(
        command_buffer[player_index], buffer, buf_size - buf_len - 1);
    process_command_buffer(
        server, player, player_index, command_buffer[player_index]);
}

static void check_player_most_coins(
    server_t *restrict server, int index)
{
    if (server->player[index].nb_coins >
        server->player[server->winner].nb_coins) {
        server->winner = index;
    }
}

static bool end_line_finished(server_t *restrict server)
{
    if (server->player[0].pos.x >= strlen(server->game.map[0]) - 1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            check_player_most_coins(server, i);
        }
        return true;
    }
    return false;
}

static bool check_end_game(server_t *restrict server)
{
    int alive_count = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->is_alive[i] == true) {
            alive_count++;
            server->winner = i;
        }
    }
    if (alive_count == 1 | alive_count == 0) {
        return true;
    }
    server->winner = 0;
    return end_line_finished(server);
}

bool should_process_frame(void)
{
    static const double frame_time_ms = 1000.0 / 60.0;
    static struct timespec last_time = {0, 0};
    struct timespec current_time;
    double elapsed_ms;

    clock_gettime(CLOCK_MONOTONIC, &current_time);
    if (last_time.tv_sec == 0 && last_time.tv_nsec == 0) {
        last_time = current_time;
        return false;
    }
    elapsed_ms =
        (current_time.tv_sec - last_time.tv_sec) * 1000.0;
    if (elapsed_ms < frame_time_ms) {
        return false;
    }
    last_time = current_time;
    return true;
}

bool handle_existing_clients(server_t *restrict server, bool ready)
{
    bool game_finished = check_end_game(server);
    server->is_finished = game_finished;

    handle_game_logic(server, ready);
    for (int i = 2; server->fds[i].fd != -1 && i < MAX_CLIENTS + 2;
        i++) {
        if (server->fds[i].revents & POLLIN) {
            handle_client_command(
                server, i, &server->player[i - 2], ready);
        }
        if (ready && server->game.ready_player[i - 2]) {
            send_command(server, i, game_finished);
        }
    }
    return true;
}
