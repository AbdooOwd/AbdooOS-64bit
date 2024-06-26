#include "kernel.h"
#include <cpu/cpu.h>
#include <drivers/screen.h>
#include <include/globals.h>
#include <lib/print.h>
#include <lib/mem.h>
#include <mm/bm_alloc.h>
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

    bitmap_init();
    HAL_init();

    print("Booted AbdooOS Successfully!\n");

    kernel_main();
}

void kernel_main() {
    clear_screen();
    kprintf(" - - - AbdooOS %s - - -\n", ABDOOOS_VERSION);

    print("\n$ ");

    for (;;) ;
}