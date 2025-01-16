#include "debug.h"
#include "score.h"
#include "board.h"

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define SCOREBOARD_FILE "scoreboard.txt"

// get the name and write it to the scoreboard
void score_name(int score)
{
    char name[MAX_NAME_LENGTH];

    printf("\tEnter your name: ");
    scanf("%49s", name);

    // remove newline character from the end of the name
    size_t len = strlen(name);
    if(len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }

    // open the file
    FILE *file = fopen(SCOREBOARD_FILE, "a");
    if(file == NULL)
    {
        perror("\tFailed to open the scoreboard!\n");
        return;
    }

    // write the name and the score
    fprintf(file, "%s %d\n", name, score);

    // close the fie
    fclose(file);

    // feedback
    printf("\033[A\33[2K");
    printf("\tYour name was saved successfully!\n\n");
}