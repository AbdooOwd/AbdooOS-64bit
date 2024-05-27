#include "gdt.h"

static struct gdt   gdt_instance;
static struct gdtr  gdtr_instance;
static struct tss_t tss_instance;
int gdt_addr;

void gdt_load(void) {
    asm volatile("lgdt %0\n\t"
                 "push $0x28\n\t"
                 "lea 1f(%%rip), %%rax\n\t"
                 "push %%rax\n\t"
                 "lretq\n\t"
                 "1:\n\t"
                 "mov $0x30, %%eax\n\t"
                 "mov %%eax, %%ds\n\t"
                 "mov %%eax, %%es\n\t"
                 "mov %%eax, %%fs\n\t"
                 "mov %%eax, %%gs\n\t"
                 "mov %%eax, %%ss\n\t"
                 :
                 : "m"(gdtr_instance)
                 : "rax", "memory");
}

void tss_load(void) {
    // this must be the offset of tss
    asm volatile("mov $0x48, %%ax\n\t"
                 "ltr %%ax\n\t"
                 :
                 :
                 : "rax", "memory");
}

void tss_set_rsp0(u64 rsp0) { tss_instance.rsp0 = rsp0; }

void gdt_init() {
    // Null descriptor. 0x00
    gdt_instance.descriptors[0].limit = 0;
    gdt_instance.descriptors[0].base_low16 = 0;
    gdt_instance.descriptors[0].base_mid8 = 0;
    gdt_instance.descriptors[0].access = 0;
    gdt_instance.descriptors[0].granularity = 0;
    gdt_instance.descriptors[0].base_high8 = 0;

    // Kernel code 16. 0x08
    gdt_instance.descriptors[1].limit = 0xffff;
    gdt_instance.descriptors[1].base_low16 = 0;
    gdt_instance.descriptors[1].base_mid8 = 0;
    gdt_instance.descriptors[1].access = 0b10011010;
    gdt_instance.descriptors[1].granularity = 0b00000000;
    gdt_instance.descriptors[1].base_high8 = 0;

    // Kernel data 16. 0x10
    gdt_instance.descriptors[2].limit = 0xffff;
    gdt_instance.descriptors[2].base_low16 = 0;
    gdt_instance.descriptors[2].base_mid8 = 0;
    gdt_instance.descriptors[2].access = 0b10010010;
    gdt_instance.descriptors[2].granularity = 0b00000000;
    gdt_instance.descriptors[2].base_high8 = 0;

    // Kernel code 32. 0x18
    gdt_instance.descriptors[3].limit = 0xffff;
    gdt_instance.descriptors[3].base_low16 = 0;
    gdt_instance.descriptors[3].base_mid8 = 0;
    gdt_instance.descriptors[3].access = 0b10011010;
    gdt_instance.descriptors[3].granularity = 0b11001111;
    gdt_instance.descriptors[3].base_high8 = 0;

    // Kernel data 32. 0x20
    gdt_instance.descriptors[4].limit = 0xffff;
    gdt_instance.descriptors[4].base_low16 = 0;
    gdt_instance.descriptors[4].base_mid8 = 0;
    gdt_instance.descriptors[4].access = 0b10010010;
    gdt_instance.descriptors[4].granularity = 0b11001111;
    gdt_instance.descriptors[4].base_high8 = 0;

    // Kernel code 64. 0x28
    gdt_instance.descriptors[5].limit = 0;
    gdt_instance.descriptors[5].base_low16 = 0;
    gdt_instance.descriptors[5].base_mid8 = 0;
    gdt_instance.descriptors[5].access = 0b10011010;
    gdt_instance.descriptors[5].granularity = 0b00100000;
    gdt_instance.descriptors[5].base_high8 = 0;

    // Kernel data 64. 0x30
    gdt_instance.descriptors[6].limit = 0;
    gdt_instance.descriptors[6].base_low16 = 0;
    gdt_instance.descriptors[6].base_mid8 = 0;
    gdt_instance.descriptors[6].access = 0b10010010;
    gdt_instance.descriptors[6].granularity = 0;
    gdt_instance.descriptors[6].base_high8 = 0;

    // User code 64. 0x38
    gdt_instance.descriptors[7].limit = 0;
    gdt_instance.descriptors[7].base_low16 = 0;
    gdt_instance.descriptors[7].base_mid8 = 0;
    gdt_instance.descriptors[7].access = 0b11111010;
    gdt_instance.descriptors[7].granularity = 0b00100000;
    gdt_instance.descriptors[7].base_high8 = 0;

    // User data 64. 0x40
    gdt_instance.descriptors[8].limit = 0;
    gdt_instance.descriptors[8].base_low16 = 0;
    gdt_instance.descriptors[8].base_mid8 = 0;
    gdt_instance.descriptors[8].access = 0b11110010;
    gdt_instance.descriptors[8].granularity = 0;
    gdt_instance.descriptors[8].base_high8 = 0;

    // TSS. 0x48
    // NOTE: not sure about the address of tss_instance
    gdt_instance.tss.length = 104;
    gdt_instance.tss.base_low16 =
        (u16) (((u64) &tss_instance) & 0xffff);
    gdt_instance.tss.base_mid8 =
        (u8) (((u64) &tss_instance >> 16) & 0xff);
    gdt_instance.tss.flags1 = 0b10001001;
    gdt_instance.tss.flags2 = 0;
    gdt_instance.tss.base_high8 =
        (u8) (((u64) &tss_instance >> 24) & 0xff);
    gdt_instance.tss.base_upper32 =
        (u32) (((u64) &tss_instance >> 32) & 0xffffffff);
    gdt_instance.tss.reserved = 0;

    gdtr_instance.limit = sizeof(struct gdt) - 1;   // size of table subtracted by 1
    gdtr_instance.address = (u64) &gdt_instance;

    gdt_addr = gdtr_instance.address;

    gdt_load();
    tss_init();
}

void tss_init(void) {
    tss_instance.resereved0 = 0;
    tss_instance.rsp0 = 0;
    tss_instance.rsp1 = 0;
    tss_instance.rsp2 = 0;
    tss_instance.resereved1 = 0;
    tss_instance.resereved2 = 0;
    tss_instance.ist1 = 0;
    tss_instance.ist2 = 0;
    tss_instance.ist3 = 0;
    tss_instance.ist4 = 0;
    tss_instance.ist5 = 0;
    tss_instance.ist6 = 0;
    tss_instance.ist7 = 0;
    tss_instance.resereved3 = 0;
    tss_instance.resereved4 = 0;
    tss_instance.iomap_offset = 0;

    tss_load();
}