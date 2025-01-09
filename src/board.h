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

// initialize board
Board board_init(Difficulty difficulty);

// prints status of the board
void board_stats(Board* board);

// free board memory
void board_free(Board* board);

#endif // BOARD_H