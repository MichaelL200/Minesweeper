#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// process the file path
void process_path(char* path)
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

int main(int argc, char *argv[])
{
    int opt;
    char* path = NULL;

    // try to get the file path from the command line
    while((opt = getopt(argc, argv, "f:" )) != -1)
    {
        if(opt == 'f')
        {
            // get the argument value
            path = optarg;
            printf("\tFile path: %s\n", path);
            process_path(path);

            // check if the file exists
            if(access(path, F_OK) == -1)
            {
                printf("\tFile does not exist\n");
                path = NULL;
            }
        }
    }

    // try to get the file path from the user
    while(path == NULL)
    {
        printf("\tEnter the file path: ");
        char input[256];
        if(scanf("%255s", input) == 1)
        {
            process_path(input);
            if(access(input, F_OK) != -1)
            {
                path = strdup(input);
            }
            else
            {
                printf("\tFile does not exist. Try again.\n");
            }
        }
    }

    printf("\tFile path: %s\n", path);

    return EXIT_SUCCESS;
}