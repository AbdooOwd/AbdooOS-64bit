#ifndef GDT_H
#define GDT_H

#pragma once

#include "../include/types.h"

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10
#define GDT_ENTRY_COUNT 5

typedef struct {
    u16 limit;
    u16 base_low;
    u8  base_mid;
    u8  access_byte;
    u8  flags;
    u8  base_high;
} __attribute__((packed)) gdt_descriptor;


typedef struct {
    u16 limit;
    u64 base;
} __attribute__((packed)) gdtr;


extern gdt_descriptor* gdt_addr;

void GDT_init();
void GDT_load(gdtr le_gdt_pointer);
void GDT_setEntry(u8 num, u32 base, u16 limit, u8 access, u8 flags);

#endif
