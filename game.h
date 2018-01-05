#include <stdint.h>
#include <stdio.h>

// this is an interface for main.c
// no other functionality needed

typedef struct game game_t;
game_t *make_game(FILE *fp, char *fn, uint32_t startpos);
void free_game(game_t *game);
void run_game(game_t *game);
