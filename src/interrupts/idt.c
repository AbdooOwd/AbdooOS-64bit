#include "idt.h"
#include "../lib/util/binary.h"

IDTEntry    idt[IDT_ENTRY_COUNT];
IDTR        idtr = { sizeof(idt) - 1, &idt };

void IDT_load(IDTR idt_pointer) {
    asm volatile("lidt %0\n\t"
                 :
                 : "m"(idt_pointer));
}

void IDT_setGate(int interrupt, void* base, u16 segmentSelector, u8 ist, u8 flags) {
    idt[interrupt].base_low = ((u64) base) & 0xFFFF;
    idt[interrupt].base_mid = (((u64) base) >> 16) & 0xFFFF;
    idt[interrupt].base_high = (((u64) base) >> 32) & 0xFFFFFFFF;
    idt[interrupt].selector = segmentSelector;
    idt[interrupt].ist = ist & 0x7;  // Only the lower 3 bits are used for IST
    idt[interrupt].flags = flags;
    idt[interrupt].reserved = 0;  // Reserved field should be set to 0
}
void IDT_enableGate(int interrupt) {
    FLAG_SET(idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void IDT_disableGate(int interrupt) {
    FLAG_UNSET(idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void IDT_init() {
    IDT_load(idtr);
}