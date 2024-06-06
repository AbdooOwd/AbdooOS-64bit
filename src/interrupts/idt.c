#include "idt.h"
#include "isr.h"
#include "irq.h"
#include <cpu/cpu.h>
#include <cpu/pic.h>
#include <lib/print.h>

IDTR idtr;
IDTEntry idt[IDT_ENTRY_COUNT];
extern u64 isr_stub_table[IDT_ENTRY_COUNT];

void IDT_load(void) {
    asm volatile("lidt %0" : : "m"(idtr) : "memory");
}

void interrupts_init() {
	for(size_t vector = 0; vector < sizeof(idt) / sizeof(IDTEntry); vector++)
        IDT_setGate(vector, isr_stub_table[vector], IDT_ATTR_PRESENT | IDT_ATTR_INTERRUPT_GATE);
}

void IDT_init(void) {
	interrupts_init();

    idtr.base = (u64) idt;
    idtr.limit = IDT_ENTRY_COUNT * sizeof(IDTEntry) - 1;

    PIC_init();
    IDT_load();
	enableInterrupts(); // 'sti' instruction
}

void IDT_setGate(int i, u64 handler, u16 flags) {
    idt[i].base_low = (u16) (handler & 0xffff);
    idt[i].base_mid = (u16) (handler >> 16) & 0xffff;
    idt[i].base_high = (u32) (handler >> 32) & 0xffffffff;
    idt[i].flags = flags;
    idt[i].reserved = 0;
    idt[i].selector = 0x08;
}