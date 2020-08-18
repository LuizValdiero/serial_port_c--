#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

typedef struct buffer_t {
    uint32_t buffer_size;
    unsigned char * buffer;
} buffer_t;

#define MAX_BUFFER_SIZE 100

#define STRING_SYNCHRONIZE "XXX"
#define STRING_SYNCHRONIZE_SIZE sizeof(STRING_SYNCHRONIZE)

#endif // DEFINES_H