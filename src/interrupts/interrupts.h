#ifndef INT_H
#define INT_H

#include <include/types.h>
#include <cpu/cpu.h>

#define IDT_ATTR_INTERRUPT_GATE 0x0e00
#define IDT_ATTR_PRESENT 0x8000


struct __attribute__((__packed__)) idtr {
    u16 limit;
    u64 base;
};

struct __attribute__((__packed__)) idt_entry {
    u16 base_low;
    u16 selector;
    u16 flags;
    u16 base_mid;
    u32 base_high;
    u32 reserved;
};

void interrupts_init(void);

void interrupt(execution_context regs);

void idt_set_gate(int i, u64 handler, u16 cs, u16 attr);
void idt_load(void);

extern void interrupt_handler_0();
extern void interrupt_handler_1();
extern void interrupt_handler_2();
extern void interrupt_handler_3();
extern void interrupt_handler_4();
extern void interrupt_handler_5();
extern void interrupt_handler_6();
extern void interrupt_handler_7();
extern void interrupt_handler_8();
extern void interrupt_handler_10();
extern void interrupt_handler_11();
extern void interrupt_handler_12();
extern void interrupt_handler_13();
extern void interrupt_handler_14();
extern void interrupt_handler_16();
extern void interrupt_handler_17();
extern void interrupt_handler_18();
extern void interrupt_handler_32();
extern void interrupt_handler_33();
extern void interrupt_handler_34();
extern void interrupt_handler_35();
extern void interrupt_handler_36();
extern void interrupt_handler_37();
extern void interrupt_handler_38();
extern void interrupt_handler_39();
extern void interrupt_handler_40();
extern void interrupt_handler_41();
extern void interrupt_handler_42();
extern void interrupt_handler_43();
extern void interrupt_handler_44();
extern void interrupt_handler_45();
extern void interrupt_handler_46();
extern void interrupt_handler_47();

#endif