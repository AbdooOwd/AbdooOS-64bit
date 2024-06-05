#include "idt.h"
#include "isr.h"
#include "irq.h"
#include <cpu/cpu.h>
#include <cpu/pic.h>
#include <lib/print.h>

#define IDT_DEFAULT_GATE(i) IDT_setGate(i, (u64) isr ## i, IDT_ATTR_PRESENT | IDT_ATTR_INTERRUPT_GATE);
#define IDT_REQUEST_GATE(i) IDT_setGate(i + 32, (u64) irq ## i, IDT_ATTR_PRESENT | IDT_ATTR_INTERRUPT_GATE);

IDTR idtr;
IDTEntry idt[IDT_ENTRY_COUNT];


void IDT_load(void) {
    asm volatile("lidt %0" : : "m"(idtr) : "memory");
}

void IDT_initGates() {
	IDT_DEFAULT_GATE(0)
	IDT_DEFAULT_GATE(1)
	IDT_DEFAULT_GATE(2)
	IDT_DEFAULT_GATE(3)
	IDT_DEFAULT_GATE(4)
	IDT_DEFAULT_GATE(5)
	IDT_DEFAULT_GATE(6)
	IDT_DEFAULT_GATE(7)
	IDT_DEFAULT_GATE(8)
	IDT_DEFAULT_GATE(9)
	IDT_DEFAULT_GATE(10)
	IDT_DEFAULT_GATE(11)
	IDT_DEFAULT_GATE(12)
	IDT_DEFAULT_GATE(13)
	IDT_DEFAULT_GATE(14)
	IDT_DEFAULT_GATE(15)
	IDT_DEFAULT_GATE(16)
	IDT_DEFAULT_GATE(17)
	IDT_DEFAULT_GATE(18)
	IDT_DEFAULT_GATE(19)
	IDT_DEFAULT_GATE(20)
	IDT_DEFAULT_GATE(21)
	IDT_DEFAULT_GATE(22)
	IDT_DEFAULT_GATE(23)
	IDT_DEFAULT_GATE(24)
	IDT_DEFAULT_GATE(25)
	IDT_DEFAULT_GATE(26)
	IDT_DEFAULT_GATE(27)
	IDT_DEFAULT_GATE(28)
	IDT_DEFAULT_GATE(29)
	IDT_DEFAULT_GATE(30)
	IDT_DEFAULT_GATE(31)

	// syscalls
	IDT_DEFAULT_GATE(128)
	IDT_DEFAULT_GATE(177)

	// irq
	
	IDT_REQUEST_GATE(0)
	IDT_REQUEST_GATE(1)
	IDT_REQUEST_GATE(2)
	IDT_REQUEST_GATE(3)
	IDT_REQUEST_GATE(4)
	IDT_REQUEST_GATE(5)
	IDT_REQUEST_GATE(6)
	IDT_REQUEST_GATE(7)
	IDT_REQUEST_GATE(8)
	IDT_REQUEST_GATE(9)
	IDT_REQUEST_GATE(10)
	IDT_REQUEST_GATE(11)
	IDT_REQUEST_GATE(12)
	IDT_REQUEST_GATE(13)
	IDT_REQUEST_GATE(14)
	IDT_REQUEST_GATE(15)
}

void IDT_init(void) {
	IDT_initGates();

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