#ifndef SCREEN_H
#define SCREEN_H

#include "../include/types.h"
#include "../limine.h"

extern struct limine_framebuffer *framebuffer;
extern volatile uint32_t* fb_addr;


extern uint8_t font_dimensions[2];      // x and y size


uint32_t get_offset(int x, int y);

void set_pixel(int x, int y, uint32_t color);
void fill_screen(uint32_t color);

void print_char_at(char c, size_t x, size_t y, size_t size, u32 color);
void print_string_at(char* str, int x, int y, uint8_t size, uint32_t color);

#endif