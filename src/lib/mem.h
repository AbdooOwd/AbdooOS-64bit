#ifndef MEM_H
#define MEM_H

#pragma once

#include <include/types.h>
#include <mm/pmm.h>

void memcpy(u64* source, u64* dest, int no_bytes);
void memset(u64* dest, u8 val, u32 len);
int  memcmp(const void* ptr1, const void* ptr2, u16 num);
void *memmove(void *dest, const void *src, size_t n);

bool alloc_test();
void* simple_malloc(size_t size);

/* At this stage there is no 'free' implemented. */
u32 kmalloc(size_t size, int align, u32 *phys_addr);

#endif
