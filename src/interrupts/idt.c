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
    __asm__ volatile("lidt %0" : : "m"(idtr) : "memory");
}

void interrupts_init() {
    kprintf("[IDT] Stubbing every IDT Gate...\n");
	for(size_t vector = 0; vector < sizeof(idt) / sizeof(IDTEntry); vector++) {
        // log("[IDT] Stubbing IDT Gate N%i\n", vector);
        IDT_setGate(vector, isr_stub_table[vector], IDT_ATTR_PRESENT | IDT_ATTR_INTERRUPT_GATE);
    }
}

void IDT_init(void) {
    kprintf("[IDT] Initializing IDT...\n");
	interrupts_init();

    idtr.base = (u64) idt;
    idtr.limit = IDT_ENTRY_COUNT * sizeof(IDTEntry) - 1;

    PIC_init();
    IDT_load();
	enableInterrupts(); // 'sti' instruction
    kprintf("[IDT] IDT Initialized!\n");
}

void IDT_setGate(int i, u64 handler, u16 flags) {
    idt[i].base_low = (u16) (handler & 0xffff);
    idt[i].base_mid = (u16) (handler >> 16) & 0xffff;
    idt[i].base_high = (u32) (handler >> 32) & 0xffffffff;
    idt[i].flags = flags;
    idt[i].reserved = 0;
    idt[i].selector = 0x08;
    // log("[IDT] Set IDT Gate %i handler to %x with flags %i\n", i, handler, flags);
}