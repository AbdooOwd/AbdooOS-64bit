#include "kernel.h"

#include "../drivers/screen.h"

#include "../lib/util.h"
#include "../lib/string.h"



bool found_fat = false;

void init_kernel() {

    // put here whatever you cannot initialize outside a function

    kernel_main();
}

void kernel_main() {
    print("Booted AbdooOS Successfully!\n");
    print("Searching for FAT32...\n");

    if (found_fat) {
        print("Found FAT!\n");
    } else {
        err("ERROR: FAT NOT FOUND\n");
        halt();
    }
}