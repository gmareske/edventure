#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "game.h"
#include "cmd.h"

uint32_t file_size(char *fname) {
    struct stat st;
    if (stat(fname, &st) > -1) {
	return st.st_size;
    } else {
	return -1;
    }
}
void run_game(game_t *g) {
    print_intro(g);
    bool running = true;
    while (running) {
	report_pos(g);
	printf("> ");
	char resp[32];
	scanf(" %s", (char*)&resp);
	running = parse_cmd(resp, g);
    }
}
int main(int argc, char **args) {
    // we care only about the first argument
    if (argc != 2) {
	printf("Must provide only one argument: edv <filename>\n");
	return 1;
    }
    char *fname = args[1];
    uint32_t fsize = file_size(fname);
    FILE *fp = fopen(fname, "r");
    srand(time(NULL));
    uint32_t startpos = rand() % fsize;
    printf("file size: %d startpos: %d\n", fsize, startpos);
    game_t *game = make_game(fp, fname, startpos);
    fclose(fp);
    run_game(game);
    free_game(game);

}
