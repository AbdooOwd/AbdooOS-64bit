#include "kernel.h"
#include <cpu/cpu.h>
#include <drivers/screen.h>
#include <include/globals.h>
#include <lib/print.h>
#include "hal/hal.h"

void kernel_init() {
    kprintf("Initialization (kernel_init) at %X\n", kernel_init);
    kprintf("Start (kernel_main) at %X\n\n", kernel_main);

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