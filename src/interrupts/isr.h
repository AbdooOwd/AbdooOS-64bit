#ifndef ISR_H
#define ISR_H
#pragma once

#include <include/types.h>

typedef struct {
    u64 rbp;
    u64 r15;
    u64 r14;
    u64 r13;
    u64 r12;
    u64 r11;
    u64 r10;
    u64 r9;
    u64 r8;
    u64 rsi;
    u64 rdi;
    u64 rdx;
    u64 rcx;
    u64 rbx;
    u64 rax;
    u64 int_number; // this is pushed in the macro (Hopefully it's 8 bytes)
    u64 error_code; // This is pushed by the cpu if the interrupt is an
                         // error interrupt. If not, then we push a dummy value
                         // of 0(in the macro)
    u64 iret_rip;   // iret prefix means that the cpu pushed this
                         // automatically and we didn't
    u64 iret_cs;
    u64 iret_flags;
    u64 iret_rsp;
    u64 iret_ss;
} __attribute__((packed)) registers;


typedef void (*ISR_handler_t)(registers* regs);

void ISR_init();
void ISR_handler(registers* regs);
void ISR_registerHandler(int interrupt, ISR_handler_t handler);

#endif