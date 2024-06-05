#include "isr.h"
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
    "Timer",
};

InterruptRegisters ISR_handler(InterruptRegisters regs) {
	kprintf("\n");
    if (regs.interrupt < 32) {
        kprintf("Unhandled Interrupt %x - error code: %x - rip: %x\n", regs.interrupt, regs.error_code, regs.iret_rip);
		kprintf("Exception: %s\n", exception_messages[regs.interrupt]);
        panic("\nCPU Panic");
    } else {
		kprintf("interrupt %d\n", regs.interrupt);
    }

	return regs;
}