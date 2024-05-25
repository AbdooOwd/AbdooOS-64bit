#ifndef UTIL_H
#define UTIL_H

#pragma once

typedef struct {
    int x;
    int y;
} __attribute__((packed)) Vector2;

void array_copy(unsigned char* source, unsigned char* dest);
char* int_to_str(int num);


static void halt(void) __attribute__((unused));
static void halt(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

#endif