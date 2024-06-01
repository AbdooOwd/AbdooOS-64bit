#include "cpu.h"
#include <lib/print.h>
#include <lib/formatter.h>
#include <stdarg.h>

void panic(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[1024] = {0};

    int i = 0;


    while(*fmt && i < 1024) {
        if (*fmt == '%') {
            fmt++;
            i = format_handler(*fmt, buffer, i, args);
            fmt++;
            continue;
        }
        buffer[i] = *fmt;
        i++;
        fmt++;
    }

    va_end(args);
    kprintf(buffer);

    halt();
}