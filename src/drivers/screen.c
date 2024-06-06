#include "screen.h"
#include <include/fonts.h>
#include <lib/string.h>
#include <kernel/io.h>


struct limine_framebuffer *framebuffer;
volatile u32* fb_addr;

Vector2 font_dimensions = {8, 8};
Vector2 cursor_position = {0, 0};


void set_pixel(int x, int y, u32 color) {
    if ((x >= 0 && (u64) x < framebuffer->width) && (y >= 0 && (u64) y < framebuffer->height))
        fb_addr[get_offset(x, y)] = color;
}

void scroll_pixel_line() {
    /*
        The idea: start by 1st line, clear it, save the next line (start of real iteration)
        then set previous line to this saved line. EZ enough.
    */
    for (size_t line = 0; line < SCREEN_HEIGHT; line++) {
        for (size_t pixel = 0; pixel < SCREEN_WIDTH; pixel++) {

            if (line == 0) {
                // clear first line
                set_pixel(pixel, line, BLACK);
                continue;
            }

            // empty last line
            //if (line + 1 >= SCREEN_HEIGHT) {
            //    set_pixel(pixel, line, BLACK);
            //    continue;
            //}

            set_pixel(pixel, line - 1, fb_addr[get_offset(pixel, line)]);
        }
    }
}




// stolen from "https://github.com/lucianoforks/tetris-os/blob/master/src/font.c"
void print_char_at(char c, int x, int y, u32 color) {
#if DEBUG_E9_PORT
    logc(c);
#endif
    if (c == '\n') {
        y++;
        x = 0;
    } else {
        const u8 *glyph = font[(size_t) c];

        for (size_t yy = 0; yy < (size_t) font_dimensions.y; yy++) {
            for (size_t xx = 0; xx < (size_t) font_dimensions.x; xx++) {
                if (glyph[yy] & (1 << xx))
                    // Set the character's pixels
                    set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, color);
                else 
                    // erase anything that's under the char (haha bye unwanted cursor)
                    set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, BLACK);
            }
        }
    }


    // handle screen's limit printing
    if (x + 1 >= SCREEN_WIDTH / 8) {
        x = 0;
        y++;
    } else if (c != '\n') x++;
    /*  ^  ^  ^  ^  ^  ^  ^  ^
        We put this in an else-if instead of a single if cuz
        or else it will offset the char by one when returning 
        on line after reaching the screen's border
    */

    // scroll handling
    if (y + 1 >= SCREEN_HEIGHT / 8) {
        for (size_t i = 0; i < 8; i++) {
            scroll_pixel_line();
            y = SCREEN_HEIGHT / 8 - 2;
            x = 0;
        }
    }

    set_cursor(x, y);
}

void print_string_at(char* str, int x, int y, u32 color) {
    
    if (x < 0 || y < 0) {
        x = get_cursor().x;
        y = get_cursor().y;
    }

    while (*str) {
        print_char_at(*str, x, y, color);

        if (*str == '\n') y++;
        else x++;

        str++;
    }
}

void success(char* success_msg) {
    print_string_at(success_msg, -1, -1, GREEN);
}

void err(char* error) {
    print_string_at(error, -1, -1, RED);
}

void print(char* str) {
    print_string_at(str, -1, -1, WHITE);
}



void draw_cell(int x, int y, u32 color) {
    for (size_t yy = 0; yy < (size_t) font_dimensions.y; yy++) {
        for (size_t xx = 0; xx < (size_t) font_dimensions.x; xx++) {
            set_pixel((x * font_dimensions.x) + xx, (y * font_dimensions.y) + yy, color);
        }
    } 
}

void fill_screen(u32 color) {
    for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        fb_addr[i] = color;
    }
}


void clear_screen() {
    fill_screen(BLACK);
    set_cursor(0, 0);
}


u32 get_offset(int x, int y) {
    return y * SCREEN_WIDTH + x;
}



Vector2 get_cursor() {
    return cursor_position;
}

void set_cursor(int x, int y) {
    // remove previous cursor (WARNING: will overwrite previous char to (; )
    // draw_cell(cursor_position.x * 8, cursor_position.y * 8, BLACK);

    cursor_position.x = x; // new position
    cursor_position.y = y;

    // draw_cursor(x, y);
}

void draw_cursor(int x, int y) {
    // Fill the bottom two lines of the cell with white
    for (size_t line = 1; line < 3; line++) {
        for (size_t x_pixel = 0; x_pixel < (size_t) font_dimensions.x; x_pixel++) {
            set_pixel(x * font_dimensions.x + x_pixel, y * font_dimensions.y + font_dimensions.y - line, WHITE);
        }
    }
}