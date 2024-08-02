#ifndef PMM_H
#define PMM_H

#pragma once

#include <include/types.h>

#define PAGE_SIZE   4096    // 4KiB

#define malloc      pmm_alloc
#define mfree       pmm_free

/* functions */
void pmm_init();
void* pmm_alloc(size_t size);
error_code pmm_free(void* ptr);

u64 get_hhdm();

#endif