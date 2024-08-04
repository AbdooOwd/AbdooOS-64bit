#include "kernel.h"
#include <limine.h>
#include <cpu/cpu.h>
#include <drivers/screen.h>
#include <include/globals.h>
#include <lib/print.h>
#include <lib/mem.h>
#include "hal/hal.h"

volatile struct limine_bootloader_info_request bootloader_info = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

void kernel_init() {
    kprintf("Initialization (kernel_init) at %X\n", kernel_init);
    kprintf("Start (kernel_main) at %X\n\n", kernel_main);

    pmm_init();
    kprintf("\n");
    HAL_init();

    if (alloc_test()) {
        kprintf("Allocation test complete! (Allocated and Free'd 8 bytes)\n\n");
    } else {
        kprintf("Memory Allocation Test Failed (8 bytes allocation failed)\n");
        panic("Halting CPU (Allocation Test Fail)\n");
    }

    print("Booted AbdooOS Successfully!\n");

    kernel_main();
}

void kernel_main() {
    clear_screen();
    print_entry();
    print_color("$ ", GREEN);

    for (;;) ;
}