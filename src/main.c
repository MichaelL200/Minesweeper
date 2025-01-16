#include "debug.h"
#include "file.h"
#include "board.h"
#include "score.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // get path for file mode (-f option)
    char* path = file_mode(argc, argv);

    Board board;

    // FILE MODE
    if(path != NULL)
    {
        printf("\n\tFile path: %s\n\n", path);

        // initialize the board
        board = file_board_init(path);

        // 
        file_commands(path, &board);

        board_print(&board, false);

        DEBUG_PRINT("\n\t--- FILE MODE ---");
    }
    // NORMAL MODE
    else
    {
        DEBUG_PRINT("\n\t--- NORMAL MODE ---");
        
        // get difficulty
        Difficulty difficulty = board_difficulty();

        // initialize the board
        board = board_init(difficulty);

        // print board stats
        board_stats(&board);

        // seed the random number generator with the current time 
        srand(time(NULL));

        // print board
        board_print(&board, true);

        // ask for commands
        board_commands(&board);

        // get the name and write the score to the scoreboard
        score_name(board.score);
        
        // show the scoreboard
        score_print();
    }

    // free board memory
    board_free(&board);

    return EXIT_SUCCESS;
}