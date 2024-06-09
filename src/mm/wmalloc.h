#ifndef WMALLOC_H
#define WMALLOC_H
#pragma once

#include <include/types.h>


typedef struct {
	u64 base;
	u64 size;
} MemoryData;

typedef struct FreeZone {
    struct FreeZone* next;
    struct FreeZone* prev;
    size_t size;
} FreeZone;

typedef struct {
    u64 base;
    u64 watermark;
    size_t size;
    FreeZone* free_list;
} WatermarkAllocator;


MemoryData get_largest_memory_region();
void wmalloc_init(WatermarkAllocator* allocator, u64 base, size_t size);
void* wmalloc(WatermarkAllocator* allocator, size_t size);
void wfree(WatermarkAllocator* allocator, void* ptr, size_t size);
void wmalloc_reset(WatermarkAllocator* allocator);

#endif