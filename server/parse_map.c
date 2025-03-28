/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** parse the map
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/server.h"

static bool fill_lines(char **map)
{
    int len = 0;

    for (int i = 0; map[i] != NULL; i++) {
            len++;
    }
    if (len > 10) {
        printf("Error: too many lines\n");
        return false;
    }
    if (len < 10) {
        printf("Error: not enough lines\n");
        return false;
    }
    return true;
}

static bool len_equals(char **map)
{
    int len_first = 0;
    int len = 0;

    for (int i = 0; map[0][i] != '\0'; i++)
        len_first++;
    for (int i = len_first + 1; map[0][i] != '\0'; i++) {
        for (int j = 0; map[0][i] != '\n'; j++)
            len++;
        if (len != len_first)
            return false;
        len = 0;
    }
    return fill_lines(map);
}

static int readfile(FILE *file, char **line, size_t *len, server_t *server)
{
    ssize_t read;
    int i = 0;

    server->game.map = malloc(sizeof(char *) * 11);
    if (server->game.map == NULL)
        return 84;
    while (1) {
        read = getline(line, len, file);
        if (read == -1)
            break;
        if ((*line)[read - 1] == '\n')
            (*line)[read - 1] = '\0';
        server->game.map[i] = strdup(*line);
        if (server->game.map[i] == NULL)
            return 84;
        i++;
    }
    server->game.map[i] = NULL;
    return 0;
}

bool set_map(char *path, server_t *server)
{
    FILE *file = fopen(path, "r");
    char *line = NULL;
    size_t len = 0;

    if (file == NULL)
        return false;
    if (readfile(file, &line, &len, server) == 84)
        return false;
    free(line);
    fclose(file);
    free(path);
    if (!len_equals(server->game.map))
        return false;
    return true;
}
