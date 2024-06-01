#ifndef CPU_H
#define CPU_H

#pragma once

#include <include/types.h>

void panic(char* fmt, ...);

static inline void halt(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

#endif