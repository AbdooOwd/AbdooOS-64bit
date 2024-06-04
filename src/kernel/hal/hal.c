#include "hal.h"
#include <kernel/gdt.h>
#include <interrupts/interrupts.h>
#include <cpu/cpu.h>

void HAL_init() {
    GDT_init();
    interrupts_init();
}