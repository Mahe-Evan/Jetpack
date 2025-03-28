/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** include of callings functions
*/

#ifndef FUNCTS_H_
    #define FUNCTS_H_

    #include "server.h"
    #include "client.h"

int set_server(int ac, char **av, server_t *server);

char *set_root_directory(char *path);
int check_directory(char *path);

int loop(server_t *server, client_t **client);

void handle_existing_clients(server_t *server, client_t **client);

void receive_command(server_t *server, client_t *client, client_t **clients);
void player_axys_y(server_t *server, client_t *client, int i, bool fly);

bool set_map(char *path, server_t *server);
void send_command(server_t *server, client_t **client, int i);


int check_errors_clients(client_t *client);
void free_all(server_t *server, client_t **client);

#endif /* !FUNCTS_H_ */
