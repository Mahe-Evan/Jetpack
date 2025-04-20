/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** main loop
*/

#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "jetpack.h"
#include "server.h"

static void set_player(int player_fd, char *map[static MAP_LEN])
{
    static int player_id = 1;

    dprintf(player_fd, "ID %d\r\n", player_id);
    player_id++;
    dprintf(player_fd, "MAP %lu ", strlen(*map));
    for (int i = 0; i < MAP_LEN; i++) {
        write(player_fd, map[i], strlen(map[i]));
    }
    write(player_fd, "\r\n", 2);
    return;
}

static bool handle_new_connection(
    pollfd_t fds[static restrict MAX_CLIENTS + 2],
    char *map[static MAP_LEN])
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = 0;

    if (fds[MAX_CLIENTS + 1].fd != -1 || !(fds[1].revents & POLLIN)) {
        return true;
    }
    client_fd =
        accept(fds[1].fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd == -1) {
        perror("accept");
        return false;
    }
    for (; fds->fd != -1; fds++)
        ;
    fds->fd = client_fd;
    set_player(client_fd, map);
    return true;
}

static bool check_player_ready(
    bool ready_player[static restrict MAX_CLIENTS],
    int nb_player_required)
{
    int nb_ready = 0;

    if (nb_player_required > MAX_CLIENTS) {
        nb_player_required = MAX_CLIENTS;
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (ready_player[i]) {
            nb_ready++;
        }
    }
    if (nb_ready >= nb_player_required) {
        return true;
    }
    return false;
}

static void send_start(
    bool *restrict is_enough_player_ready,
    bool ready_player[static MAX_CLIENTS],
    pollfd_t fds[static restrict MAX_CLIENTS + 2],
    bool is_alive[static MAX_CLIENTS])
{
    *is_enough_player_ready = check_player_ready(ready_player, 2);
    if (!(*is_enough_player_ready)) {
        return;
    }
    for (int i = 2; fds[i].fd != -1 && i < MAX_CLIENTS + 2; i++) {
        if (ready_player[i - 2]) {
            write(fds[i].fd, "START\r\n", 7);
            is_alive[i - 2] = true;
        }
    }
    return;
}

static void handle_server_loop_iteration(server_t *restrict server,
    bool *is_enough_player_ready, bool *restrict is_running)
{
    int poll_res = poll(server->fds, MAX_CLIENTS + 2, 0);

    if (poll_res == -1) {
        perror("poll");
        return;
    }
    if (!*is_enough_player_ready) {
        *is_running = handle_new_connection(server->fds, server->game.map);
        send_start(is_enough_player_ready, server->game.ready_player,
            server->fds, server->is_alive);
    }
    handle_existing_clients(server, *is_enough_player_ready);
    *is_running = *is_running ? handle_signal(&server->fds[0]) : false;
    return;
}

int loop(server_t *restrict server)
{
    bool is_running = true;
    bool is_enough_player_ready = false;

    while (is_running) {
        handle_server_loop_iteration(server, &is_enough_player_ready,
            &is_running);
        usleep(1000000 / 90);
    }
    return errno ? EXIT_FAILURE : EXIT_SUCCESS;
}
