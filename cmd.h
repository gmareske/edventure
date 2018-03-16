#pragma once

/*
  cmd.h exposes functions that a game loop
  in main.c might need in order to run the game
*/

#include "game.h"

bool parse_cmd(char *cmd, game_t *g);
void print_intro(game_t *g);
void report_pos(game_t *g);

