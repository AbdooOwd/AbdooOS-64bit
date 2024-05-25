#include "mem.h"

void memory_copy(u8* source, u8* dest, int no_bytes) {
    int i;
    for (i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8* dest, u8 val, u32 len) {
    u8* temp = (u8*)dest;
    for (; len != 0; len--) *temp++ = val;
}

u32 free_mem_addr = 0x1000;

u32 kmalloc(size_t size, int align, u32 *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    u32 ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}