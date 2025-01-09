#include "file.h"
#include "board.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // get path for file mode (-f option)
    char* path = file_mode(argc, argv);

    // FILE MODE
    if(path != NULL)
    {
        printf("\tFile path: %s\n", path);

        printf("\tFILE MODE\n");
    }
    // NORMAL MODE
    else
    {
        printf("\tNORMAL MODE\n");

        // get difficulty
        Difficulty difficulty = board_difficulty();

        // initialize board
        Board board = board_init(difficulty);

        // print board stats
        board_stats(&board);
    }

    return EXIT_SUCCESS;
}