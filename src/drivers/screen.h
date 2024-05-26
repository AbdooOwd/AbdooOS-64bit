#ifndef SCREEN_H
#define SCREEN_H

#pragma once

#include "../include/types.h"
#include "../limine.h"
#include "../lib/util.h"



// framebuffer related

extern struct limine_framebuffer *framebuffer;
extern volatile u32* fb_addr;

#define SCREEN_WIDTH    framebuffer->width
#define SCREEN_HEIGHT   framebuffer->height

#define WHITE   0xffffff
#define BLACK   0x0
#define RED     0xff0000
#define GREEN   0x00ff00


// text data
extern Vector2 font_dimensions;
extern Vector2 cursor_position;

// functions

u32 get_offset(int x, int y);
void set_cursor(int x, int y);
Vector2 get_cursor();
void draw_cursor(int x, int y);

void set_pixel(int x, int y, u32 color);
void fill_screen(u32 color);
void draw_cell(int x, int y, u32 color);

void err(char* error);
void success(char* success_msg);
void print(char* str);
void print_char_at(char c, int x, int y, u32 color);
void print_string_at(char* str, int x, int y, u32 color);

#endif