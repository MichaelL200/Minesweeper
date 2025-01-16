#include "debug.h"
#include "score.h"
#include "board.h"

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define SCOREBOARD_FILE "data/scoreboard.txt"

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

// print the scoreboard - results of the 5 best players
void score_print()
{
    // open the scoreboard
    FILE *file = fopen(SCOREBOARD_FILE, "r");
    if (file == NULL)
    {
        perror("\tFailed to open the scoreboard!\n");
        return;
    }

    char name[MAX_NAME_LENGTH];
    int score;
    int scores[5] = {0};
    char names[5][MAX_NAME_LENGTH] = {0};

    // read and assign the 5 biggest scores
    int count = 0;
    while (fscanf(file, "%49s %d", name, &score) == 2)
    {
        for (int i = 0; i < 5; i++)
        {
            if (score > scores[i])
            {
                for (int j = 4; j > i; j--)
                {
                    scores[j] = scores[j - 1];
                    strcpy(names[j], names[j - 1]);
                }
                scores[i] = score;
                strcpy(names[i], name);
                if (count < 5) count++;
                break;
            }
        }
    }

    // print the scoreboard
    printf("\n\tScoreboard:\n");
    for (int i = 0; i < count; i++)
    {
        printf("\t%d. %s - %d\n", i + 1, names[i], scores[i]);
    }
    printf("\n");

    fclose(file);
}