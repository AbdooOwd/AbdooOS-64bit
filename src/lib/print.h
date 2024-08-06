#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <include/types.h>

#define p kprintf

// kernel printf: load args, then call virtual kernel printf (vkprintf)
void kprintf(const char* fmt, ...);
void kprintf_color(const char* fmt, u32 color, ...);
void kprintf_at(const char* fmt, int x, int y, ...);
void kprint_color_at(const char* fmt, int x, int y, u32 color, ...);

// virtual kernel printf: format and print
void vkprintf(const char* fmt, u32 color, int x, int y, va_list args);

void log(const char* fmt, ...);
void vlog(const char* fmt, va_list args);
void log_print(const char* str);
void logc(char c);
#endif
