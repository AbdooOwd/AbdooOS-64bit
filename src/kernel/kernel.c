#include "kernel.h"
#include "../drivers/screen.h"
#include "../lib/util.h"

void init_kernel() {

    // put here whatever you cannot initialize outside a function

    kernel_main();
}

void kernel_main() {
    print("Hello World!\n");
    print("Pigeons are attacking!");
}