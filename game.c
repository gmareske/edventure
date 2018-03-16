#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"

typedef struct game {
    list_t *left;
    list_t *right;
    uint32_t pos; 
    FILE *fp;
    char *fname;
    uint32_t line_pos;
} game_t;

void set_linepos(game_t *g) {
    uint32_t j;
    uint32_t left_len = get_size(g->left);
    for (j = 0; get(g->left, j) != '\n'&& j < left_len; j++) {}
    g->line_pos = j;
}
game_t *make_game(FILE *fp, char *fn, uint32_t startpos) {
    game_t *ret = (game_t*)calloc(1,sizeof(game_t));
    ret->pos = startpos;
    ret->left = make_list();
    ret->right = make_list();
    // fp is closed in main.c, and re-opened only when we need
    // to write out the the file
    ret->fp = NULL;
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
    // set inital position in line
    set_linepos(ret);
    return ret;
}
void transfer_from(list_t *origin, list_t *dest);
void exit_game(game_t *g) {
    // on game exit, write out to the file
    // the contents of left and right buffers
    // open the file in write mode this time
    g->fp = fopen(g->fname, "w");
    // the left buffer is backwards, reverse it by shoving
    // everything to a new list.
    uint32_t lbuf_sz = get_size(g->left);
    list_t *lbuf = make_list();
    for (uint32_t i = 0; i < lbuf_sz; i++) {
	transfer_from(g->left, lbuf);
    }
    // write lbuf out
    // todo: is there a faster way than character by character output?
    for (uint32_t i = 0; i < lbuf_sz; i++) {
	fputc(remove_front(lbuf), g->fp);
	// todo: check errors (fputc returns EOF on error)
	// not sure how I would gracefully exit here
    }
    free_list(lbuf);
    // right side is in order so write it out
    for (uint32_t i = 0; i < get_size(g->right); i++) {
	fputc(get(g->right, i), g->fp);
    }
    // close the file pointer here
    fclose(g->fp);
    g->fp = NULL;
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

void transfer_from(list_t *origin, list_t *dest) {
    // transfer something across the gap buffer
    // can be called to transfer from left to right or vice versa
    if (get_size(origin) > 0) {
	uint8_t val = remove_front(origin);
	insert_front(dest, val);
    }
}

void move_left(game_t *g, uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
	transfer_from(g->left, g->right);
    }
    g->pos -= times;
    set_linepos(g);
}

void move_right(game_t *g, uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
	transfer_from(g->right, g->left);
    }
    g->pos += times;
    set_linepos(g);
}

void seek_left(game_t *g) {
    /* search left until we find the beginning of the line */
    while (get_first(g->left) != '\n' && get_size(g->left) > 0) {
	move_left(g,1);
    }
}

void seek_right(game_t *g) {
    /* search right until we find the beginning of the next line */
    while (get_first(g->right) != '\n' && get_size(g->right) > 0) {
	move_right(g,1);
    }
    if (get_size(g->right) > 0) {
	move_right(g,1);
    }
}

void move_up(game_t *g) {
    uint32_t cur = g->line_pos;
    // go back two lines and go forward to our current position
    seek_left(g);
    move_left(g,1);
    seek_left(g);
    move_right(g,cur);
}
void move_down(game_t *g) {
    uint32_t cur = g->line_pos;
    // go forward one line, then go forward to current position
    // or the end of the line
    seek_right(g);
    uint32_t i = 0;
    while (i < cur &&
	   get_first(g->right) != '\n' &&
	   get_size(g->right) > 0) {
	move_right(g,1);
    }
}
uint8_t get_close_left(game_t *g) {
    return get_first(g->left);
}
uint8_t get_close_right(game_t *g) {
    return get_first(g->right);
}
