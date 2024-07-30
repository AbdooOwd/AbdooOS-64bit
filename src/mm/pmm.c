#include "pmm.h"
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


volatile struct limine_memmap_response* memmap;
volatile struct limine_hhdm_response* hhdm_response;

size_t blocks_count = 0;   // blocks' count
size_t mem_size = 0;       // memory's size
u64* watermarks;
u64 watermarks_size;
bool watermark_set = false;
// TODO: Make watermarks only store usable entries' watermarks, not include useless entries


u8* bitmap = NULL;        // The bitmap array (size = blocks_count / 8)
u64 used_bits = 0;
u64 last_size;


void pmm_init() {
    // requests' responses
    memmap = memmap_request.response;
    hhdm_response = hhdm_request.response;

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
		
		watermarks_size = entry->length / BLOCK_SIZE;
		watermarks = (u64*)(entry->base + get_hhdm());

        entry->base += watermarks_size;
        entry->length -= watermarks_size;

        mem_size += entry->length;
		break;
    }

    blocks_count = mem_size / BLOCK_SIZE;

    kprintf("PMM Initialized!\n");
}

void* alloc(size_t size) {
    if (size == 0 || size > mem_size) {
        kprintf("ERROR: Invalid size %x\n", size);
        return NULL;
	}

    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;   // not a usable memory region. Skip to next entry
        
        if ((u64) size > (entry->length - (entry->base - watermarks[i])) )
            continue;   // this entry isn't big enough. Skip to next one
        
        void* ptr = (void*)(get_hhdm() + entry->base + watermarks[i]);
        watermarks[i] += size;
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
            watermarks[i] = (u64) ptr;
            return 0;
        }
    }

    return -1;
}

u64 get_hhdm() { return hhdm_response->offset; }