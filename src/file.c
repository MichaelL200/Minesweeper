#include "file.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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