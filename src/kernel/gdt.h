#ifndef GDT_H
#define GDT_H


#include "../cpu/cpu.h"
#include <stdint.h>

struct tss_t {
    u32 resereved0;
    u64 rsp0;
    u64 rsp1;
    u64 rsp2;
    u64 resereved1;
    u64 resereved2;
    u64 ist1;
    u64 ist2;
    u64 ist3;
    u64 ist4;
    u64 ist5;
    u64 ist6;
    u64 ist7;
    u64 resereved3;
    u16 resereved4;
    u16 iomap_offset;
} __attribute__((packed));

struct tss_descriptor {
    u16 length;
    u16 base_low16;
    u8  base_mid8;
    u8  flags1;
    u8  flags2;
    u8  base_high8;
    u32 base_upper32;
    u32 reserved;
};

struct gdt_descriptor {
    u16 limit;
    u16 base_low16;
    u8  base_mid8;
    u8  access;
    u8  granularity;
    u8  base_high8;
};

struct gdt {
    struct gdt_descriptor descriptors[9];
    struct tss_descriptor tss;
};

struct gdtr {
    u16 limit;
    u64 address;
} __attribute__((packed));


extern int gdt_addr;

void gdt_init();

void tss_init();

void gdt_load();

void tss_set_rsp0(u64 rsp0);

#endif