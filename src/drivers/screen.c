#include "screen.h"
#include <include/fonts.h>
#include <include/globals.h>
#include <lib/string.h>
#include <lib/print.h>
#include <kernel/io.h>
#include <kernel/kernel.h>

struct limine_framebuffer *framebuffer;
volatile u32* fb_addr;

Vector2 font_dimensions = {
    .x = 8,
    .y = 16
};
Vector2 cursor_position = {0, 0};
u8 cursor_delay = 1;
bool cursor_blinked = false;

bool use_altFont = true;


void set_pixel(int x, int y, u32 color) {
    if ((x >= 0 && (u64) x < framebuffer->width) && (y >= 0 && (u64) y < framebuffer->height))
        fb_addr[get_offset(x, y)] = color;
}

u32 get_pixel(int x, int y) {
    if ((x >= 0 && (u64) x < framebuffer->width) && (y >= 0 && (u64) y < framebuffer->height))
        return fb_addr[get_offset(x, y)];
    return BLACK;
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
        for (size_t yy = 0; yy < (size_t) font_dimensions.y; yy++) {
            for (size_t xx = 0; xx < (size_t) font_dimensions.x; xx++) {
                bool can_draw;

                // handle two fonts
                if (use_altFont)  can_draw = (ibm_pgc[c * font_dimensions.y + yy]) & (1 << (7 - xx));
                else                can_draw = (font[(size_t) c][yy]) & (1 << xx);

                if (can_draw)
                    // Set the character's pixels
                    set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, color);
                else 
                    // erase anything that's under the char (haha bye unwanted cursor)
                    set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, BLACK);
            }
        }

        /*for (int yy = 0; yy < font_dimensions.y; yy++) {
	    	u8 selection = moon_font[c * font_dimensions.y + yy];
	    	for (int xx = 0; xx < font_dimensions.x; xx++)
	    	{
	    		bool can_draw = selection & (1 << (7 - xx));
	    		if (can_draw)
	    			set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, WHITE);
	    		else
	    			set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, BLACK);
	    	}
	    }*/
    }


    // handle screen's limit printing
    if (x + 1 >= (int) SCREEN_WIDTH / font_dimensions.x) {
        x = 0;
        y++;
    } else if (c != '\n') x++;
    /*  ^  ^  ^  ^  ^  ^  ^  ^
        We put this in an else-if instead of a single if cuz
        or else it will offset the char by one when returning 
        on line after reaching the screen's border
    */

    // scroll handling
    if (y + 1 >= (int) (SCREEN_HEIGHT / font_dimensions.y)) {
        for (size_t i = 0; i < (size_t) font_dimensions.y; i++) {
            scroll_pixel_line();
            y = SCREEN_HEIGHT / font_dimensions.y - 2;  // another -1 cuz screen_height doesn't start from 0
            x = 0;
        }
    }

    if(!isEditingHorizontally) set_cursor(x, y);    // for shell commands
}

void print_string_at(char* str, int x, int y, u32 color) {

    int x_start = x;
    int y_start = y;
    
    if (x < 0 || y < 0) {
        x = get_cursor().x;
        y = get_cursor().y;
    }

    while (*str) {
        print_char_at(*str, x, y, color);

        if (*str == '\n') { y++; x = x_start; }
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

void print_color(char* str, u32 color) {
    print_string_at(str, -1, -1, color);
}

void print(char* str) {
    print_string_at(str, -1, -1, WHITE);
}

void print_backspace() {

    u32 new_x = get_cursor().x - 1;
    u32 new_y = get_cursor().y;

    if (new_x <= 0) {
        new_x = SCREEN_WIDTH - 2;
        if (new_y != 0) new_y--;
    }

    if (new_x > 0 && new_y > 0) { 
        print_char_at(0, new_x, new_y, WHITE);
        print_char_at(0, new_x + 1, new_y, WHITE);  // remove cursor
        set_cursor(new_x, new_y);
        iowait();
        print_cursor(true);
    }
}

void invert_char_colors(int x, int y) {
    for (size_t yy = 0; yy < (size_t) font_dimensions.y; yy++) {
        for (size_t xx = 0; xx < (size_t) font_dimensions.x; xx++) {
            // Assuming we only use Black/White
            // TODO: support any color invertion
            if (get_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy) == WHITE)
                set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, BLACK);
            else
                set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, WHITE);
        }
    }
}

void color_char(int x, int y, u32 color) {
    for (size_t yy = 0; yy < (size_t) font_dimensions.y; yy++) {
        for (size_t xx = 0; xx < (size_t) font_dimensions.x; xx++) {
            // Assuming we only use Black/White
            // TODO: support any color invertion
            if (color == WHITE)
                set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, BLACK);
            else
                set_pixel(x * font_dimensions.x + xx, y * font_dimensions.y + yy, WHITE);
        }
    }
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

void move_cursor(int x, int y) {
    set_cursor(get_cursor().x + x, get_cursor().y + y);
}

void set_cursor(int x, int y) {

    cursor_position.x = x; // new position
    cursor_position.y = y;
}

void draw_cursor(int x, int y) {
    // Fill the bottom two lines of the cell with white
    for (size_t line = 1; line < 3; line++) {
        for (size_t x_pixel = 0; x_pixel < (size_t) font_dimensions.x; x_pixel++) {
            set_pixel(x * font_dimensions.x + x_pixel, y * font_dimensions.y + font_dimensions.y - line, WHITE);
        }
    }
}

void print_cursor(bool visible) {
    if ((u64) get_cursor().x >= (SCREEN_WIDTH / font_dimensions.x) - 1) return;
    if (isEditingHorizontally)  return;

    if (visible) print_char_at('_', cursor_position.x, cursor_position.y, WHITE);
    else print_char_at(' ', cursor_position.x, cursor_position.y, BLACK);
    cursor_position.x -= 1;
}

void blink_cursor() {
    cursor_blinked = !cursor_blinked;
    print_cursor(cursor_blinked);
}

void print_entry() {
    kprintf(" - - - AbdooOS %s | %s Bootloader Version %s - - -\n\n", ABDOOOS_VERSION, 
        bootloader_info.response->name, bootloader_info.response->version);
}

void GUI_init() {
    for (u16 yyy = SCREEN_HEIGHT - TASKBAR_HEIGHT; yyy < SCREEN_HEIGHT; yyy++)
        for (u16 xxx = 0; xxx < SCREEN_WIDTH; xxx++)
            if (yyy == SCREEN_HEIGHT - TASKBAR_HEIGHT)
                set_pixel(xxx, yyy, WHITE);
            else
                if (xxx % TASKBAR_ICON_WIDTH == 0)
                    set_pixel(xxx, yyy, WHITE);
                else
                    set_pixel(xxx, yyy, BLACK);
}