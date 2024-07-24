#include "bm_alloc.h"
#include <lib/mem.h>
#include <lib/print.h>
#include <cpu/cpu.h>
#include <limine.h>

// I request the memory map bruh

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};


volatile struct limine_memmap_response* memmap_response;
volatile struct limine_hhdm_response* hhdm_response;

size_t blocks_count = 0;   // blocks' count
size_t mem_size = 0;       // memory's size
u64 watermarks[32];
mem_t memory;               // useless for now, will replace blocks and fix alignement
// TODO: Make watermarks only store usable entries' watermarks, not include useless entries


u8* bitmap = NULL;        // The bitmap array (size = blocks_count / 8)
u64 used_bits = 0;
u64 last_size;


void* allocation(size_t size) {
    // p("Size %x / %i provided\n", size, size);
    // p("Total free size (without current allocation size): %x / %i\n", mem_size - size, mem_size - size);
    log("[DEV] Allocation for size %x / %i\n", size, size);

    if (size == 0 || size > mem_size) {
        kprintf("ERROR: Invalid size %x\n", size);
        return NULL;
    }

    for (size_t i = 0; i < memmap_response->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap_response->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;   // not a usable memory region. Skip to next entry
        
        if ((u64) size > (entry->length - (entry->base - watermarks[i])) )
            continue;   // this entry isn't big enough. Skip to next one

        void* ptr = (void*)(get_hhdm() + entry->base + watermarks[i]);
        // p("Found a pointer %x in entry %i at %x with size %x, end of entry at %x\n", ptr, i, entry->base, entry->length, entry->base + entry->length);
        // p("OG WM: %x - New WM: %x\n", watermarks[i], watermarks[i] + size);
        watermarks[i] += size;
        log("[DEV] Watermark N%i: %x\n", i, watermarks[i]);

        return ptr;
    }

    kprintf("ERROR: Not enough free blocks for size %zu\n", size);
    return NULL;
}

int freebird(void* ptr) {
    log("[DEV] Freeing for pointer to %x\n", &ptr);
    if (
        ptr == NULL || 
        (u64) ptr < memmap_response->entries[0]->base + get_hhdm() || 
        (u64) ptr >= get_hhdm() + (memmap_response->entries[memmap_response->entry_count - 1]->base + 
        memmap_response->entries[memmap_response->entry_count - 1]->length)
    ) {
        kprintf("ERROR: Invalid pointer %p\n", ptr);
        return -1;
    }

    for (size_t i = 0; i < memmap_response->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap_response->entries[i];

        if (
            (u64) ptr >= entry->base + get_hhdm() && 
            (u64) ptr < memmap_response->entries[i + 1]->base + get_hhdm()
        ) {
            watermarks[i] = (u64) ptr - get_hhdm();
            log("[DEV] Watermark N%i after free: %x\n", i, watermarks[i]);
            return 0;
        }
    }

    return -1;
}
/*
void* bitmap_malloc(size_t size) {
    if (size == 0 || size > mem_size) {
        kprintf("Error: Invalid size %zu\n", size);
        return NULL; // Invalid size
    }

    size_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE; // num of blocks needed
    size_t consecutive_free_blocks = 0;
    size_t start_block = 0;

    for (size_t i = 0; i < blocks_count; i++) {
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
    kprintf("Error: No sufficient contiguous free blocks for size %i\n", size);
    return NULL;
}

void bitmap_free(void* ptr) {
    if (ptr == NULL || ptr < (void*)memory || ptr >= (void*)(memory + mem_size)) {
        kprintf("Error: Invalid pointer %p\n", ptr);
        return; // Invalid pointer
    }

    size_t block_index = ((u8*)ptr - memory) / BLOCK_SIZE;
    size_t blocks_freed = 0;

    while (block_index < blocks_count && BIT_CHECK(bitmap, block_index)) {
        BIT_CLEAR(bitmap, block_index);
        blocks_freed++;
        block_index++;
    }

    used_bits -= blocks_freed * BLOCK_SIZE;
}
*/

void bitmap_init() {
    // requests' responses
    memmap_response = memmap_request.response;
    hhdm_response = hhdm_request.response;
    log("[DEV] HHDM: %x\n", get_hhdm());

    if (memmap_response == NULL) {
        panic("ERROR: Memory map response is null\n");
        return;
    }

    kprintf("-> Total Entries: %i\n", memmap_response->entry_count);
    
    for (size_t i = 0; i < memmap_response->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap_response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            kprintf("Entry %i at %x with size %x\n", i, entry, entry->length);
            mem_size += entry->length;
        }
    }

    blocks_count = mem_size / BLOCK_SIZE;

    for (size_t i = 0; i < memmap_response->entry_count; i++)
        // every usable entry should be empty, so free space starts at 0
        watermarks[i] = 0x0;

    kprintf("PMM Initialized!\n");
}

u64 get_hhdm() { return hhdm_response->offset; }