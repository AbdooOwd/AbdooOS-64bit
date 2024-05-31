#include "gdt.h"
#include "../lib/print.h"

gdt_descriptor* gdt_addr;

gdt_descriptor gdt[GDT_ENTRY_COUNT];
gdtr gdt_ptr;


void GDT_setEntry(u8 num, u32 base, u16 limit, u8 access, u8 flags) {
    gdt[num].limit = (limit & 0xFFFF);

    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_mid = (base << 16) & 0xFF;
    gdt[num].base_high = (base << 24) & 0xFF;

    gdt[num].access_byte = access;
    gdt[num].flags = (((limit >> 16) & 0xF) | (flags & 0xF0));
}

void GDT_load(gdtr le_gdt_pointer) {
    asm volatile("lgdt %0\n\t"
                 "push $0x08\n\t"
                 "lea 1f(%%rip), %%rax\n\t"
                 "push %%rax\n\t"
                 "lretq\n\t"
                 "1:\n\t"
                 "mov $0x10, %%eax\n\t"
                 "mov %%eax, %%ds\n\t"
                 "mov %%eax, %%es\n\t"
                 "mov %%eax, %%fs\n\t"
                 "mov %%eax, %%gs\n\t"
                 "mov %%eax, %%ss\n\t"
                 :
                 : "m"(le_gdt_pointer)
                 : "rax", "memory");
}

void GDT_init() {

    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (u64) &gdt;

    gdt_addr = gdt_ptr.base;

    GDT_setEntry(0, 0, 0, 0, 0);                // Null-Segment                 0x00
    GDT_setEntry(1, 0, 0xFFFFF, 0x9A, 0xA0);    // Kernel 64bit Code Segment    0x08
    GDT_setEntry(2, 0, 0xFFFFF, 0x92, 0xC0);    // Kernel 64bit Data Segment    0x10
    GDT_setEntry(3, 0, 0xFFFFF, 0xFA, 0xA0);    // User   64bit Data Segment    0x18
    GDT_setEntry(4, 0, 0xFFFFF, 0xF2, 0xC0);    // User   64bit Data Segment    0x20
    GDT_load(gdt_ptr);
}