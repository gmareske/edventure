#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef struct game {
    list_t *left;
    list_t *right;
    uint32_t pos;
    FILE *fp;
    char *fname;
} game_t;

game_t *make_game(FILE *fp, char *fn, uint32_t startpos) {
    game_t *ret = (game_t*)calloc(1,sizeof(game_t));
    ret->pos = startpos;
    ret->left = make_list();
    ret->right = make_list();
    ret->fp = fp;

    size_t len = strlen(fn);
    char *fname = (char*)calloc(len+1,sizeof(uint8_t));
    strcpy(fname, fn);
    ret->fname = fname;

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

void exit_game(game_t *g) {
    // on game exit, write out to the file
    // the contents of left and right buffers
    // reset to the beginning
    fseek(g->fp, 0, SEEK_SET);
    // the left buffer is backwards, first put all
    // characters in the the buffer
    uint32_t lbuf_sz = get_size(g->left);
    uint8_t *lbuf = (uint8_t*)calloc(lbuf_sz, sizeof(uint8_t));
    for (uint32_t i = 0; i < lbuf_sz; i++) {
	// very slow!! O(nlogn) at least
	lbuf[lbuf_sz - 1 - i] = get(g->left, i);
    }
    // write lbuf out
    // todo: is there a faster way than character by character output?
    for (uint32_t i = 0; i < lbuf_sz; i++) {
	fputc(lbuf[i], g->fp); // todo: check errors (fputc returns EOF on error)
    }
    free(lbuf);
    // right side is in order so write it out
    for (uint32_t i = 0; i < get_size(g->right); i++) {
	fputc(get(g->right, i), g->fp);
    }
    // fp should be closed in main.c so don't close it here
}

void free_game(game_t *g) {
    // todo: may move this around
    // exit game and write out to file
    exit_game(g);
    free_list(g->left);
    free_list(g->right);
    free(g->fname);
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
    printf("To your left is a \"%c\" and on your right is a \"%c\"\n",
	   get_first(g->left),
	   get_first(g->right));
}
void print_intro(game_t *g) {
    printf("Welcome brave adventurer!\n");
    printf("You are exploring the mystical lands of %s\n",
	   g->fname);
}
void run_game(game_t *g) {
    print_intro(g);
    for (int i = 0; i < 20; i++) {
	move_right(g,1);
	report_pos(g);
    }
    for (int i = 0; i < 10; i++) {
	move_left(g,1);
	report_pos(g);
    }
}
