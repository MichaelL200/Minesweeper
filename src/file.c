#include "file.h"
#include "board.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// process the file path
void file_process_path(char* path)
{
    // replace backslashes with forward slashes
    for(char* p = path; *p != '\0'; p++)
    {
        if(*p == '\\')
        {
            *p = '/';
        }
    }

    // remove duplicates slashes
    for(char* p = path; *p != '\0'; p++)
    {
        if(*p == '/' && *(p + 1) == '/')
        {
            for(char* q = p + 1; *q != '\0'; q++)
            {
                *q = *(q + 1);
            }
            p--;
        }
    }

    // remove slashes at the beginning
    while(*path == '/')
    {
        for(char* p = path; *p != '\0'; p++)
        {
            *p = *(p + 1);
        }
    }

    // remove slashes at the end
    for(char* p = path; *p != '\0'; p++)
    {
        if(*p == '/' && *(p + 1) == '\0')
        {
            *p = '\0';
        }
    }    
}

// check if file exists
bool file_exists(const char* path)
{
    return access(path, F_OK) != -1;
}

// get path for file mode (-f option)
char* file_mode(int argc, char *argv[])
{
    int opt;
    char* path = NULL;
    int lines = 0;

    while((opt = getopt(argc, argv, "f:" )) != -1)
    {
        // try to get the file path from the command line
        if(opt == 'f')
        {
            // get the argument value
            path = optarg;
            file_process_path(path);

            // check if the file exists
            if(!file_exists(path))
            {
                printf("\033[A\33[2K");
                printf("\n\tFile %s does not exist. Try again.\n", path);
                lines++;
                path = NULL;
            }
        }

        // if wrong, try to get the file path from the user
        while(path == NULL)
        {
            printf("\tEnter the file path: ");
            lines++;
            char input[256];
            if(fgets(input, sizeof(input), stdin) != NULL)
            {
                size_t len = strlen(input);
                if(len > 0 && input[len - 1] == '\n')
                {
                    input[--len] = '\0';
                }

                file_process_path(input);
                if(file_exists(input) == true)
                {
                    path = strdup(input);

                }
                else
                {
                    printf("\033[A\33[2K");
                    if(lines)
                    {
                        printf("\033[A\33[2K");
                        lines--;
                    }
                    printf("\tFile %s does not exist. Try again.\n", input);
                }
            }
        }
    }

    for(int i = 0; i < lines; i++)
    {
        printf("\033[A\33[2K");
    }
    if(lines)
    {
        printf("\033[A\33[2K");
    }

    return path;
}

