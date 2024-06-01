#include "print.h"
#include "formatter.h"
#include <drivers/screen.h>
#include "util/util.h"

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

u64 print_syscall(u64 frame, u64 str, u64 unused,
                       u64 unused2, u64 unused3) {
    UNUSED(frame, unused, unused2, unused3);
    kprintf((char*) str);
}
