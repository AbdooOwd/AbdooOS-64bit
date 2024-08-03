#include "pmm.h"
#include <limine.h>
#include <lib/mem.h>
#include <lib/print.h>
#include <lib/util/binary.h>
#include <lib/util/util.h>
#include <cpu/cpu.h>

/**
 * The idea:
 * - Have a bitmap which is an array of bytes. Each element in 
 * the array is another array of bytes for i entry.
 * Meaning, if I want to access the bitmap of entry i at byte x 
 * I do: bitmap[i][x]
 */

// TODO: I use too much loops...

/* Limine Features */
static volatile struct limine_memmap_request memmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_response* memmap;
static volatile struct limine_hhdm_response* hhdm;

/**
 * @brief each element for each entry, then the arrays of each entry are for the pages
 */
u64** bitmap;       
u64* bitmap_sizes;	// measured in bytes
u64 free_entry_count;
u64 full_bitmap_size;   // measured in bytes

// TODO: Optimize
void pmm_init() {
    if (memmap_req.response == NULL)
        panic("[PMM] Limine MemMap Request's Response is NULL");
    
    if (hhdm_req.response == NULL)
        panic("[PMM] Limine HHDM Request's Response is NULL");
    
    memmap = memmap_req.response;
    hhdm = hhdm_req.response;

    /** Whats being done here:
     * - Get number of free entries
     * - get size required for the bitmap
     */
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;
        
        kprintf("[PMM] Entry %i > Base: %x - Size: %x\n", i, entry->base, entry->length);
        kprintf("[PMM] This Entry's Bitmap Size: %i\n", entry->length / PAGE_SIZE);

        free_entry_count++;
        // TODO: Something feels off here... We're counting bits, not bytes (number of bits = number of pages)
        full_bitmap_size += (entry->length / PAGE_SIZE) / 8;  // we get it in bytes
    }

    /** Whats done
     * - find a place to store the bitmap
     */
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];
        u64 bitmap_size = sizeof(u64) * free_entry_count;
        
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;
		
		// not enough for bitmap
		if (entry->length < full_bitmap_size + bitmap_size)
			continue;
		
        // place the bitmap in memory
		bitmap = (u64**) (entry->base + get_hhdm());
		memset(bitmap, 0, full_bitmap_size);
		entry->base += full_bitmap_size;
		entry->length -= full_bitmap_size;
        kprintf("[PMM] Placed Bitmap in memory at %x\n", (u64) bitmap);

        // place bitmap sizes in memory
        bitmap_sizes = (u64*) (entry->base + get_hhdm());
		memset(bitmap_sizes, 0, bitmap_size);
        entry->base += bitmap_size;
        entry->length -= bitmap_size;
        kprintf("[PMM] Placed Bitmap Sizes in Memory at %x\n", (u64) bitmap_sizes);

        kprintf("[PMM] New entry base: %x - new entry size: %x\n", entry->base, entry->length);
		break;
    }

    /** Whats done
     * - Set the size of each entry's bitmap
     */
	for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

		if (entry->type != LIMINE_MEMMAP_USABLE)
			continue;

        // Each index is a byte, and each bit is a page. So each element is (size of datatype * 8) pages.
        bitmap_sizes[i] = entry->length / PAGE_SIZE;
	}
}

void* pmm_alloc(size_t size) {
    /**
     * # The slow way:
     * - We iterate over each entry, then iterate over its bitmap.
     * If the bitmap says that there isn't enough free space for our allocation
     * (if `FLOOR_DIV(size, PAGE_SIZE) > free space`), we go to the next entry.
     * The problem with this approach is that we have to iterate over each entry,
     * then over each bitmap element, then over each bit in the bitmap element... etc
     */

    size_t pages_needed = CEIL_DIV(size, PAGE_SIZE);
    bool found_space = false;

    size_t continued_free_bits = 0;
    // TODO: replace bit_start_offset with those dividing technics like used in the framebuffer (more efficient)
    size_t bit_start_offset[2] = {0, 0};    // 1: u64 index - 2: bit offset
    bool found_start = false;

    log("[PMM] Need %i free pages\n", pages_needed);

    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (found_space) break;

        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;
        
        /**
         * The idea:
         * - We scan each bit in the bitmap, if we meet a zero,
         * we go through another loop. The loop's condition will be that 
         * each bit we iterate to (??? bad english) should be clear.
         * If we meet a 1 in that loop, we break from it and search for another "free hole".
         * The goal of that is finding a "free hole" large enough for our allocation.
         */

        /**
         * 1. go through each u64 (first for loop 'u64_i')
         * 2. go through each bit in the u64 (2nd loop 'b')
         * 3. repeat (go back to  step 1)
         */

        for (size_t u64_i = 0; u64_i < bitmap_sizes[i]; u64_i++) {
            if (continued_free_bits >= pages_needed) {
                log("[PMM] Enough space has been found for allocation of %i pages\n", continued_free_bits);
                found_space = true;
                break;
            }

            for (size_t b = 0; b < sizeof(u64) * 8; b++) {
                if (!bit_get(bitmap[i][u64_i], b)) {   //! problem here
                    if (!found_start) {
                        bit_start_offset[0] = u64_i;
                        bit_start_offset[1] = b;
                        found_start = true;
                    }
                    continued_free_bits++;
                    continue;
                } else {
                    continued_free_bits = 0;
                    bit_start_offset[0] = 0;
                    bit_start_offset[1] = 1;
                    found_start = false;
                }
            }
        }
    }

    log("[PMM] Found %i free pages starting at bit %i of u64 %i\n", 
        continued_free_bits, bit_start_offset[1], bit_start_offset[0]);

    return NULL;
}

error_code pmm_free(void* ptr) {
    return 0;
}

u64 get_hhdm() {
	return hhdm->offset;
}