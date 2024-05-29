#include "kernel.h"

#include "../drivers/screen.h"

#include "../lib/util.h"
#include "../lib/formatter.h"

#include "../lib/print.h"

#include "../cpu/cpu.h"
#include "gdt.h"


bool found_fat = false;

void kernel_init() {
    print("Initializing GDT...\n");
    gdt_init();

    kprintf("GDT Loaded at: %X\n", gdt_addr);
    
    kernel_main();
}

void kernel_main() {
    print("\nBooted AbdooOS Successfully!\n");
}

void search_fat() {
    if (found_fat) {
        success("Found FAT!\n");
    } else {
        err("ERROR: FAT NOT FOUND\n");
        halt();
    }
}