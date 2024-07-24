#ifndef BITMAP_ALLOCATOR_H
#define BITMAP_ALLOCATOR_H
#pragma once

#include <include/types.h>


#define malloc 	allocation
#define free	freebird


#define BLOCK_SIZE 	1024 * 4         			// Size of each memory block (4KiB)

// Utility macros for setting and checking bits in the bitmap
#define BIT_SET(bitmap, n) (bitmap[(n) / 8] |= (1 << ((n) % 8)))
#define BIT_CLEAR(bitmap, n) (bitmap[(n) / 8] &= ~(1 << ((n) % 8)))
#define BIT_CHECK(bitmap, n) (bitmap[(n) / 8] & (1 << ((n) % 8)))

extern size_t blocks_count;
extern size_t mem_size;

extern u8* bitmap; // The bitmap array
extern u64 used_bits;

typedef struct {
	u64 base;
	size_t count;
	struct mem_entry_t** entries;
} mem_t;

typedef struct {
	u64 base;
	u64 size;
	struct mem_entry_t* next_entry;
} mem_entry_t;

// functions

void* allocation(size_t size);
int freebird(void* ptr);

void* bitmap_malloc(size_t size);
void bitmap_free(void* ptr);
void bitmap_init();

u64 get_hhdm();

#endif