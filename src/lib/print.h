#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include "../include/types.h"

// kernel printf: load args, then call virtual kernel printf (vkprintf)
void kprintf(const char* fmt, ...);

// virtual kernel printf: format and print
void vkprintf(const char* fmt, va_list args);

u64 print_syscall(u64 frame, u64 str, u64 unused,
                       u64 unused2, u64 unused3);
#endif
