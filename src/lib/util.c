#include "util.h"
#include "../kernel/gdt.h"

void array_copy(unsigned char* source, unsigned char* dest) {
    for (size_t i = 0; i < sizeof(source); i++) {
        dest[i] = source[i];
    }
}

// works when KASLR on
int random() {
    // haha playing with addresses to generate a random number until I make a timer
    return (u8) ((gdt_addr >> 16) / 3 + 5 * (4 - 2 / ((u8) gdt_init)) - 12);
}