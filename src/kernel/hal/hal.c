#include "hal.h"
#include <kernel/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/pit.h>
#include <cpu/cpu.h>
#include <drivers/keyboard.h>

void HAL_init() {
    GDT_init();
    IDT_init();
    PIT_init();
    keyboard_init();
}