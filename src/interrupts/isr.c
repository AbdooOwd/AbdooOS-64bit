#include "isr.h"
#include "idt.h"
#include "../lib/print.h"

void ISR_initGates();

void ISR_init() {
    ISR_initGates();

    for (size_t i = 0; i < IDT_ENTRY_COUNT; i++)
        IDT_enableGate(i);
}

void ISR_handler(registers* regs) {
    kprintf("Interrupt %d\n", regs->int_number);
}