#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <include/types.h>

// kernel printf: load args, then call virtual kernel printf (vkprintf)
void kprintf(const char* fmt, ...);

// virtual kernel printf: format and print
void vkprintf(const char* fmt, va_list args);
#endif
