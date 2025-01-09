#include "file.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // get path for file mode (-f option)
    char* path = file_mode(argc, argv);

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

        printf("\tChoose a difficulty level:\n");
        printf("\t1. Easy\n");
        printf("\t2. Medium\n");
        printf("\t3. Hard\n");
    }

    return EXIT_SUCCESS;
}