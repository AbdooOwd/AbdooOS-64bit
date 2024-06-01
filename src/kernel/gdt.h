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
} __attribute__((packed)) GDTEntry;

typedef struct {
    u16 length;
    u16 base_low;
    u8  base_mid;
    u8  flags1;
    u8  flags2;
    u8  base_high;
    u32 base_upper;
    u32 reserved;
} __attribute__((packed)) TSSEntry;

typedef struct {
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
} __attribute__((packed)) tss_t;

typedef struct {
    GDTEntry gdt_entries[GDT_ENTRY_COUNT];
    TSSEntry tss;
} __attribute__((packed)) GDT;

typedef struct {
    u16 limit;
    u64 base;
} __attribute__((packed)) GDTR;


void GDT_init();
void GDT_load(GDTR le_gdt_pointer);
void GDT_setEntry(u8 num, u32 base, u16 limit, u8 access, u8 flags);

void tss_init(void);
void tss_load();
void tss_set_rsp0(u64 rsp0);

#endif
