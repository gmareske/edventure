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

void transfer_from(list_t *origin, list_t *dest);
void exit_game(game_t *g) {
    // on game exit, write out to the file
    // the contents of left and right buffers
    // reset to the beginning
    fseek(g->fp, 0, SEEK_SET);
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
	fputc(get_first(lbuf), g->fp);
	// todo: check errors (fputc returns EOF on error)
	remove_front(lbuf);
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

void transfer_from(list_t *origin, list_t *dest) {
    // transfer something across the gap buffer
    // can be called to transfer from left to right or vice versa
    if (get_size(origin) > 0) {
	uint8_t val = get_first(origin);
	remove_front(origin);
	insert_front(dest, val);
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
    printf("To your left is a \"%c\" and to your right is a \"%c\"\n",
	   get_first(g->left),
	   get_first(g->right));
}
/* Actual game stuff */
void print_intro(game_t *g) {
    printf("Welcome brave adventurer!\n");
    printf("You are exploring the mystical lands of %s.\n",
	   g->fname);
}
void print_help() {
    printf("Commands:\n");
    for (uint32_t i = 0; i < 32; i++) {
	printf("=");
    }
    printf("\n");
    printf("(l)eft: move left\n");
    printf("(r)ight: move right\n");
    printf("(h)elp: show this help page\n");
    printf("(q)uit: quit this game session\n");
}
void cmd_move_right(game_t *g) {
    uint32_t times = 1; // todo: temporary
    move_right(g, times);
}
void cmd_move_left(game_t *g) {
    uint32_t times = 1; // todo: temporary
    move_left(g, times);
}
bool parse_cmd(char *cmd, game_t *g) {
    // pre: cmd is a null-terminated string
    // should come from user input
    // returns false only on quitting
    if ((strcmp(cmd,"l")) == 0 ||
	(strcmp(cmd,"left")) == 0) {
	cmd_move_left(g); return true;
    } else if ((strcmp(cmd,"r")) == 0 ||
	       (strcmp(cmd,"right")) == 0) {
	cmd_move_right(g); return true;
    } else if ((strcmp(cmd,"h")) == 0 ||
	       (strcmp(cmd,"help")) == 0) {
	print_help(); return true;
    } else if ((strcmp(cmd,"q")) == 0 ||
	       (strcmp(cmd,"quit")) == 0) {
	return false;
    } else {
	printf("Unrecognized command. (h)elp for a list of commands.\n");
	return true;
    }
}
void run_game(game_t *g) {
    print_intro(g);
    bool running = true;
    while (running) {
	report_pos(g);
	printf("> ");
	char resp[32];
	scanf("%s", &resp);
	running = parse_cmd(resp, g);
    }
}
