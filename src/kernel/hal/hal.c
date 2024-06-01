#include "hal.h"
#include "../gdt.h"
#include "../../interrupts/idt.h"

void HAL_init() {
    GDT_init();
    IDT_init();
}