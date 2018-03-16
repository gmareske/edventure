#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "game.h"

void report_pos(game_t *g) {
    printf("Position %d in line.\n", g->line_pos);
    printf("To your left is a \"%c\" and to your right is a \"%c\"\n",
	   get_close_left(g),
	   get_close_right(g));
}
void print_intro(game_t *g) {
    printf("Welcome brave adventurer!\n");
    printf("You are exploring the mystical lands of %s.\n",
	   g->fname);
}
void print_help() {
    printf("Commands:\n");
    for (uint32_t i = 0; i < 48; i++) {
	printf("=");
    }
    printf("\n");
    printf("(l)eft: move left\n");
    printf("(r)ight: move right\n");
    printf("(u)p: move up\n");
    printf("(d)own: move down\n");
    printf("(k)ill: battle the right obstacle\n");
    printf("(c)raft: build something to the right\n");
    printf("look: inspect immediate surroundings\n");
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
void cmd_move_up(game_t *g) {
    //uint32_t times = 1;
    move_up(g);
}
void cmd_move_down(game_t *g) {
    //uint32_t times = 1;
    move_down(g);
}
void cmd_kill(game_t *g) {
    // killing a character removes first character to the right
    uint8_t right_char = remove_front(g->right);
    printf("You fight and kill the \"%c\"\n", right_char);
}
void cmd_craft(game_t *g) {
    printf("What do you want to craft?\n");
    printf(">> ");
    char c[2048]; // arbitrary
    scanf(" %s", (char*)&c);
    for (uint32_t i = strlen(c); i > 0; i--) {
	insert_front(g->right,c[i-1]);
    }
    printf("You craft a \"%s\" and place it to your right.\n",c);
}
void cmd_inspect(game_t *g) {
    list_t *lbuf = make_list();
    uint32_t i = 0;
    uint8_t c;
    uint32_t l_sz = get_size(g->left);
    while ((c = get(g->left, i)) != '\n' && i < l_sz) {
	insert_front(lbuf, c);
	i++;
    }
    printf("You inspect your surroundings and see: \n");
    uint32_t lbuf_sz = get_size(lbuf);
    for (uint32_t j = 0; j < lbuf_sz; j++) {
	printf("%c", remove_front(lbuf));
    }
    i = 0;
    uint32_t r_sz = get_size(g->right);
    while ((c = get(g->right, i)) != '\n' && i < r_sz) {
	printf("%c", c);
	i++;
    }
    printf("\n");
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
    } else if ((strcmp(cmd,"u")) == 0 ||
	       (strcmp(cmd,"up")) == 0) {
	cmd_move_up(g); return true;
    } else if ((strcmp(cmd,"d")) == 0 ||
	       (strcmp(cmd,"down")) == 0) {
	cmd_move_down(g); return true;
    } else if ((strcmp(cmd,"k")) == 0 ||
	       (strcmp(cmd,"kill")) == 0) {
	cmd_kill(g); return true;
    } else if ((strcmp(cmd,"c")) == 0 ||
	       (strcmp(cmd,"craft")) == 0) {
	cmd_craft(g); return true;
    } else if ((strcmp(cmd,"look")) == 0) {
	cmd_inspect(g); return true;
    } else if ((strcmp(cmd,"h")) == 0 ||
	       (strcmp(cmd,"help")) == 0) {
	print_help(); return true;
    } else if ((strcmp(cmd,"q")) == 0 ||
	       (strcmp(cmd,"quit")) == 0) {
	return false;
    } else {
	printf("Unrecognized command \"%s\". (h)elp for a list of commands.\n", cmd);
	return true;
    }
}
