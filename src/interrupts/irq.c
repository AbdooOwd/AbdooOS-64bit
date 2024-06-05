#include "irq.h"
#include <cpu/pic.h>
#include <kernel/io.h>

void* IRQ_handlers[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void IRQ_installHandler(int irq, handler_t handler) {
	IRQ_handlers[irq] = handler;
}

void IRQ_uninstallHandler(int irq) {
	IRQ_handlers[irq] = 0;
}

InterruptRegisters* IRQ_handler(InterruptRegisters* regs) {
	// void (*handler)(InterruptRegisters* regs); 
	handler_t handler;
	handler = IRQ_handlers[regs->interrupt - 32];

	if (handler)
		handler(regs);

	if (regs->interrupt >= 40)
		outb(0xA0, 0x20);
	
	outb(0x20, 0x20);

	PIC_eoi(regs->interrupt - 32);

	return regs;
}