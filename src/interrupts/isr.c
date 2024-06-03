#include "isr.h"
#include "idt.h"
#include <lib/print.h>

char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

extern char isr0[];

execution_context* ISR_handler(execution_context* regs) {
	
	// that's a fault interrupt!!1!
	if (regs->interrupt < 32) {
		kprintf("Unhandled Interrupt %d: %s\n", regs->interrupt, exception_messages[regs->interrupt]);
		panic("\nHalting CPU\n");
	}
	
	return regs;
}

void ISR_initGates() {
	for (size_t i = 0; i < IDT_ENTRY_COUNT; i++)
		// we know that each handler is 16 bytes long
		IDT_setEntry(i, (u64)isr0 + (i * 16), 0);
}