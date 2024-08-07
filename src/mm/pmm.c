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
u32 free_entry_count;
u32 full_bitmap_size;   // measured in bytes

// TODO: Have a data structure to store the free entry index and its actual index in the memmap (+ some info)
// TODO: Optimize
// TODO: Add Error Handling (eg: no entry big enough)
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

        full_bitmap_size += CEIL_DIV(CEIL_DIV(entry->length, PAGE_SIZE), 8);  // we get it in bytes
        /** explanation:
         * Each page represents a bit. When we divide entry's length
         * by PAGE_SIZE, we get the number of pages that the entry
         * can have. So it also means the number of bits we need.
         * And then (number of bits) / 8 = (number of bytes)
         */

        free_entry_count++;
    }

    log ("[PMM] Full bitmap size: %i bytes\n", full_bitmap_size);

    /** Whats done
     * - find how much pages each bitmap (each entry) can have
     * - find a place to store the bitmap
     */
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];
        u64 bitmap_size_size = sizeof(u64) * free_entry_count; // lol
        
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;
		
		// not enough for bitmap
		if (entry->length < full_bitmap_size + bitmap_size_size)
			continue;
		
        // place the bitmap in memory
		bitmap = (u64**) (entry->base + get_hhdm());
		memset(bitmap, 0, full_bitmap_size);
		entry->base += full_bitmap_size;
		entry->length -= full_bitmap_size;
        kprintf("[PMM] Placed Bitmap in memory at %x\n", (u64) bitmap);

        // place bitmap sizes in memory
        bitmap_sizes = (u64*) (entry->base + get_hhdm());
		memset(bitmap_sizes, 0, bitmap_size_size);
        entry->base += bitmap_size_size;
        entry->length -= bitmap_size_size;
        kprintf("[PMM] Placed Bitmap Sizes in Memory at %x\n", (u64) bitmap_sizes);

        kprintf("[PMM] New entry base: %x - new entry size: %x\n", entry->base, entry->length);
		break;
    }

    /** Whats done
     * - Set the size of each entry's bitmap
     */
    size_t free_i = 0;
    /**
     * Iterating over entry count and not free entries count
     * cuz I need the entry's length (extra un-optimized)
     */
	for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

		if (entry->type != LIMINE_MEMMAP_USABLE)
			continue;

        // Each index is a byte, and each bit is a page. So each element is (size of datatype * 8) pages.
        bitmap_sizes[free_i] = CEIL_DIV(CEIL_DIV(entry->length, PAGE_SIZE), 8);
        free_i++;
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

    /**
     * Another Idea is doing as we did with the bitmap and its sizes.
     * You give it that base, increase that base, 
     * and decrease the remaining "free length"
     */

    size_t pages_needed = CEIL_DIV(size, PAGE_SIZE);
    bool found_space = false;

    size_t continued_free_bits = 0;
    // TODO: replace bit_start_offset with those dividing technics like used in the framebuffer (more efficient)
    // size_t bit_start_offset[3] = {0, 0,};    // 1: u64 index - 2: bit offset - 3: entry
    bitmap_offset_t bitmap_offsets = {
        .bit_offset = 0,
        .entry_index = 0,
        .u64_index = 0
    };  // bad practice (setting everything to 0), we don't know ANYTHING about our offsets!
    bool found_start = false;

    u64 cur_u64 = (u64) bitmap; // base of current u64

    log("[PMM] Need %i free pages\n", pages_needed);

    for (size_t i = 0; i < free_entry_count; i++) {
        if (found_space) break;
        
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

        for (size_t u64_i = 0; u64_i < bitmap_sizes[i] / sizeof(u64); u64_i++) {
            

            for (size_t j = 0; j != i; j++)
                cur_u64 += bitmap_sizes[j]; // find cur entry base
            
            for (size_t pp = 0; pp < u64_i; pp++)
                cur_u64 += sizeof(u64);
            
            log("[PMM] Base of current u64: %x\n", cur_u64);

            /**
             * DEVLOG
             * - I gotta use my bitmap_sizes to find where
             * the u64 is. As using it like a simple array
             * won't work. There isn't a fixed size.
             * Each entry's bitmap has a different size.
             */

            for (size_t b = 0; b < sizeof(u64) * 8; b++) {
                // log("[PMM] Bit: %i - U64: %i - Free Entry: %i\n", b, u64_i, i);

                if (continued_free_bits >= pages_needed) {
                    log("[PMM] Enough space has been found for allocation of %i pages\n", continued_free_bits);
                    found_space = true;
                    break;
                }
                
                if (!TEST_BIT(*((u64*) cur_u64), b)) {
                    if (!found_start) {
                        bitmap_offsets.u64_index = u64_i;
                        bitmap_offsets.bit_offset = b;
                        bitmap_offsets.entry_index = i;
                        found_start = true;
                    }
                    continued_free_bits++;
                    continue;
                } else {
                    continued_free_bits = 0;
                    
                    bitmap_offsets.bit_offset = 0;
                    bitmap_offsets.u64_index = 0;
                    found_start = false;
                }
            }
        }
    }

    log("[PMM] Found %i free pages starting at bit %i of u64 %i in entry %i\n", 
        continued_free_bits, bitmap_offsets.bit_offset, bitmap_offsets.u64_index, bitmap_offsets.entry_index);

    if (found_start && found_space) {
        void* ptr;
        u64 ptr_base = 0;
        // step 1: set all found bits as used now
        for (size_t b = 0; b < pages_needed; b++)
            SET_BIT(*((u64*) cur_u64), b + bitmap_offsets.bit_offset);
        
        // step 2: get the address
        /**
         * 1. get entry index
         * 2. calculate the page address/position using the bitmap
         * 3. boom, you got the base
         */
        size_t free_i = 0;
        for (size_t i = 0; i < memmap->entry_count; i++) {
            struct limine_memmap_entry* entry = memmap->entries[i];

            if (entry->type != LIMINE_MEMMAP_USABLE)
                continue;
            
            if (free_i == bitmap_offsets.entry_index) {
                ptr_base = entry->base;
                break;
            }

            free_i++;
        }
        ptr = (void*) (ptr_base + (u64) (((bitmap_offsets.u64_index * 8) + bitmap_offsets.bit_offset) * PAGE_SIZE) + get_hhdm());
        log("[PMM] Returning Pointer Address: %x\n", (u64) ptr);
        return ptr;
    } else return NULL;
}

error_code pmm_free(void* ptr) {
    return 0;
}

u64 get_hhdm() {
	return hhdm->offset;
}