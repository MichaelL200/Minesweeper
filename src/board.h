#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdbool.h>

// Difficulty levels structure
typedef struct
{
    char* name;
    int rows;
    int cols;
    int mines;
    int multiplier;
} Difficulty;

// Game board structure
typedef struct
{
    Difficulty difficulty;
    int rows;
    int cols;
    int mines;
    int multiplier;
    bool* isMine;
    int* adjacentMines;
    bool* isRevealed;
    bool* isFlagged;
    bool isFirstRevealed;
    int score;
} Board;

// declare difficulty levels
extern Difficulty EASY;
extern Difficulty MEDIUM;
extern Difficulty HARD;
extern Difficulty CUSTOM;

// set difficulty
Difficulty board_difficulty();

// ask user for custom board
void board_custom(Board* board);

// initialize board
Board board_init(Difficulty difficulty);

// print stats of the board
void board_stats(Board* board);

// random board
void board_random(Board* board, int first_x, int first_y);

// print board
void board_print(Board* board, bool isScore);

// update the board
void board_update(Board* board, int line_plus);

// reveals all the neighboring cells with no mines
void board_reveal(Board* board, int x, int y);

// reveal all the cells
void board_reveal_all(Board* board);

// check if the player has won
int board_check_win(Board* board);

// ask for commands
void board_commands(Board* board);

// free board memory
void board_free(Board* board);

#endif // BOARD_H