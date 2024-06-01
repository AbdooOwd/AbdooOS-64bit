#include "isr.h"
#include "idt.h"
#include "../lib/print.h"
#include <cpu/cpu.h>

static const char* exception_messages[] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

// a list of handling functions
ISR_handler_t ISR_handlers[IDT_ENTRY_COUNT];

// generated in isr_gen.c
void ISR_initGates();

void ISR_init() {
    ISR_initGates();
    
    // enable every gate
    for (size_t i = 0; i < IDT_ENTRY_COUNT; i++)
        IDT_enableGate(i);
}

void ISR_handler(registers* regs) {
    if (ISR_handlers[regs->int_number] != NULL)
        ISR_handlers[regs->int_number](regs);       // call the handler function
    else if (regs->int_number >= 32)
        panic("Unhandled Interrupt %d!\n", regs->int_number);
    else {
        kprintf("Unhanlded Exception %d: %s!\n", 
            regs->int_number, exception_messages[regs->int_number]);
        
        // print all registers so the user understands the issue
        kprintf("  rbp=%x  r15=%x  r14=%x  r13=%x  r12=%x\n",
            regs->rbp, regs->r15, regs->r14, regs->r13, regs->r12);
        kprintf("  r11=%x  r10=%x  r9=%x  r8=%x\n",
            regs->r11, regs->r10, regs->r9, regs->r8);
        kprintf("  rsi=%x  rdi=%x  rdx=%x  rcx=%x  rbx=%x  rax=%x\n",
            regs->rsi, regs->rdi, regs->rdx, regs->rcx, regs->rbx, regs->rax);

        kprintf("  interrupt=%x  errorcode=%x\n", regs->int_number, regs->error_code);

        panic("\nPanic! Halting CPU");
    }
}