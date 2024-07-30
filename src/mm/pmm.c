#include "pmm.h"
#include <lib/mem.h>
#include <lib/print.h>
#include <lib/util/util.h>
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


volatile struct limine_memmap_response* memmap;
volatile struct limine_hhdm_response* hhdm_response;

u64     mem_size;
size_t  free_entries;
bool**  usemap;
u64     usemap_size = 0;
u64*    usemap_watermarks;


// TODO: OPTIMIZE!!! TOO MUCH LOOPS!!1!

void pmm_init() {
    // requests' responses
    memmap = memmap_request.response;
    hhdm_response = hhdm_request.response;

    mem_size = 0;

    if (memmap == NULL) {
        panic("ERROR: Memory map response is null\n");
        return;
    }

    kprintf("-> Total Entries: %i\n", memmap->entry_count);
    
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (entry->type != LIMINE_MEMMAP_USABLE)
			continue;
		
        kprintf("Entry %i at %x with size %x\n", i, entry, entry->length);
        mem_size += entry->length;
        free_entries++;
    }

    // set usemap's size
    usemap_size = CEIL_DIV(mem_size, BLOCK_SIZE);
    
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (entry->length < usemap_size)
            continue;
        
        usemap = (bool**) (entry->base + get_hhdm());
        usemap_watermarks = (u64*) ((u64) usemap + usemap_size + get_hhdm());
        entry->base += usemap_size + (free_entries * sizeof(u64));
        entry->length -= usemap_size + (free_entries * sizeof(u64));

        memset(usemap_watermarks, 0, sizeof(u64) * free_entries);

        for (size_t j = 0; j < usemap_size; j++)
            usemap[i][j] = false;   // assuming every blocks is free and unused
    }

    kprintf("PMM Initialized!\n");
}

void* alloc(size_t size) {
    if (size == 0 || size > mem_size) {
        kprintf("ERROR: Invalid size %x\n", size);
        return NULL;
	}

    size_t first_free_block;
    size_t needed_blocks = CEIL_DIV(size, BLOCK_SIZE);

    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;   // not a usable memory region. Skip to next entry
        
        if (needed_blocks * BLOCK_SIZE > entry->length)
            continue;   // not enough blocks

        for (size_t j = 0; j < entry->length; j++) {
            if (j % BLOCK_SIZE == 0 && usemap[i][j] == false)
                first_free_block = j;    // first free page found in entry
        }

        // check if there are enough contiguous blocks
        size_t contiguous_blocks = 0;
        for (size_t t = 0; t < needed_blocks; t++) {
            if (!usemap[i][t + usemap_watermarks[i]])
                kprintf("[PMM] Not enough blocks in entry %i\n", i);
                break;
            contiguous_blocks++;
        }

        usemap_watermarks[i] += contiguous_blocks;
        
        void* ptr = (void*)((entry->base + (usemap_watermarks[i] * BLOCK_SIZE)) + get_hhdm());
        log("[PMM] Returning Address %x (next entry at %x)\n", (u64) ptr, memmap->entries[i+1]->base + get_hhdm());
        return ptr;
    }

    kprintf("ERROR: Not enough free blocks for size %zu\n", size);
    return NULL;
}

int free(void* ptr) {
    if (
        ptr == NULL || 
        (u64) ptr < memmap->entries[0]->base + get_hhdm() || 
        (u64) ptr >= get_hhdm() + (memmap->entries[memmap->entry_count - 1]->base + 
        memmap->entries[memmap->entry_count - 1]->length)
    ) {
        kprintf("ERROR: Invalid pointer %p\n", ptr);
        return -1;
    }

    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];

        if (
            (u64) ptr >= entry->base + get_hhdm() && 
            (u64) ptr < memmap->entries[i + 1]->base + get_hhdm()
        ) {
            //watermarks[i] = (u64) ptr;
            return 0;
        }
    }

    return -1;
}

u64 get_hhdm() { return hhdm_response->offset; }