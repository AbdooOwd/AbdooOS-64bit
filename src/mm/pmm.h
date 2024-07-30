#ifndef PMM_H
#define PMM_H
#pragma once

#include <include/types.h>

#define malloc 	alloc
#define mfree	free

#define BLOCK_SIZE 	1024 * 4         			// (4096 bytes) Size of each memory block (4KiB)

// functions
void* alloc(size_t size);
int free(void* ptr);
void pmm_init();

u64 get_hhdm();

#endif