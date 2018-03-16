#pragma once
// cmd.h
// should only be exposing the parse command to main
// where main controls the game event loop

#include "game.h"

bool parse_cmd(char *cmd, game_t *g);
void print_intro(game_t *g);
void report_pos(game_t *g);

