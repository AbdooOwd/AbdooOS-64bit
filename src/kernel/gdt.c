#include "gdt.h"
#include "../lib/print.h"
#include <include/globals.h>

u64 gdt_addr;

GDT   the_gdt;
GDTR  gdt_ptr;
tss_t the_tss;


void GDT_setEntry(u8 num, u32 base, u16 limit, u8 access, u8 flags) {
    the_gdt.gdt_entries[num].limit = (limit & 0xFFFF);

    the_gdt.gdt_entries[num].base_low = (base & 0xFFFF);
    the_gdt.gdt_entries[num].base_mid = (base << 16) & 0xFF;
    the_gdt.gdt_entries[num].base_high = (base << 24) & 0xFF;

    the_gdt.gdt_entries[num].access_byte = access;
    the_gdt.gdt_entries[num].flags = (((limit >> 16) & 0xF) | (flags & 0xF0));
}

void GDT_load(GDTR le_gdt_pointer) {
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
    print("Initializing GDT...\n");

    gdt_ptr.limit = sizeof(the_gdt) - 1;
    gdt_ptr.base = (u64) &the_gdt;

    gdt_addr = gdt_ptr.base;

    GDT_setEntry(0, 0, 0, 0, 0);                // Null-Segment                 0x00
    GDT_setEntry(1, 0, 0xFFFFF, 0x9A, 0xA0);    // Kernel 64bit Code Segment    0x08
    GDT_setEntry(2, 0, 0xFFFFF, 0x92, 0xC0);    // Kernel 64bit Data Segment    0x10
    GDT_setEntry(3, 0, 0xFFFFF, 0xFA, 0xA0);    // User   64bit Data Segment    0x18
    GDT_setEntry(4, 0, 0xFFFFF, 0xF2, 0xC0);    // User   64bit Data Segment    0x20
    
    // TSS 0x28
    the_gdt.tss.length = 104;
    the_gdt.tss.base_low = (u16) (((u64) &the_tss) & 0xffff);
    the_gdt.tss.base_mid = (u8) (((u64) &the_tss >> 16) & 0xff);
    the_gdt.tss.flags1 = 0b10001001;
    the_gdt.tss.flags2 = 0;
    the_gdt.tss.base_high = (u8) (((u64) &the_tss >> 24) & 0xff);
    the_gdt.tss.base_upper = (u32) (((u64) &the_tss >> 32) & 0xffffffff);
    the_gdt.tss.reserved = 0;
    
    GDT_load(gdt_ptr);
    kprintf("Loaded GDT at %X\n\n", gdt_addr);
    tss_init();
}

void tss_load(void) {
    kprintf("Loading TSS at %X...\n", &the_gdt.tss);
    // this must be the offset of tss
    asm volatile("mov $0x28, %%ax\n\t"
                 "ltr %%ax\n\t"
                 :
                 :
                 : "rax", "memory");
}

void tss_set_rsp0(u64 rsp0) { the_tss.rsp0 = rsp0; }

void tss_init(void) {
    the_tss.resereved0 = 0;
    the_tss.rsp0 = 0;
    the_tss.rsp1 = 0;
    the_tss.rsp2 = 0;
    the_tss.resereved1 = 0;
    the_tss.resereved2 = 0;
    the_tss.ist1 = 0;
    the_tss.ist2 = 0;
    the_tss.ist3 = 0;
    the_tss.ist4 = 0;
    the_tss.ist5 = 0;
    the_tss.ist6 = 0;
    the_tss.ist7 = 0;
    the_tss.resereved3 = 0;
    the_tss.resereved4 = 0;
    the_tss.iomap_offset = 0;

    tss_load();
}
