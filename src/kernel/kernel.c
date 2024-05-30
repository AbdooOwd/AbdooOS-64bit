#include "kernel.h"
#include "../cpu/cpu.h"
#include "../drivers/screen.h"
#include "../lib/util.h"
#include "../lib/print.h"
#include "gdt.h"


void kernel_init() {
    kprintf("Initialization (kernel_init) at %X\n", kernel_init);
    kprintf("Start (kernel_main) at %X\n\n", kernel_main);

    print("Initializing GDT...\n");
    GDT_init();
    kprintf("Loaded GDT at %X\n\n", gdt_addr);

    print("Booted AbdooOS Successfully!\n");
    // clear_screen();

    kernel_main();
}

void kernel_main() {
    print("Welcome to your life!\n");
    halt();
}