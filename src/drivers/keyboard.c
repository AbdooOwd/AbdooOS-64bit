#include "keyboard.h"
#include <interrupts/irq.h>
#include <include/functions.h>
#include <lib/print.h>
#include <kernel/io.h>
#include <cpu/pic.h>


#define KBD_DATA_PORT 0x60


InterruptRegisters* keyboard_handler(InterruptRegisters* regs) {
	inb(KBD_DATA_PORT);

	return regs;
}

char get_input_char() {
	u8 scancode = inb(0x60) & 0x7F;
}

void keyboard_init() {
	IRQ_installHandler(1, keyboard_handler);
}