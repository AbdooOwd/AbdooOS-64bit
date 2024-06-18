#include "bm_alloc.h"

u8 memory[MEMORY_SIZE]; // The memory array
u8 bitmap[NUM_BLOCKS / 8]; // The bitmap array
u64 used_bits = 0;
u64 last_size;

void* bitmap_malloc(size_t size) {
    last_size = size;
    if (size > BLOCK_SIZE) {
        // Requested size is too large for a single block
        return NULL;
    }

    // Find a free block
    for (size_t i = 0; i < NUM_BLOCKS; i++) {
        if (!BIT_CHECK(bitmap, i)) {
            // Mark the block as used
            BIT_SET(bitmap, i);
            used_bits += size;
            // Return the pointer to the memory block
            return &memory[i * BLOCK_SIZE];
        }
    }

    // No free block found
    return NULL;
}

void bitmap_free(void* ptr) {
    // Calculate the block index from the pointer
    size_t block_index = ((u8*)ptr - memory) / BLOCK_SIZE;

    if (block_index < NUM_BLOCKS) {
        // Mark the block as free
        BIT_CLEAR(bitmap, block_index);
        used_bits -= last_size;
    }
}