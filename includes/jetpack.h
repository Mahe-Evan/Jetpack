/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** include of callings functions
*/

#ifndef JETPACK_H_
    #define JETPACK_H_

    #include <stdbool.h>
    #include <sys/poll.h>
    #include "server.h"

    #define MAP_LEN 10
    #ifdef EXIT_FAILURE
        #undef EXIT_FAILURE
    #endif
    #define EXIT_FAILURE 84

typedef struct pollfd pollfd_t;

typedef struct parsed_s {
    long port_nb;
    char *map[MAP_LEN];
    bool is_in_debug_mode;
} parsed_t;

parsed_t parse_args(char *argv[]);
parsed_t error_parsed(parsed_t *restrict parsed);

server_t set_server(char *av[]);

int set_signal(void);
bool handle_signal(struct pollfd *restrict pfd);

int loop(server_t *restrict server);

bool handle_existing_clients(server_t *restrict server, bool ready);

void receive_command(server_t *restrict server, player_t *restrict player,
    int client_index, bool ready);
void send_command(server_t *restrict server, int index, bool game_finished);

void handle_game_logic(server_t *restrict server, bool ready);

#endif /* !JETPACK_H_ */
