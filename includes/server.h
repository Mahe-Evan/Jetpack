/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** epitech ftp
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <poll.h>
    #include <stdbool.h>

    #define MAX_CLIENTS 128
    #define TIMEOUT 5000

typedef struct position {
    double x;
    double y;
} position_t;

typedef struct coin {
    unsigned long player_id;
    position_t pos;
} coin_t;

typedef struct coins_collected {
    bool players[MAX_CLIENTS];
    position_t pos;
} coins_collected_t;

typedef struct game {
    char *map[10];
    position_t *coins_pos;
    coins_collected_t *coins_collected;
    bool ready_player[MAX_CLIENTS];
    coin_t collected_coins[MAX_CLIENTS * 2];
} game_t;

typedef struct client {
    char command[128];
    position_t pos;
    bool is_flying;
    unsigned int nb_coins;
} player_t, client_t;

typedef struct server_s {
    bool debug_mode;
    struct pollfd fds[MAX_CLIENTS + 2];
    game_t game;
    player_t player[MAX_CLIENTS];
} server_t;

#endif /* !SERVER_H_ */
