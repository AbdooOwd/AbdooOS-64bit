#include "hal.h"
#include <kernel/gdt.h>
#include <interrupts/idt.h>
#include <cpu/cpu.h>

void HAL_init() {
    GDT_init();
    IDT_init();
}