#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "list.h"

typedef struct game {
    list_t *left;
    list_t *right;
    uint32_t pos;
} game_t;

game_t *make_game(FILE *fp, uint32_t startpos) {
    game_t *ret = (game_t*)calloc(1,sizeof(game_t));
    ret->pos = startpos;
    ret->left = make_list();
    ret->right = make_list();
    // we know startpos will be less than the length of the file
    uint32_t c;
    for (uint32_t i = 0; i < startpos; i++) {
	c = getc(fp);
	insert_front(ret->left, c);
    }
    while ((int32_t)(c = getc(fp)) != EOF) {
	insert_back(ret->right, c);
    }
    return ret;
}
void free_game(game_t *g) {
    free_list(g->left);
    free_list(g->right);
    free(g);
}

void transfer_from(list_t *origin, list_t *destination) {
    // transfer something across the gap buffer
    // can be called to transfer from left to right or vice versa
    if (get_size(origin) > 0) {	
	uint8_t val = get_first(origin);
	remove_front(origin);
	insert_front(destination, val);
    }
}

void move_left(game_t *g, uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
	transfer_from(g->left, g->right);
    }
}
void move_right(game_t *g, uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
	transfer_from(g->right, g->left);
    }
}
void report_pos(game_t *g) {
    printf("On your left is a %c | On your right is a %c\n",
	   get_first(g->left),
	   get_first(g->right));
}

void run_game(game_t *g) {
    for (int i = 0; i < 20; i++) {
	move_right(g,1);
	report_pos(g);
    }
    for (int i = 0; i < 10; i++) {
	move_left(g,1);
	report_pos(g);
    }
}
