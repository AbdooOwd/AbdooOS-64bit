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

void interrupts_init(void);
InterruptRegisters ISR_handler(InterruptRegisters regs);

void IDT_setGate(int i, u64 handler, u16 flags);
void IDT_initGates();
void IDT_load(void);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();

#endif