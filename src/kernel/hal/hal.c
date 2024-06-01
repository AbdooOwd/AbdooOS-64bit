#include "hal.h"
#include <kernel/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/isr.h> 

void HAL_init() {
    GDT_init();
    IDT_init();
    ISR_init();
}