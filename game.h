#pragma once

#include <stdint.h>
#include <stdio.h>

#include "list.h"

// this is an interface for main.c
// no other functionality needed

typedef struct game {
    list_t *left;
    list_t *right;
    uint32_t pos; 
    FILE *fp;
    char *fname;
    uint32_t line_pos;
} game_t;

game_t *make_game(FILE *fp, char *fn, uint32_t startpos);
void free_game(game_t *game);

// this is an interface for cmd.c
// where cmd needs the functionality defined in game.c

// critical movement functions
void move_left(game_t *g, uint32_t times);
void move_right(game_t *g, uint32_t times);
void move_down(game_t *g);
void move_up(game_t *g);
void seek_left(game_t *g);
void seek_right(game_t *g);
// critical getting fuctions
// todo: maybe just expose the fields in the data structure?
uint8_t get_close_left(game_t *g);
uint8_t get_close_right(game_t *g);

// may not be necessary, see in testing
void transfer_from(list_t *origin, list_t *dest);
