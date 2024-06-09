/*
	Implementation of WaterMark Allocation
*/

#include "wmalloc.h"
#include <limine.h>
#include <lib/print.h>

__attribute__((used, section(".requests")))
static struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};


MemoryData get_largest_memory_region() {

	u64 free_mem_base = 0;
	u64 free_mem_size = 0;

    if (memmap_request.response == NULL) {
        kprintf("Memory map response is NULL\n");
        return;
    }

    struct limine_memmap_response *memmap_response = memmap_request.response;
    for (u64 i = 0; i < memmap_response->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
            if (entry->length > free_mem_size) {
                free_mem_base = entry->base;
                free_mem_size = entry->length;
            }
        }
    }

    /*kprintf("Largest available memory region: base = 0x%lx, size = 0x%lx (%lu KB)\n",
           free_mem_base,
           free_mem_size,
           free_mem_size / 1024);*/

	MemoryData result = { free_mem_base, free_mem_size };
		
	return result;
}

void wmalloc_init(WatermarkAllocator* allocator, u64 base, size_t size) {
    allocator->base = base;
    allocator->watermark = allocator->base;
    allocator->size = size;
	allocator->free_list = NULL;
}

void* wmalloc(WatermarkAllocator* allocator, size_t size) {
    // Align size to the nearest multiple of 64 for proper alignment
    size = (size + 63) & ~63;

    // Search the free list for a suitable free zone
    FreeZone* prev = NULL;
    FreeZone* current = allocator->free_list;
    while (current != NULL) {
        if (current->size >= size) {
            // Suitable free zone found, remove it from the free list
            if (prev) {
                prev->next = current->next;
            } else {
                allocator->free_list = current->next;
            }
            return (void*)current;
        }
        prev = current;
        current = current->next;
    }

    // If no suitable free zone found, use the watermark
    if (allocator->watermark + size > allocator->base + allocator->size) {
        // Out of memory
        return NULL;
    }

    void* mem = (void*)allocator->watermark;
    allocator->watermark += size;
    return mem;
}

void gpt_wfree(WatermarkAllocator* allocator, void* ptr, size_t size) {
    // Align size to the nearest multiple of 64 for proper alignment
    size = (size + 63) & ~63;  // Ensure proper alignment of size

    // Create a new free zone
    FreeZone* new_free_zone = (FreeZone*)ptr;  // Cast ptr to FreeZone pointer
    new_free_zone->size = size;  // Set the size of the new free zone
    new_free_zone->next = NULL;   // Initialize the next pointer of the new free zone to NULL

    // Insert the new free zone into the free list
    FreeZone* prev = NULL;  // Initialize a pointer to keep track of the previous free zone
    FreeZone* current = allocator->free_list;  // Start traversal from the head of the free list
    while (current != NULL && (uint64_t)current < (uint64_t)new_free_zone) {
        // Traverse the free list until we find the appropriate position to insert the new free zone
        prev = current;  // Update prev to the current free zone
        current = current->next;  // Move to the next free zone
    }

    // Insert the new free zone into the free list
    new_free_zone->next = current;  // Set the next pointer of the new free zone to the current free zone
    if (prev) {
        prev->next = new_free_zone;  // If prev is not NULL, set its next pointer to the new free zone
    } else {
        allocator->free_list = new_free_zone;  // If prev is NULL, update the head of the free list to the new free zone
    }

    // Merge contiguous free zones
    if (new_free_zone->next != NULL && (uint64_t)new_free_zone + new_free_zone->size == (uint64_t)new_free_zone->next) {
        // Check if the new free zone is contiguous with its next free zone
        new_free_zone->size += new_free_zone->next->size;  // Merge the new free zone with its next free zone
        new_free_zone->next = new_free_zone->next->next;    // Skip the next free zone in the list
    }
    if (prev != NULL && (uint64_t)prev + prev->size == (uint64_t)new_free_zone) {
        // Check if the previous free zone is contiguous with the new free zone
        prev->size += new_free_zone->size;  // Merge the previous free zone with the new free zone
        prev->next = new_free_zone->next;   // Skip the new free zone in the list
    }
}


void wfree(WatermarkAllocator* allocator, void* ptr, size_t size) {
	size = (size + 63) & ~63;	// nearest multiple of 64

	// allocated memory is now free memory, meaning:
	//	ptr (allocated memory) -> ptr (now free memory)
	FreeZone* new_freeZone = (FreeZone*) ptr;
	new_freeZone->size = size;
	new_freeZone->next = NULL;	// nothing next yet (we're new)

	FreeZone* prev = NULL;
	FreeZone* cur  = allocator->free_list;

	// keep iterating until current free zone is null
	// or we reached the end of the free zone list
	while (cur != NULL & (u64) cur < (u64) new_freeZone) {
		// updating free zone
		prev = cur;
		cur = cur->next;
	}

	// we get out of the loop with the last Free Zone

	new_freeZone->next = cur;

	// update previous free zone's next free zone
	if (prev) 	prev->next = new_freeZone;
	// put new free zone at the top of the list (???)
	else		allocator->free_list = new_freeZone;

	// if new zone not null, and the end of new free zone is 
	// the beginning of the next one then...
	if (new_freeZone != NULL && (u64) new_freeZone + new_freeZone->size == new_freeZone->next) {
		// merge the sizes
		new_freeZone->size += new_freeZone->next->size;
		/* (long explanation)
			We got 3 free zones: A, B and C.
			size of A + size of B = size of AB.
			now we got 2 free zones: AB and C.
			Because of that, next free zone now is C, not B.
		*/
		new_freeZone->next = new_freeZone->next->next;
	}

	// if previous zone's end is new zone's base...
	if (prev != NULL && (u64) prev + prev->size == (u64) new_freeZone) {
		// now the previous size is the current size
		prev->size += new_freeZone->size;
		prev->next = new_freeZone->next;

		// Turning new free zone into previous zone
	}
}

void wmalloc_reset(WatermarkAllocator* allocator) {
    allocator->watermark = allocator->base;
}