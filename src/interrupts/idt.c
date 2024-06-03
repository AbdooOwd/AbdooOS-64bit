#include "idt.h"
#include "isr.h"

IDTEntry idt[IDT_ENTRY_COUNT];

void IDT_setEntry(u8 vector, void* handler, u8 dpl) {
	u64 handler_addr = (u64) handler;

	IDTEntry* entry = &idt[vector];

	entry->base_low = handler_addr & 0xFFFF;
	entry->base_mid = (handler_addr >> 16) & 0xFFFF;
	entry->base_high = (handler_addr >> 32) & 0xFFFFFFFF;
	
	entry->selector = 0x08;		// Code segment

	// idk what i'm doing
	entry->flags = 0b1110 | ((dpl & 0b11) << 5) | (1 << 7);
	entry->ist = 0;		// i don't have a tss soooooo
}

void IDT_load(void* IDT_addr) {
	/**
	 *		 - Why the IDTR is declared here and not globally -
	 *  Because we will only need to load the IDT using the IDTR
	 * here. So when the IDTR will be loaded, there will be
	 * a copy of the IDTR used by the computer. 
	*/

	IDTR IDT_ptr;
	IDT_ptr.limit = 0xFFF;
	IDT_ptr.base = (u64) IDT_addr;
	asm volatile ("lidt %0" :: "m"(IDT_ptr) : "memory");
}

void IDT_init() {

	ISR_initGates();
	IDT_load(&idt);
}