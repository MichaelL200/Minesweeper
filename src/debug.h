#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

// define debug macro
#ifdef DEBUG // -DDEBUG flag
    #define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: " fmt "\n", ##args)
#else
    #define DEBUG_PRINT(fmt, args...) // no-op
#endif

#endif // DEBUG_H