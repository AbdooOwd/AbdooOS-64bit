#ifndef INT_H
#define INT_H

#include <include/types.h>
#include <cpu/cpu.h>


#define IDT_ENTRY_COUNT 256

#define IDT_ATTR_INTERRUPT_GATE 0x0e00
#define IDT_ATTR_PRESENT 0x8000


typedef struct {
    u16 limit;
    u64 base;
} __attribute__((packed)) IDTR;

typedef struct {
    u16 base_low;
    u16 selector;
    u16 flags;		// ist + attributes
    u16 base_mid;
    u32 base_high;
    u32 reserved;
} __attribute((packed)) IDTEntry;


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
} __attribute__((packed)) InterruptRegisters;

void IDT_init(void);
void IDT_setGate(int i, u64 handler, u16 flags);
void IDT_initGates();
void IDT_load(void);

#endif