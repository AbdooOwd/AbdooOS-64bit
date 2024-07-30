#ifndef UTIL_H
#define UTIL_H
#pragma once

#include <include/types.h>


#define PAGE_SIZE 4096

#define VEC_TYPE(type)                                                         \
    struct {                                                                   \
        type*    data;                                                         \
        uint64_t size;                                                         \
        uint64_t capacity;                                                     \
    }

#define MIN(A, B)                                                              \
    ({                                                                         \
        __auto_type MIN_a = A;                                                 \
        __auto_type MIN_b = B;                                                 \
        MIN_a < MIN_b ? MIN_a : MIN_b;                                         \
    })

#define MAX(A, B)                                                              \
    ({                                                                         \
        __auto_type MAX_a = A;                                                 \
        __auto_type MAX_b = B;                                                 \
        MAX_a > MAX_b ? MAX_a : MAX_b;                                         \
    })

#define DIV_ROUNDUP(VALUE, DIV)                                                \
    ({                                                                         \
        __auto_type DIV_ROUNDUP_value = VALUE;                                 \
        __auto_type DIV_ROUNDUP_div = DIV;                                     \
        (DIV_ROUNDUP_value + (DIV_ROUNDUP_div - 1)) / DIV_ROUNDUP_div;         \
    })

#define CEIL_DIV(x, y) (x + y - 1) / y

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

#define SIZEOF_ARRAY(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))


void array_copy(unsigned char* source, unsigned char* dest);
char* int_to_str(int num);

int random();


// throw random stuff as arguments so u don't let stuff unused (foq u GCC)
static inline void UNUSED(u64, ...) {}

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

#endif