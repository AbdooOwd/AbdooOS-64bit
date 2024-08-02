#include "mem.h"
#include <cpu/cpu.h>
#include <mm/pmm.h>
#include <lib/print.h>

#define POOL_SIZE (1024 * 1024) // 1 MiB for the memory pool
static u8 mem_pool[POOL_SIZE];
static size_t mem_pool_offset = 0;

void* simple_malloc(size_t size) {
    if (mem_pool_offset + size > POOL_SIZE) {
        // Out of memory in the pool
        return NULL;
    }
    void* ptr = &mem_pool[mem_pool_offset];
    mem_pool_offset += size;
    return ptr;
}

bool alloc_test() {
    void* mem_test = malloc(8);
    if (mem_test == NULL) return false;
    log("Allocated 8 bytes\n");
    mfree(mem_test);
    log("Free'd 8 bytes\n");
    return true;
}

void memcpy(u64* source, u64* dest, int no_bytes) {
    size_t i;
    for (i = 0; (int) i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void* memset(void* dest, int ch, size_t count)
{
    uint8_t* dst = dest;
    for (size_t i = 0; i < count; i++)
        dst[i] = ch;

    return dest;
}

int memcmp(const void* ptr1, const void* ptr2, u16 num)
{
    const u8* u8Ptr1 = (const u8 *)ptr1;
    const u8* u8Ptr2 = (const u8 *)ptr2;

    for (u16 i = 0; i < num; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

void *memmove(void *dest, const void *src, size_t n) {
    // Typecast src and dest to (char *)
    char *d = (char *)dest;
    const char *s = (const char *)src;

    // If destination address is greater than source address, we copy backwards to handle overlap
    if (d > s) {
        for (size_t i = n; i != 0; i--) {
            d[i - 1] = s[i - 1];
        }
    } else {
        // Copy forwards otherwise
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    }

    return dest;
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