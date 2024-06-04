#ifndef INT_H
#define INT_H

#include <include/types.h>
#include <cpu/cpu.h>

#define IDT_ATTR_INTERRUPT_GATE 0x0e00
#define IDT_ATTR_PRESENT 0x8000


typedef struct {
    u16 limit;
    u64 base;
} __attribute__((__packed__)) IDTR;

typedef struct {
    u16 base_low;
    u16 selector;
    u16 flags;
    u16 base_mid;
    u32 base_high;
    u32 reserved;
} __attribute((packed)) IDTEntry;

void interrupts_init(void);

void interrupt(execution_context regs);

void IDT_setGate(int i, u64 handler, u16 flags);
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
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr16();
extern void isr17();
extern void isr18();
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