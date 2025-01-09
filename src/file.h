#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

// process the file path
void file_process_path(char* path);

// check if file exists
bool file_exists(const char* path);

// get path for file mode (-f option)
char* file_mode(int argc, char *argv[]);

#endif