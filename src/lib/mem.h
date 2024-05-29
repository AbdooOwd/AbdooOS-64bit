#ifndef MEM_H
#define MEM_H

#pragma once

#include "../include/types.h"

void memcpy(u8* source, u8* dest, int no_bytes);
void memset(u64* dest, u8 val, u32 len);
int memcmp(const void* ptr1, const void* ptr2, u16 num);

/* At this stage there is no 'free' implemented. */
u32 kmalloc(size_t size, int align, u32 *phys_addr);

#endif
