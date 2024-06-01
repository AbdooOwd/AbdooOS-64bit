#ifndef IDT_H
#define IDT_H
#pragma once

#include "../include/types.h"

#define IDT_ENTRY_COUNT 256
#define CODE_SEGMENT    0x08

typedef struct {
    u16 base_low;
    u16 selector;
    u8  ist;            // 3bits for IST, rest is reserved (0)
    u8  flags;          // 4bits: Gate Type, 1bit: reserved, 2bits: dpl, 1bit: present bit
    u16 base_mid;
    u32 base_high;
    u32 reserved;
} __attribute__((packed)) IDTEntry;

typedef struct {
    u16 limit;
    IDTEntry* ptr;
} __attribute__((packed)) IDTR;

typedef enum {
    IDT_FLAG_64BIT_INTERRUPT_GATE   = 0xE,
    IDT_FLAG_64BIT_TRAP_GAET        = 0xF,

    IDT_FLAG_RING0                  = (0 << 5),
    IDT_FLAG_RING1                  = (1 << 5),
    IDT_FLAG_RING2                  = (2 << 5),
    IDT_FLAG_RING3                  = (3 << 5),

    IDT_FLAG_PRESENT                = 0x80,
} IDT_FLAGS;


void IDT_init();
void IDT_load(IDTR idt_pointer);
void IDT_setGate(int interrupt, void* base, u16 segmentSelector, u8 ist, u8 flags);
void IDT_enableGate(int interrupt);
void IDT_disableGate(int interrupt);

#endif