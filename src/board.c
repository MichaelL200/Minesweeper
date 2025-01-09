#include "debug.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>

// assign values to difficulty levels
Difficulty EASY = {"Easy", 9, 9, 10};
Difficulty MEDIUM = {"Medium", 16, 16, 40};
Difficulty HARD = {"Hard", 16, 30, 99};
Difficulty CUSTOM = {"Custom", 0, 0, 0};

// get difficulty
Difficulty board_difficulty()
{
    Difficulty difficulty;

    printf("\n\tChoose a difficulty level:\n");
        printf("\t1. Easy\n");
        printf("\t2. Medium\n");
        printf("\t3. Hard\n");
        printf("\t4. Custom\n");
        int tried = 0;
        while(true)
        {
            if(tried > 0)
            {
                // delete the current line
                printf("\033[A\33[2K");
                printf("\tInvalid choice. Please try again.\n");

                // clear input buffer
                while(getchar() != '\n');
            }
            tried = 1;

            printf("\tEnter a number: ");
            int input;
            scanf("%d", &input);
            if(input >= 1 && input <= 4)
            {
                // delete last 5 / 6 lines
                for(int i = 0; i < 5 + tried; i++)
                {
                    printf("\033[A\33[2K");
                }

                switch(input)
                {
                    case 1:
                        difficulty = EASY;
                        break;
                    case 2:
                        difficulty = MEDIUM;
                        break;
                    case 3:
                        difficulty = HARD;
                        break;
                    case 4:
                        difficulty = CUSTOM;
                        break;
                }
                break;
            }
            else
            {
                // delete the current line
                printf("\033[A\33[2K");
            }
        }

        printf("\tChosen difficulty: %s\n", difficulty.name);

        return difficulty;
}

// ask user for custom board
void board_custom(Board* board)
{
    int tried = 0;
    while(board->rows < 1 || board->rows > 100)
    {
        if(tried > 0)
        {
            printf("\033[A\33[2K");
            if(tried > 1)
            {
                printf("\033[A\33[2K");
            }
            printf("\tInvalid number of rows. Please try again.\n");
        }
        printf("\tEnter number of rows: ");
        // check for a number
        if(scanf("%d", &board->rows) != 1)
        {
            // clear input buffer
            while(getchar() != '\n');
        }
        tried++;
    }

    tried = 0;
    while(board->cols < 1 || board->cols > 100)
    {
        if(tried > 0)
        {
            printf("\033[A\33[2K");
            if(tried > 1)
            {
                printf("\033[A\33[2K");
            }
            printf("\tInvalid number of columns. Please try again.\n");
        }
        printf("\tEnter number of columns: ");
        if(scanf("%d", &board->cols) != 1)
        {
            while(getchar() != '\n');
        }
        tried++;
    }

    tried = 0;
    int max_mines = board->rows * board->cols - 1;
    while(board->mines < 1 || board->mines > max_mines)
    {
        if(tried > 0)
        {
            printf("\033[A\33[2K");
            if(tried > 1)
            {
                printf("\033[A\33[2K");
            }
            printf("\tInvalid number of mines. Please try again.\n");
        }
        printf("\tEnter number of mines: ");
        if(scanf("%d", &board->mines) != 1)
        {
            while(getchar() != '\n');
        }
        tried++;
    }
    for(int i = 0; i < 3 + tried; i++)
    {
        printf("\033[A\33[2K");
    }
}

// initialize board
Board board_init(Difficulty difficulty)
{
    Board board;

    board.difficulty = difficulty;

    if(difficulty.name == CUSTOM.name)
    {
        board_custom(&board);
    }
    else
    {
        board.rows = difficulty.rows;
        board.cols = difficulty.cols;
        board.mines = difficulty.mines;
    }

    board.isMine = (bool*)malloc(board.rows * board.cols * sizeof(bool));
    for(int i = 0; i < board.rows * board.cols; i++)
    {
        board.isMine[i] = false;
    }
    board.adjacentMines = (int*)malloc(board.rows * board.cols * sizeof(int));
    for(int i = 0; i < board.rows * board.cols; i++)
    {
        board.adjacentMines[i] = 0;
    }
    board.isRevealed = (bool*)malloc(board.rows * board.cols * sizeof(bool));
    for(int i = 0; i < board.rows * board.cols; i++)
    {
        board.isRevealed[i] = false;
    }
    board.isFlagged = (bool*)malloc(board.rows * board.cols * sizeof(bool));
    for(int i = 0; i < board.rows * board.cols; i++)
    {
        board.isFlagged[i] = false;
    }

    return board;
}

// prints status of the board
void board_stats(Board* board)
{
    printf("\tRows: %d\n", board->rows);
    printf("\tColumns: %d\n", board->cols);
    printf("\tTotal cells: %d\n", board->rows * board->cols);
    int percentage = (board->mines * 100) / (board->rows * board->cols);
    printf("\tMines: %d (%d%%) \n", board->mines, percentage);
}

// print board
void board_print(Board* board)
{
    // print column headers
    printf("\n\t   ");
    for (int col = 0; col < board->cols; col++)
    {
        printf("%2d ", col);
    }
    printf("\n\t   ");
    for (int col = 0; col < board->cols; col++)
    {
        printf("---");
    }
    printf("\n");

    // print rows with row headers
    for (int row = 0; row < board->rows; row++)
    {
        printf("\t%2d| ", row);
        for (int col = 0; col < board->cols; col++)
        {
            int index = row * board->cols + col;
            if (board->isRevealed[index])
            {
                if (board->isMine[index])
                {
                    printf(" X ");
                }
                else
                {
                    if(board->adjacentMines[index] == 0)
                    {
                        printf("   ");
                    }
                    else
                    {
                        printf(" %d ", board->adjacentMines[index]);
                    }
                }
            }
            else
            {
                if(board->isFlagged[index])
                {
                    printf(" F ");
                }
                else
                {   
                    printf(" □ ");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

// ask for a command
void board_command(Board* board)
{
    printf("\tEnter a command: ");
    char command[10];
    scanf("%s", command);
    // get the first string
    if(strcmp(command, "f") == 0)
    {
        DEBUG_PRINT("\t--- FLAG COMMAND ---\n");
        printf("\tFlag command\n");
    }
    else if(strcmp(command, "r") == 0)
    {
        DEBUG_PRINT("\t--- REVEAL COMMAND ---\n");
        printf("\tReveal command\n");
    }
    else
    {
        DEBUG_PRINT("\t--- INVALID COMMAND ---\n");
        printf("\tInvalid command.\n");
    }

}

// free board memory
void board_free(Board* board)
{
    free(board->isMine);
    free(board->adjacentMines);
    free(board->isRevealed);
    free(board->isFlagged);
}