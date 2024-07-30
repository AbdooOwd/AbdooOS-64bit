#ifndef PMM_H
#define PMM_H
#pragma once

#include <include/types.h>


#define malloc 	alloc
#define mfree	free


#define BLOCK_SIZE 	1024 * 4         			// Size of each memory block (4KiB)

// Utility macros for setting and checking bits in the bitmap
#define BIT_SET(bitmap, n) (bitmap[(n) / 8] |= (1 << ((n) % 8)))
#define BIT_CLEAR(bitmap, n) (bitmap[(n) / 8] &= ~(1 << ((n) % 8)))
#define BIT_CHECK(bitmap, n) (bitmap[(n) / 8] & (1 << ((n) % 8)))

// functions
void* alloc(size_t size);
int free(void* ptr);
void pmm_init();

u64 get_hhdm();

#endif