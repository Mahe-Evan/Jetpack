/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** main loop
*/

#include "../includes/server.h"
#include "../includes/client.h"
#include "../includes/functs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

static void set_client(client_t **client, int client_fd)
{
    int i = 0;

    for (i = 0; client[i]->client_fd != -1 && i < MAX_CLIENTS; i++)
        continue;
    if (client[i]->client_fd == -1) {
        client[i]->client_fd = client_fd;
    }
}

static void accept_new_client(server_t *server, client_t **client)
{
    struct sockaddr_in client_addr;
    int client_fd = 0;
    socklen_t addr_len = sizeof(client_addr);

    client_fd = accept(server->fd_server,
        (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return;
    }
    if (server->poll < MAX_CLIENTS) {
        server->poll++;
        server->fds[server->poll].fd = client_fd;
        server->fds[server->poll].events = POLLIN;
    } else {
        close(client_fd);
    }
    return set_client(client, client_fd);
}

int loop(server_t *server, client_t **client)
{
    int poll_ret = 0;

    server->fds[0].fd = server->fd_server;
    server->fds[0].events = POLLIN;
    while (1) {
        poll_ret = poll(server->fds, server->poll + 1, -1);
        if (poll_ret < 0) {
            perror("Poll failed");
            break;
        }
        if (poll_ret == 0) {
            printf("Poll timed out\n");
            continue;
        }
        if (server->fds[0].revents & POLLIN) {
            accept_new_client(server, client);
        }
        handle_existing_clients(server, client);
    }
    return 0;
}
