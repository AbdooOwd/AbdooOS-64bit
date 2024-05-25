#ifndef MEM_H
#define MEM_H

#pragma once

#include "../include/types.h"

void memory_copy(u8* source, u8* dest, int no_bytes);
void memory_set(u8* dest, u8 val, u32 len);

/* At this stage there is no 'free' implemented. */
u32 kmalloc(size_t size, int align, u32 *phys_addr);

#endif
