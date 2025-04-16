/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** game logic and events
*/

#include <stdlib.h>
#include <string.h>
#include "server.h"

#define GRAVITY 0.2f
#define JETPACK_FORCE 0.5f
#define HORIZONTAL_SPEED 0.3f
#define MIN_Y 0.0f
#define MAX_Y_MULTIPLIER 0.8f

typedef struct {
    unsigned long player_id;
    int x;
    int y;
} collected_coin_t;

static void apply_physics(player_t *player, const char **map)
{
    int max_height = 0;
    
    // Find map height
    for (int i = 0; map[i] != NULL; i++)
        max_height++;
    
    // Apply vertical movement
    if (player->is_flying) {
        player->pos.y -= JETPACK_FORCE;
    } else {
        player->pos.y += GRAVITY;
    }
    
    // Apply boundaries
    if (player->pos.y < MIN_Y)
        player->pos.y = MIN_Y;
    if (player->pos.y > max_height * MAX_Y_MULTIPLIER)
        player->pos.y = max_height * MAX_Y_MULTIPLIER;
    
    // Apply horizontal movement (always moving forward)
    player->pos.x += HORIZONTAL_SPEED;
}

static bool check_coin_collision(player_t *player, char **map, int *coin_pos)
{
    int player_x = (int)player->pos.x;
    int player_y = (int)player->pos.y;
    
    // Check if there's a coin at the player's position
    if (player_x >= 0 && player_y >= 0 && map[player_y] && 
        player_x < (int)strlen(map[player_y]) && map[player_y][player_x] == 'c') {
        coin_pos[0] = player_x;
        coin_pos[1] = player_y;
        // Replace the collected coin with empty space
        map[player_y][player_x] = '_';
        return true;
    }
    return false;
}

static void add_collected_coin(collected_coin_t ***coins, int *count, 
                              unsigned long player_id, int x, int y)
{
    *coins = realloc(*coins, (*count + 1) * sizeof(collected_coin_t *));
    if (*coins == NULL)
        return;
    
    (*coins)[*count] = malloc(sizeof(collected_coin_t));
    if ((*coins)[*count] == NULL)
        return;
    
    (*coins)[*count]->player_id = player_id;
    (*coins)[*count]->x = x;
    (*coins)[*count]->y = y;
    (*count)++;
}

static void free_collected_coins(coin_t **coins)
{
    if (coins == NULL)
        return;
    
    for (int i = 0; coins[i] != NULL; i++)
        free(coins[i]);
    free(coins);
}

static void update_player(server_t *server, int player_idx)
{
    player_t *player = &server->player[player_idx];
    int coin_pos[2] = {0};
    
    // Update player position based on physics
    apply_physics(player, (const char **)server->game.map);
    
    // Check for coin collection
    if (check_coin_collision(player, server->game.map, coin_pos)) {
        player->nb_coins++;
        // Add to collected coins list
        int count = 0;
        if (server->game.collected_coins != NULL) {
            for (; server->game.collected_coins[count] != NULL; count++);
        }
        add_collected_coin((collected_coin_t ***)&server->game.collected_coins, 
                          &count, player_idx + 1, coin_pos[0], coin_pos[1]);
    }
}

void handle_game_logic(server_t *restrict server, bool ready)
{
    static float tick_counter = 0.0f;
    
    if (!ready)
        return;
    
    // Free previous frame's collected coins
    free_collected_coins(server->game.collected_coins);
    server->game.collected_coins = NULL;
    
    // Update all players
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->fds[i + 2].fd != -1 && server->game.ready_player[i])
            update_player(server, i);
    }
    
    // Increment game tick
    tick_counter += 1.0f;
}
