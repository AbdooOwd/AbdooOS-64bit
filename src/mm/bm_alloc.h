#ifndef BITMAP_ALLOCATOR_H
#define BITMAP_ALLOCATOR_H
#pragma once

#include <include/types.h>


#define malloc 	bitmap_malloc
#define free	bitmap_free


#define MEMORY_SIZE 1024 * 1024 // Total memory managed by the allocator (1MB)
#define BLOCK_SIZE 4096         // Size of each memory block (4KB)
#define NUM_BLOCKS (MEMORY_SIZE / BLOCK_SIZE) // Total number of blocks

u8 memory[MEMORY_SIZE]; // The memory array
u8 bitmap[NUM_BLOCKS / 8]; // The bitmap array

// Utility macros for setting and checking bits in the bitmap
#define BIT_SET(bitmap, n) (bitmap[(n) / 8] |= (1 << ((n) % 8)))
#define BIT_CLEAR(bitmap, n) (bitmap[(n) / 8] &= ~(1 << ((n) % 8)))
#define BIT_CHECK(bitmap, n) (bitmap[(n) / 8] & (1 << ((n) % 8)))


// functions

void* bitmap_malloc(size_t size);
void bitmap_free(void* ptr);


#endif