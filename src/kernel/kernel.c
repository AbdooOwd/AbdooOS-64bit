#include "kernel.h"

#include "../drivers/screen.h"

#include "../lib/util.h"
#include "../lib/string.h"

#include "../cpu/cpu.h"
#include "gdt.h"


bool found_fat = false;

void kernel_init() {
    print("Initializing GDT...\n");
    gdt_init();

    print("GDT Loaded at: ");
    char gdt_addr_str[69];
    hex_to_ascii(gdt_addr, gdt_addr_str);
    print(gdt_addr_str);
    print("\n");
    
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