#include "kernel.h"
#include <cpu/cpu.h>
#include "../drivers/screen.h"
#include "../lib/util/util.h"
#include "../lib/print.h"
#include <include/globals.h>


void kernel_init() {
    kprintf("Initialization (kernel_init) at %X\n", kernel_init);
    kprintf("Start (kernel_main) at %X\n\n", kernel_main);

    HAL_init();

    print("Booted AbdooOS Successfully!\n");
    // clear_screen();

    kernel_main();
}

void kernel_main() {
    print("Welcome to your life!\n");
    
    halt();
}