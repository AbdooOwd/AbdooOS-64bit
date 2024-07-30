#ifndef TYPES_H
#define TYPES_H

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


// Ah yes short typenames
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t	u64;

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef u64     	addr_t;		// cuz we're in 64bits
typedef char*       string;

typedef int         error_code;

typedef struct {
    int x;
    int y;
} Vector2;

#endif