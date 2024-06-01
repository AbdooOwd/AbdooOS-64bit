#include "low_level.h"
#include <include/types.h>

unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile("in %%dx , %% al" : "=a" (result) : "d" (port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    asm volatile(" out %%al , %% dx" : : "a" (data), "d" (port));
}

unsigned short inw(unsigned short port) {
    unsigned short result;
    asm volatile("in %%dx , %% ax" : "=a" (result) : "d" (port));
    return result;
}

void outw(unsigned short port, unsigned short data) {
    asm volatile(" out %%ax , %% dx" : : "a" (data), "d" (port));
}

static inline void outb(u16 port, u8 val) {
    asm volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a u16.  %w1 could be used if we had the port number a wider C type */
}

static inline u8 inb(u16 port) {
    u8 ret;
    asm volatile ("inb %w1, %b0"
        : "=a"(ret)
        : "Nd"(port)
        : "memory");
    return ret;
}