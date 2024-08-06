#ifndef SCREEN_H
#define SCREEN_H

#pragma once

#include <include/types.h>
#include <limine.h>
#include <lib/util/util.h>



// framebuffer related

extern struct limine_framebuffer *framebuffer;
extern volatile u32* fb_addr;


#define SCREEN_WIDTH    framebuffer->width
#define SCREEN_HEIGHT   framebuffer->height

#define WHITE   0xffffff
#define BLACK   0x0
#define RED     0xff0000
#define GREEN   0x00ff00

#define DEBUG_E9_PORT 1


// text data
extern Vector2 font_dimensions;
extern Vector2 cursor_position;
extern u8 cursor_delay;

// functions

u32  get_offset(int x, int y);
void set_cursor(int x, int y);
Vector2 get_cursor();
void move_cursor(int x, int y);
void draw_cursor(int x, int y);
void print_cursor(bool visible);
void blink_cursor();

void invert_char_colors(int x, int y);
void color_char(int x, int y, u32 color);

void set_pixel(int x, int y, u32 color);
u32  get_pixel(int x, int y);
void scroll_pixel_line();
void scrollFramebuffer(u32 rows);
void fill_screen(u32 color);
void draw_cell(int x, int y, u32 color);
void clear_screen();

void err(char* error);
void success(char* success_msg);
void print(char* str);
void print_color(char* str, u32 color);
void print_char_at(char c, int x, int y, u32 color);
void print_string_at(char* str, int x, int y, u32 color);
void print_backspace();

void print_entry();

#endif