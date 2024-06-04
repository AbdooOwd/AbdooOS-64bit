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

typedef struct {
    u64 r15;
    u64 r14;
    u64 r13;
    u64 r12;
    u64 r11;
    u64 r10;
    u64 r9;
    u64 r8;
    u64 rbp;
    u64 rdi;
    u64 rsi;
    u64 rdx;
    u64 rcx;
    u64 rbx;
    u64 rax;
    u64 interrupt;  // this is pushed in the macro (Hopefully it's 8 bytes)
    u64 error_code; // This is pushed by the cpu if the interrupt is an
                    // error interrupt. If not, then we push a dummy value
                    // of 0(in the macro)
    u64 iret_rip;   // iret prefix means that the cpu pushed this
                    // automatically and we didn't
    u64 iret_cs;
    u64 iret_flags;
    u64 iret_rsp;
    u64 iret_ss;
} __attribute__((packed)) execution_context;


#endif