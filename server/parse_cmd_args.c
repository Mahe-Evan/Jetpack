/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** parse command-line arguments
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jetpack.h"

static bool is_valid_map_char(char c)
{
    return c == '_' || c == 'e' || c == 'c';
}

static bool validate_map_line(const char *line, size_t *restrict expected_len)
{
    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n')
        len--;
    if (*expected_len == 0)
        *expected_len = len;
    else if (len != *expected_len)
        return false;
    for (size_t i = 0; i < len; i++)
        if (!is_valid_map_char(line[i]))
            return false;
    return true;
}

static char *copy_map_line(char *line)
{
    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    return strdup(line);
}

static char *fill_empty_line(size_t expected_len)
{
    char *line = malloc(expected_len + 1);

    line = memset(line, '_', expected_len);
    line[expected_len] = '\0';
    return line;
}

static bool read_map_file(const char *path, char *map[static MAP_LEN])
{
    FILE *file = fopen(path, "r");
    char line[1024];
    size_t expected_len = 0;
    int line_count = 0;

    if (!file)
        return false;
    while (line_count < MAP_LEN && fgets(line, sizeof(line), file)) {
        if (!validate_map_line(line, &expected_len)) {
            fclose(file);
            return false;
        }
        map[line_count] = copy_map_line(line);
        line_count++;
    }
    for (int i = line_count; i < MAP_LEN; i++)
        map[i] = fill_empty_line(expected_len);
    fclose(file);
    return line_count > 0;
}

static bool parse_port(const char *port_str, long *restrict port)
{
    char *endptr;
    long val = strtol(port_str, &endptr, 10);

    if (*endptr != '\0' || val <= 0 || val > 65535)
        return false;
    *port = val;
    return true;
}

static void init_parsed(parsed_t *restrict parsed)
{
    parsed->port_nb = 0;
    parsed->is_in_debug_mode = false;
    for (int i = 0; i < MAP_LEN; i++)
        parsed->map[i] = NULL;
}

parsed_t error_parsed(parsed_t *restrict parsed)
{
    parsed->port_nb = 0;
    for (int i = 0; i < MAP_LEN; i++) {
        if (parsed->map[i] != NULL) {
            free(parsed->map[i]);
            parsed->map[i] = NULL;
        }
    }
    return *parsed;
}

static int process_arg(char *argv[], int i, parsed_t *restrict parsed)
{
    if (argv[i] == NULL)
        return -1;
    if (strcmp(argv[i], "-p") == 0 && argv[i + 1]) {
        if (!parse_port(argv[i + 1], &parsed->port_nb))
            return -1;
        return i + 2;
    }
    if (strcmp(argv[i], "-m") == 0 && argv[i + 1]) {
        if (!read_map_file(argv[i + 1], parsed->map))
            return -1;
        return i + 2;
    }
    if (strcmp(argv[i], "-d") == 0) {
        parsed->is_in_debug_mode = true;
        return i + 1;
    }
    return -1;
}

parsed_t parse_args(char *argv[])
{
    parsed_t parsed;
    int i = 1;

    init_parsed(&parsed);
    while (argv[i]) {
        i = process_arg(argv, i, &parsed);
        if (i == -1)
            return error_parsed(&parsed);
    }
    if (parsed.port_nb == 0 || parsed.map[0] == NULL)
        return error_parsed(&parsed);
    return parsed;
}
