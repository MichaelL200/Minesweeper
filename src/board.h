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
} Difficulty;

// Game board structure
typedef struct
{
    Difficulty difficulty;
    int rows;
    int cols;
    int mines;
    bool* isMine;
    int* adjacentMines;
    bool* isRevealed;
    bool* isFlagged;
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
board_random(Board* board);

// print board
void board_print(Board* board);

// update the board
void board_update(Board* board, int line_plus);

// ask for commands
void board_commands(Board* board);

// free board memory
void board_free(Board* board);

#endif // BOARD_H