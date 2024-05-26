#include "kernel.h"

#include "../drivers/screen.h"

#include "../lib/util.h"
#include "../lib/disk.h"

#include "../cpu/cpu.h"


bool found_fat = false;

void init_kernel() {

    kernel_main();
    
}

void kernel_main() {
    print("Booted AbdooOS Successfully!\n");
    print("Searching for FAT32...\n");

    search_fat();
}

void search_fat() {
    if (found_fat) {
        success("Found FAT!\n");
    } else {
        err("ERROR: FAT NOT FOUND\n");
        halt();
    }
}