#include "debug.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>

// assign values to difficulty levels
Difficulty EASY = {"Easy", 9, 9, 10, 1};
Difficulty MEDIUM = {"Medium", 16, 16, 40, 2};
Difficulty HARD = {"Hard", 16, 30, 99, 3};
Difficulty CUSTOM = {"Custom", 0, 0, 0, 1};

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
    // ask for rows
    int tried = 0;
    while(board->rows < 4 || board->rows > 100)
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

    // ask for columns
    tried = 0;
    while(board->cols < 4 || board->cols > 100)
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

    // ask for number of mines
    tried = 0;
    int max_mines = board->rows * board->cols - 9;
    board->mines = 0;
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

    // calculate multiplier
    int cells = board->rows * board->cols;
    int percentage = (board->mines * 100) / cells;
    board->multiplier = 1;
    if(cells >= 81 && percentage >= 12)
    {
        board->multiplier++;
        if(cells >= 256 && percentage >= 15)
        {
            board->multiplier++;
            if(cells >= 480 && percentage >= 20)
            {
                board->multiplier++;
            }
        }
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
        board.multiplier = difficulty.multiplier;
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
    board.isFirstRevealed = false;
    board.score = 0;

    return board;
}

// print stats of the board
void board_stats(Board* board)
{
    printf("\tRows: %d\n", board->rows);
    printf("\tColumns: %d\n", board->cols);
    printf("\tTotal cells: %d\n", board->rows * board->cols);
    int percentage = (board->mines * 100) / (board->rows * board->cols);
    printf("\tMines: %d (%d%%) \n", board->mines, percentage);
    printf("\tMultiplier: %d \n", board->multiplier);
}

// random board
void board_random(Board* board, int first_x, int first_y)
{
    // randomize mines
    int mines_placed = 0;
    while(mines_placed < board->mines)
    {
        int x = rand() % board->cols;
        int y = rand() % board->rows;
        int index = x + y * board->cols;

        /* place the mines only if it's not in the first index *
         * and there are no mines in the closest area          */
        if(board->isMine[index] == false &&
           !(x >= first_x - 1 && x <= first_x + 1
           && y >= first_y - 1 && y <= first_y + 1))
        {
            board->isMine[index] = true;
            mines_placed++;
        }
    }

    // calculate adjacent mines for each cell
    for(int c = 0; c < board->cols; c++)
    {
        for(int r = 0; r < board->rows; r++)
        {
            int index = r * board->cols + c;
            if(board->isMine[index] == false)
            {
                int count = 0;
                for(int k = -1; k <= 1; k++)
                {
                    for(int l = -1; l <= 1; l++)
                    {
                        int x = r + k;
                        int y = c + l;
                        if(x >= 0 && x < board->cols && y >= 0 && y < board->rows)
                        {
                            int index2 = y + x * board->rows;
                            if(board->isMine[index2] == true)
                            {
                                count++;
                            }
                        }
                    }
                }
                board->adjacentMines[index] = count;
            }
        }
    }
}

// print board
void board_print(Board* board, bool isScore)
{
    // print column headers
    printf("\n\t    ");
    for (int col = 0; col < board->cols; col++)
    {
        printf("%2d ", col);
    }
    printf("\n\t    ");
    for (int col = 0; col < board->cols; col++)
    {
        printf("---");
    }
    printf("\n");

    // print rows with row headers
    for (int row = 0; row < board->rows; row++)
    {
        printf("\t%2d |", row);
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

    // show the score
    if(isScore)
    {
        printf("\tScore: %d\n\n", board->score);
    }
}

// update the board
void board_update(Board* board, int line_plus, bool isScore)
{
    // delete lines
    for(int i = 0; i < board->rows + 6 + line_plus; i++)
    {
        printf("\033[A\33[2K");
    }
    if(line_plus > 0)
    {
        printf("\033[A\33[2K");
    }

    // print the board
    if(isScore)
    {
        board_print(board, true);
    }
}

// reveals all the neighboring cells with no mines
void board_reveal(Board* board, int x, int y)
{
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int x2 = x + i;
            int y2 = y + j;
            if(x2 >= 0 && x2 < board->cols && y2 >= 0 && y2 < board->rows)
            {
                int index = board->cols * y2 + x2;
                if(board->isRevealed[index] == false)
                {
                    board->isRevealed[index] = true;
                    board->score += board->multiplier;
                    if(board->adjacentMines[index] == 0)
                    {
                        // recurrence
                        board_reveal(board, x2, y2);
                    }
                }
            }
        }
    }
}

