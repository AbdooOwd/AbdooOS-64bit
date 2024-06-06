#include "hal.h"
#include <kernel/gdt.h>
#include <interrupts/idt.h>
#include <cpu/cpu.h>
#include <drivers/keyboard.h>

void HAL_init() {
    GDT_init();
    IDT_init();
    keyboard_init();
}