#ifndef UTIL_H
#define UTIL_H
#pragma once

#include <include/types.h>
#include <interrupts/pit.h>


#define PAGE_SIZE 4096


#define CEIL_DIV(x, y) (x + y - 1) / y
#define FLOOR_DIV(x, y) (x - (x % y)) / y

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

#define SIZEOF_ARRAY(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))


#define LCG_A 1664525
#define LCG_C 1013904223
#define LCG_M 0xFFFFFFFF


static int random() {
    return (ticks ^ (ticks >> 21)) * 0x5bd1e5; // idk what i'm doing
}

static u32 rand_range(u32 min_value, u32 max_value) {
    static u32 seed = 1;
    seed = (LCG_A * seed + LCG_C) & LCG_M;
    return min_value + (seed % (max_value - min_value + 1));
}

static void array_copy(unsigned char* source, unsigned char* dest) {
    for (size_t i = 0; i < sizeof(source); i++) {
        dest[i] = source[i];
    }
}


// throw random stuff as arguments so u don't let stuff unused (foq u GCC)
static inline void UNUSED(u64, ...) {}

#define MIN(a, b)    ((a) < (b) ? (a) : (b))
#define MAX(a, b)    ((a) > (b) ? (a) : (b))

#endif