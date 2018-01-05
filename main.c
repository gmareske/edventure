#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "game.h"

uint32_t file_size(char *fname) {
    struct stat st;
    if (stat(fname, &st) > -1) {
	return st.st_size;
    } else {
	return -1;
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
    FILE *fp = fopen(fname, "a+");
    srand(time(NULL));
    uint32_t startpos = rand() % fsize;
    printf("file size: %d startpos: %d\n", fsize, startpos);
    game_t *game = make_game(fp, fname, startpos);
    //    printf("buffer sizes, left: %d right: %d\n", get_size(game->left), get_size(game->right));
    run_game(game);
    free_game(game);
    fclose(fp);
}
