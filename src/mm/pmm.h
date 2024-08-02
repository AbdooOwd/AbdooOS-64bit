#ifndef PMM_H
#define PMM_H

#pragma once

#include <include/types.h>

/* functions */
void pmm_init();
void* pmm_alloc(size_t size);
error_code pmm_free(void* ptr);

#endif