#include "bm_alloc.h"
#include <lib/mem.h>

u8 memory[MEMORY_SIZE]; // The memory array
u8 bitmap[NUM_BLOCKS / 8]; // The bitmap array
u64 used_bits = 0;
u64 last_size;

void* bitmap_malloc(size_t size) {
    if (size == 0 || size > MEMORY_SIZE) {
        return NULL; // Invalid size
    }

    size_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Ceiling division
    size_t consecutive_free_blocks = 0;
    size_t start_block = 0;

    for (size_t i = 0; i < NUM_BLOCKS; i++) {
        if (!BIT_CHECK(bitmap, i)) {
            if (consecutive_free_blocks == 0) {
                start_block = i;
            }
            consecutive_free_blocks++;
            if (consecutive_free_blocks == blocks_needed) {
                // Found enough contiguous free blocks
                for (size_t j = start_block; j < start_block + blocks_needed; j++) {
                    BIT_SET(bitmap, j);
                }
                used_bits += blocks_needed * BLOCK_SIZE;
                return &memory[start_block * BLOCK_SIZE];
            }
        } else {
            consecutive_free_blocks = 0; // Reset if a used block is found
        }
    }

    // No sufficient contiguous free blocks found
    return NULL;
}

void bitmap_free(void* ptr) {
    if (ptr == NULL || ptr < (void*)memory || ptr >= (void*)(memory + MEMORY_SIZE)) {
        return; // Invalid pointer
    }

    size_t block_index = ((u8*)ptr - memory) / BLOCK_SIZE;
    size_t blocks_freed = 0;

    while (block_index < NUM_BLOCKS && BIT_CHECK(bitmap, block_index)) {
        BIT_CLEAR(bitmap, block_index);
        blocks_freed++;
        block_index++;
    }

    used_bits -= blocks_freed * BLOCK_SIZE;
}

void bitmap_init() {
    memset((u64*) bitmap, 0, sizeof(bitmap));
    used_bits = 0;
}