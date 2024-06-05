#include "print.h"
#include "formatter.h"
#include <drivers/screen.h>
#include "util/util.h"
#include <kernel/io.h>

void kprintf(const char* fmt, ...) {
    // loading optional parameters
    va_list args;
    va_start(args, fmt);

    vkprintf(fmt, args);

    va_end(args);
}

void vkprintf(const char* fmt, va_list args) {
    char buffer[MAX_STRING_FORMATTER_BUFFER_SIZE] = {0};

    int i = 0;
    while (*fmt && i < MAX_STRING_FORMATTER_BUFFER_SIZE) {
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
    print(buffer);
}

void log(const char* fmt, ...) {
    // loading optional parameters
    va_list args;
    va_start(args, fmt);

    vlog(fmt, args);

    va_end(args);
}

void vlog(const char* fmt, va_list args) {
    char buffer[MAX_STRING_FORMATTER_BUFFER_SIZE] = {0};

    int i = 0;
    while (*fmt && i < MAX_STRING_FORMATTER_BUFFER_SIZE) {
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
    log_print(buffer);
}

void log_print(const char* str) {
    while (*str) {
        outb(0xE9, *str);
        str++;
    }
}