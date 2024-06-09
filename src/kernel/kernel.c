#include "kernel.h"
#include <cpu/cpu.h>
#include <drivers/screen.h>
#include <include/globals.h>
#include <lib/print.h>
#include <lib/mem.h>
#include "hal/hal.h"

void kernel_init() {
    kprintf("Initialization (kernel_init) at %X\n", kernel_init);
    kprintf("Start (kernel_main) at %X\n\n", kernel_main);

    if (alloc_test()) {
        kprintf("Allocation test complete! (Allocated and Free'd 8 bytes)\n\n");
    } else {
        kprintf("Memory Allocation Test Failed (8 bytes allocation failed)\n");
        panic("Halting CPU (Allocation Test Fail)\n");
    }

    HAL_init();

    print("Booted AbdooOS Successfully!\n");
    clear_screen();

    kernel_main();
}

void kernel_main() {
    kprintf(" - - - AbdooOS %s - - -\n", ABDOOOS_VERSION);

    print("\n$ ");

    for (;;) ;
}