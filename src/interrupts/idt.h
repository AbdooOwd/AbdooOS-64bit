#pragma once

#include <include/types.h>

#define IDT_ENTRY_COUNT 	256

typedef struct {
	u16 base_low;
	u16 selector;
	u8  ist;
	u8  flags;
	u16 base_mid;
	u32 base_high;
	u32 reserved;
} __attribute__((packed)) IDTEntry;

typedef struct {
	u16 limit;
	u64 base;
} __attribute__((packed)) IDTR;

void IDT_setEntry(u8 vector, void* handler, u8 dpl);
void IDT_load(void* IDT_addr);
void IDT_init();
