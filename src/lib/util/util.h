#ifndef UTIL_H
#define UTIL_H
#pragma once

#include <include/types.h>

typedef struct {
    int x;
    int y;
} __attribute__((packed)) Vector2;

void array_copy(unsigned char* source, unsigned char* dest);
char* int_to_str(int num);

int random();


// throw random stuff as arguments so u don't let stuff unused (foq u GCC)
static inline void UNUSED(u64, ...) {}

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

#endif