// initialize board for file mode (without difficulty and isFirstRevealed)
Board file_board_init(char* path)
{
    Board board;

    // open the file
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "\tError when opening file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    // get the rows, cols, mines from the path
    if (fscanf(file, "%d %d %d", &board.rows, &board.cols, &board.mines) != 3)
    {
        fprintf(stderr, "\tError when reading the board data: %s (r, c, m)\n", path);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    printf("\tRows: %d\n\tColumns: %d\n", board.rows, board.cols);
    printf("\tTotal cells: %d\n", board.rows * board.rows);
    printf("\tMines: %d (%d%%)\n",
           board.mines, (board.mines * 100) / (board.rows * board.cols));

    // initialize adjacentMines and isMine
    board.adjacentMines = (int*)malloc(board.rows * board.cols * sizeof(int));
    board.isMine = (bool*)malloc(board.rows * board.cols * sizeof(bool));
    if (!board.adjacentMines || !board.isMine)
    {
        fprintf(stderr, "\tMemory allocation failed!\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // assign adjacentMines and isMine values
    for (int r = 0; r < board.rows; r++)
    {
        for (int c = 0; c < board.cols; c++)
        {
            int index = c + r * board.cols;

            char element[3];
            if (fscanf(file, "%s", element) != 1)
            {
                fprintf(stderr, "\tError when reading board data at (%d, %d)\n", r, c);
                fclose(file);
                exit(EXIT_FAILURE);
            }

            if (isdigit(element[0]))
            {
                board.adjacentMines[index] = atoi(element);
                board.isMine[index] = false;
            }
            else if (element[0] == 'X')
            {
                board.adjacentMines[index] = 0;
                board.isMine[index] = true;
            }
            else
            {
                fprintf(stderr, "\tInvalid element: %s at (%d, %d)\n", element, r, c);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    // initialize isRevealed and isFlagged
    board.isRevealed = (bool*)calloc(board.rows * board.cols, sizeof(bool));
    board.isFlagged = (bool*)calloc(board.rows * board.cols, sizeof(bool));

    board.score = 0;

    // calculate multiplier
    int cells = board.rows * board.cols;
    int percentage = (board.mines * 100) / cells;
    board.multiplier = 1;
    if(cells >= 81 && percentage >= 12)
    {
        board.multiplier++;
        if(cells >= 256 && percentage >= 15)
        {
            board.multiplier++;
            if(cells >= 480 && percentage >= 20)
            {
                board.multiplier++;
            }
        }
    }
    printf("\tMultiplier: %d\n", board.multiplier);

    return board;
}

// read commands and update the board
void file_commands(char* path, Board* board)
{
    printf("\n");

    // open the file
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "\tError when opening file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    // skip the board data
    int rows, cols, mines;
    if (fscanf(file, "%d %d %d", &rows, &cols, &mines) != 3)
    {
        fprintf(stderr, "\tError when reading the board data: %s (r, c, m)\n", path);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // skip the newline
    fgetc(file);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            char element = '\0';
            if (fscanf(file, " %c", &element) != 1)
            {
                fprintf(stderr, "\tError when reading board data at (%d, %d)\n", r, c);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        // skip the newline at the end of each row
        fgetc(file);
    }

    // read and process commands from the file
    char comm;
    int x, y;
    bool game_over = false;
    int result = 0;

    while(fscanf(file, " %c %d %d", &comm, &x, &y) == 3 && !game_over)
    {
        // sleep for a while
        usleep(150000);

        int index = y * board->cols + x;

        if(comm == 'r')
        {
            if(board->isRevealed[index] == true)
            {
                printf("\tCell (%d, %d) already revealed.\n", x, y);
            }
            else if(board->isFlagged[index] == true)
            {
                printf("\tCell (%d, %d) is already flagged. Cant' reveal.\n", x, y);
            }
            else
            {
                printf("\tReveal cell (%d, %d)\n", x, y);
                board->isRevealed[index] = true;
                board->score += board->multiplier;
                if(board->isMine[index] == true)
                {
                    board->score -= board->multiplier;
                    board_reveal_all(board);
                    result = -1;
                    break;
                }
                else if(board->adjacentMines[index] == 0)
                {
                    board_reveal(board, x, y);
                }
                if(board_check_win(board))
                {
                    board_reveal_all(board);
                    result = 1;
                    break;
                }
            }
        }
        else if(comm == 'f')
        {
            if (board->isRevealed[index] == true)
            {
                printf("\tCell already revealed. Please try again.\n");
            }
            else if (board->isFlagged[index] == true)
            {
                board->isFlagged[index] = false;
            }
            else
            {
                board->isFlagged[index] = true;
            }
        }
        else
        {
            fprintf(stderr, "\tUnknown command: %c\n", comm);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    if (!feof(file))
    {
        fprintf(stderr, "\tError when reading commands from file: %s\n", path);
    }

    // close the file
    fclose(file);

    // check the result
    printf("\n");
    if(result == 1)
    {
        printf("\tWin!\n\n");
    }
    else if(result == -1)
    {
        printf("\tGame over! Hit a mine.\n\n");
    }
    else
    {
        printf("\tNo win, no loss...\n\n");
    }
}