// reveal all the cells
void board_reveal_all(Board* board)
{
    for(int id = 0; id < board->rows * board->cols; id++)
    {
        board->isRevealed[id] = true;
    }
}

// check if the player has won
int board_check_win(Board* board)
{
    return board->score == board->multiplier * (board->cols * board->rows - board->mines);
}

// ask for commands
void board_commands(Board* board)
{
    while(true)
    {
        char comm = '\0';
        int x, y;
        int try = 0;

        // clear the buffer
        while (getchar() != '\n');
        // get the command
        printf("\tEnter a command: ");
        if(scanf(" %c %d %d", &comm, &x, &y) == 3)
        {
            if (comm == 'r' || comm == 'f')
            {
                printf("\033[A\33[2K");
                DEBUG_PRINT("\tCOMM = %c\tX = %d\tY = %d\n", comm, x, y);
                if (y >= 0 && y < board->rows && x >= 0 && x < board->cols)
                {
                    int index = y * board->cols + x;

                    if (comm == 'r')
                    {
                        /* the first revealed cell has to be without mine *
                         * and is not to have mines nearby                */
                        if(!board->isFirstRevealed)
                        {
                            board_random(board, x, y);
                            board->isFirstRevealed = true;

                            // a case when there is a win after the first command
                            if(board_check_win(board))
                            {
                                board_reveal_all(board);
                                board_update(board, try, true);
                                printf("\tYou won!\n\n");
                                break;
                            }
                        }

                        if(board->isRevealed[index] == true)
                        {
                            printf("\tCell already revealed. Please try again.\n");
                            try = 1;
                        }
                        else if(board->isFlagged[index] == true)
                        {
                            printf("\tThe cell is flagged. Unflag to reveal.\n");
                            try = 1;
                        }
                        else
                        {
                            board->isRevealed[index] = true;
                            board->score += board->multiplier;
                            if(board->isMine[index] == true)
                            {
                                board->score -= board->multiplier;
                                board_reveal_all(board);
                                board_update(board, try, true);
                                printf("\tGame over! You hit a mine.\n\n");
                                break;
                            }
                            else if(board->adjacentMines[index] == 0)
                            {
                                board_reveal(board, x, y);
                            }
                            board_update(board, try, true);
                            if(board_check_win(board))
                            {
                                board_reveal_all(board);
                                board_update(board, try, true);
                                printf("\tYou won!\n\n");
                                break;
                            }
                        }
                    }
                    else if (comm == 'f')
                    {
                        if(board->isRevealed[index] == true)
                        {
                            printf("\tCell already revealed. Please try again.\n");
                            try = 1;
                        }
                        else if(board->isFlagged[index] == true)
                        {
                            board->isFlagged[index] = false;
                            board_update(board, try, true);
                        }
                        else
                        {
                            board->isFlagged[index] = true;
                            board_update(board, try, true);
                        }
                    }
                }
                else
                {
                    printf("\033[A\33[2K");
                    printf("\tCoordinates out of bounds. Please try again.\n");
                    try = 1;
                }
            }
            else
            {
                printf("\033[A\33[2K");
                if(try)
                {
                    printf("\033[A\33[2K");
                }
                printf("\tInvalid command. Please use 'r' or 'f' followed by coordinates.\n");
                try = 1;
            }
        }
        else
        {
            printf("\033[A\33[2K");
            if(try)
            {
                printf("\033[A\33[2K");
            }
            printf("\tInvalid command format. Please enter command as 'r x y' or 'f x y'.\n");
            try = 1;
        }
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