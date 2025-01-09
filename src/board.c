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

// initialize board
Board board_init(Difficulty difficulty)
{
    Board board;

    board.difficulty = difficulty;

    if(difficulty.name == CUSTOM.name)
    {
        int tried = 0;
        printf("\tEnter number of rows: ");
        scanf("%d", &board.rows);
        while(board.rows < 1 || board.rows > 100)
        {
            printf("\033[A\33[2K");
            if(tried > 0)
            {
                printf("\033[A\33[2K");
            }
            tried = 1;
            printf("\tInvalid number of rows. Please try again.\n");
            printf("\tEnter number of rows: ");
            scanf("%d", &board.rows);
        }

        tried = 0;
        printf("\tEnter number of columns: ");
        scanf("%d", &board.cols);
        while(board.cols < 1 || board.cols > 100)
        {
            printf("\033[A\33[2K");
            if(tried > 0)
            {
                printf("\033[A\33[2K");
            }
            tried = 1;
            printf("\tInvalid number of columns. Please try again.\n");
            printf("\tEnter number of columns: ");
            scanf("%d", &board.cols);
        }

        tried = 0;
        int max_mines = board.rows * board.cols - 1;
        printf("\tEnter number of mines: ");
        scanf("%d", &board.mines);
        while(board.mines < 1 || board.mines > max_mines)
        {
            printf("\033[A\33[2K");
            if(tried > 0)
            {
                printf("\033[A\33[2K");
            }
            tried = 1;
            printf("\tInvalid number of mines. Please try again.\n");
            printf("\tEnter number of mines: ");
            scanf("%d", &board.mines);
        }
        for(int i = 0; i < 3 + tried; i++)
        {
            printf("\033[A\33[2K");
        }
    }
    else
    {
        board.rows = difficulty.rows;
        board.cols = difficulty.cols;
        board.mines = difficulty.mines;
    }

    board.isMine = (bool*)malloc(board.rows * board.cols * sizeof(bool));
    board.adjacentMines = (int*)malloc(board.rows * board.cols * sizeof(int));
    board.isRevealed = (bool*)malloc(board.rows * board.cols * sizeof(bool));
    board.isFlagged = (bool*)malloc(board.rows * board.cols * sizeof(bool));

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

// free board memory
void board_free(Board* board)
{
    free(board->isMine);
    free(board->adjacentMines);
    free(board->isRevealed);
    free(board->isFlagged);
}