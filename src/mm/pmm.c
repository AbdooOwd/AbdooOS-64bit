#include "pmm.h"
#include <limine.h>
#include <lib/mem.h>
#include <lib/print.h>
#include <cpu/cpu.h>

/**
 * The idea:
 * - Have a bitmap which is an array of bytes. Each element in 
 * the array is another array of bytes for i entry.
 * Meaning, if I want to access the bitmap of entry i at byte x 
 * I do: bitmap[i][x]
 */

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
        full_bitmap_size += entry->length / PAGE_SIZE;  // we'll use this later
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

        // Each index is a byte, and each bit is a page. So each element is 8 pages.
        bitmap_sizes[i] = entry->length / PAGE_SIZE;
	}
}

void* pmm_alloc(size_t size) {
    return NULL;
}

error_code pmm_free(void* ptr) {
    return 0;
}

u64 get_hhdm() {
	return hhdm->offset;
}