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

    // for -f option
    while((opt = getopt(argc, argv, "f:" )) != -1)
    {
        // try to get the file path from the command line
        if(opt == 'f')
        {
            // get the argument value
            path = optarg;
            process_path(path);

            // check if the file exists
            if(access(path, F_OK) == -1)
            {
                printf("\tFile %s does not exist. Try again.\n", path);
                path = NULL;
            }
        }

        // if wrong, try to get the file path from the user
        while(path == NULL)
        {
            printf("\tEnter the file path: ");
            char input[256];
            if(fgets(input, sizeof(input), stdin) != NULL)
            {
                size_t len = strlen(input);
                if(len > 0 && input[len - 1] == '\n')
                {
                    input[--len] = '\0';
                }

                process_path(input);
                if(access(input, F_OK) != -1)
                {
                    path = strdup(input);

                }
                else
                {
                    printf("\tFile %s does not exist. Try again.\n", input);
                }
            }
        }
    }

    // FILE MODE
    if(path != NULL)
    {
        printf("\tFile path: %s\n", path);

        printf("\tFILE MODE\n");
    }
    // NORMAL MODE
    else
    {
        printf("\tNORMAL MODE\n");
    }

    return EXIT_SUCCESS;
}