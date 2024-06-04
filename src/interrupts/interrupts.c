#include "interrupts.h"
#include <cpu/cpu.h>
#include <lib/print.h>

#define IDT_DEFAULT_GATE(i) idt_set_gate(i, (uint64_t) interrupt_handler_ ## i, 0x08, IDT_ATTR_PRESENT | IDT_ATTR_INTERRUPT_GATE);

struct idtr idtr;
struct idt_entry idt_entries[256];

char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Timer",
};


void idt_load(void) {
    asm volatile("lidt %0" : : "m"(idtr) : "memory");
    asm volatile("sti");
}

void interrupts_init(void) {
    IDT_DEFAULT_GATE(0)
    IDT_DEFAULT_GATE(1)
    IDT_DEFAULT_GATE(2)
    IDT_DEFAULT_GATE(3)
    IDT_DEFAULT_GATE(4)
    IDT_DEFAULT_GATE(5)
    IDT_DEFAULT_GATE(6)
    IDT_DEFAULT_GATE(7)
    IDT_DEFAULT_GATE(8)
    IDT_DEFAULT_GATE(10)
    IDT_DEFAULT_GATE(11)
    IDT_DEFAULT_GATE(12)
    IDT_DEFAULT_GATE(13)
    IDT_DEFAULT_GATE(14)
    IDT_DEFAULT_GATE(16)
    IDT_DEFAULT_GATE(17)
    IDT_DEFAULT_GATE(18)

	IDT_DEFAULT_GATE(32)
	IDT_DEFAULT_GATE(33)
	IDT_DEFAULT_GATE(34)
	IDT_DEFAULT_GATE(35)
	IDT_DEFAULT_GATE(36)
	IDT_DEFAULT_GATE(37)
	IDT_DEFAULT_GATE(38)
	IDT_DEFAULT_GATE(39)
	IDT_DEFAULT_GATE(40)
	IDT_DEFAULT_GATE(41)
	IDT_DEFAULT_GATE(42)
	IDT_DEFAULT_GATE(43)
	IDT_DEFAULT_GATE(44)
	IDT_DEFAULT_GATE(45)
	IDT_DEFAULT_GATE(46)
	IDT_DEFAULT_GATE(47)

    idtr.base = (uint64_t) idt_entries;
    idtr.limit = 256 * sizeof(struct idt_entry) - 1;

    idt_load();
    __asm__("sti");
    kprintf("idt loaded\n");
}

void idt_set_gate(int i, u64 handler, u16 cs, u16 attr) {
    idt_entries[i].base_low = (u16) (handler & 0xffff);
    idt_entries[i].base_mid = (u16) (handler >> 16) & 0xffff;
    idt_entries[i].base_high = (u32) (handler >> 32) & 0xffffffff;
    idt_entries[i].flags = attr;
    idt_entries[i].reserved = 0;
    idt_entries[i].selector = cs;
}

void interrupt(execution_context regs)
{
    if (regs.interrupt <= 32) {
        kprintf("Interrupt %x - error code: %x - rip: %x\n", regs.interrupt, regs.error_code, regs.iret_rip);
		kprintf("Exception: %s\n", exception_messages[regs.interrupt]);
        panic("unexpected interrupt");
    } else {
		kprintf("interrupt %d\n", regs.interrupt);
    }
}