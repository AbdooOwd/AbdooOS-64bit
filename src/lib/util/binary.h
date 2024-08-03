#pragma once

#include <include/types.h>

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

#define SET_BIT(value, bit) ((value) |= ((u64)1 << (bit)))
#define CLEAR_BIT(value, bit) ((value) &= ~((u64)1 << (bit)))
#define TEST_BIT(value, bit) (((value) & ((u64)1 << (bit))) != 0)

// Ingenius
static inline void bit_set(u64* value, u64 bit) {
    *value |= ((u64) 1 << (bit % 8));
}

static inline void bit_clear(u64* value, uint64_t bit) {
    *value &= ~((u64) 1 << (bit % 8)); // basically clear the bit
}

static inline bool bit_get(u64* value, uint64_t bit) {
    return *value & ((u64) 1 << (bit % 8));
}
