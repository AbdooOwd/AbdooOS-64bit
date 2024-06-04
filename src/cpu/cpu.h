#ifndef CPU_H
#define CPU_H

#pragma once

#include <include/types.h>


extern void enableInterrupts();
extern void disableInterrupts();

void panic(char* fmt, ...);
extern void crash_me(void);

static inline void halt(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}


#endif