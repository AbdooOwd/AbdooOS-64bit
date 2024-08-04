#include "pit.h"
#include "irq.h"
#include <kernel/io.h>
#include <drivers/screen.h> // for cursor manipulation

u64 ticks = 0;
u64 seconds = 0;
u32 freq = 1;

InterruptRegisters* PIT_handler(InterruptRegisters* regs) {
	ticks++;
	if (ticks % 87 == 0) seconds++;

    if (ticks % 87 == 0 && seconds % cursor_delay == 0)
        blink_cursor();

	return regs;
}

void PIT_init(){
    ticks = 0;
    IRQ_installHandler(0, PIT_handler);
    // 1.193181 MHz
    u32 divisor = 1193180 / freq;

    outb(0x43, 0x36);
    outb(0x40, (u8) (divisor & 0xFF));
    outb(0x40, (u8) (divisor >> 8 & 0xFF)); 
}