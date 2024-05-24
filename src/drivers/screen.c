#include "screen.h"
#include "../include/fonts.h"
#include <stdint.h>
#include "../lib/util.h"



struct limine_framebuffer *framebuffer;
volatile uint32_t* fb_addr;

uint8_t font_dimensions[2] = {8, 8};


void set_pixel(int x, int y, uint32_t color) {
    if ((x >= 0 && (uint64_t) x < framebuffer->width) && (y >= 0 && (uint64_t) y < framebuffer->height))
        fb_addr[get_offset(x, y)] = color;
}


// stolen from "https://github.com/lucianoforks/tetris-os/blob/master/src/font.c"
void print_char_at(char c, size_t x, size_t y, size_t size, u32 color) {
    const u8 *glyph = font[(size_t) c];

    for (size_t yy = 0; yy < font_dimensions[1]; yy++) {
        for (size_t xx = 0; xx < font_dimensions[0]; xx++) {
            if (glyph[yy] & (1 << xx)) {
                set_pixel(x + xx, y + yy, color);
            }
        }
    }
}

void print_string_at(char* str, int x, int y, uint8_t size, uint32_t color) {
    int start_x = x;
    int start_y = y;


    while (*str) {
        if (*str == '\n') { 
            y += font_dimensions[1] * size;
            x = start_x;
            str++;
        }
        print_char_at(*str, x, y, size, color);
        x += font_dimensions[0] * size;
        str++;
    }
}


void fill_screen(uint32_t color) {
    for (size_t i = 0; i < framebuffer->width * framebuffer->height; i++) {
        fb_addr[i] = color;
    }
}

uint32_t get_offset(int x, int y) {
    return y * framebuffer->width + x;
}