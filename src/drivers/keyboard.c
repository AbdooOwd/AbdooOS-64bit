#include "keyboard.h"
#include "screen.h"
#include "tester.h"
#include <interrupts/irq.h>
#include <include/functions.h>
#include <include/globals.h>
#include <lib/print.h>
#include <lib/string.h>
#include <kernel/io.h>
#include <kernel/shell.h>
#include <cpu/pic.h>
#include <cpu/cpu.h>
#include <mm/pmm.h>

#define HORIZONTAL_EDITING  0


#define KBD_DATA_PORT 	0x60
#define KBD_STATUS_PORT KBD_DATA_PORT + 4	// 0x64

#define BACKSPACE 		0x0E
#define ENTER 			0x1C
#define LSHIFT 			0x2A

#define DOWN            0x50
#define UP              0x48
#define RIGHT           0x4D
#define LEFT            0x4B

#define SC_MAX 			80

/*
typedef enum {
    UNKNOWN = 0xFFFFFFFF,
    ESC = 0xFFFFFFFF - 1,
    CTRL = 0xFFFFFFFF - 2,
    LSHFT = 0xFFFFFFFF - 3,
    RSHFT = 0xFFFFFFFF - 4,
    ALT = 0xFFFFFFFF - 5,
    F1 = 0xFFFFFFFF - 6,
    F2 = 0xFFFFFFFF - 7,
    F3 = 0xFFFFFFFF - 8,
    F4 = 0xFFFFFFFF - 9,
    F5 = 0xFFFFFFFF - 10,
    F6 = 0xFFFFFFFF - 11,
    F7 = 0xFFFFFFFF - 12,
    F8 = 0xFFFFFFFF - 13,
    F9 = 0xFFFFFFFF - 14,
    F10 = 0xFFFFFFFF - 15,
    F11 = 0xFFFFFFFF - 16,
    F12 = 0xFFFFFFFF - 17,
    SCRLCK = 0xFFFFFFFF - 18,
    HOME = 0xFFFFFFFF - 19,
    UP = 0xFFFFFFFF - 20,
    LEFT = 0xFFFFFFFF - 21,
    RIGHT = 0xFFFFFFFF - 22,
    DOWN = 0xFFFFFFFF - 23,
    PGUP = 0xFFFFFFFF - 24,
    PGDOWN = 0xFFFFFFFF - 25,
    END = 0xFFFFFFFF - 26,
    INS = 0xFFFFFFFF - 27,
    DEL = 0xFFFFFFFF - 28,
    CAPS = 0xFFFFFFFF - 29,
    NONE = 0xFFFFFFFF - 30,
    ALTGR = 0xFFFFFFFF - 31,
    NUMLCK = 0xFFFFFFFF - 32,
} special_keys;
*/

/*
const u64 layout[128] = {
    UNKNOWN, ESC,     '1',     '2',     '3',     '4',     '5',     '6',
    '7',     '8',     '9',     '0',     '-',     '=',     '\b',    '\t',
    'q',     'w',     'e',     'r',     't',     'y',     'u',     'i',
    'o',     'p',     '[',     ']',     '\n',    CTRL,    'a',     's',
    'd',     'f',     'g',     'h',     'j',     'k',     'l',     ';',
    '\'',    '`',     LSHFT,   '\\',    'z',     'x',     'c',     'v',
    'b',     'n',     'm',     ',',     '.',     '/',     RSHFT,   '*',
    ALT,     ' ',     CAPS,    F1,      F2,      F3,      F4,      F5,
    F6,      F7,      F8,      F9,      F10,     NUMLCK,  SCRLCK,  HOME,
    UP,      PGUP,    '-',     LEFT,    UNKNOWN, RIGHT,   '+',     END,
    DOWN,    PGDOWN,  INS,     DEL,     UNKNOWN, UNKNOWN, UNKNOWN, F11,
    F12,     UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN};
*/

char ascii_US[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' ' };

char shifted_US[] = { 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0 
};


static char input_buffer[256];
static bool shift_pressed;
char* prev_input;
u8 last_line_start = 0;
int input_mover = 0;


InterruptRegisters* keyboard_handler(InterruptRegisters* regs) {
	u8 scancode = inb(KBD_DATA_PORT);

    // if we press an unhandled 
    if (scancode > SC_MAX && !(scancode == LSHIFT || scancode == (LSHIFT | 0x80))) return regs;


    switch (scancode) {     // if pressed
        case LSHIFT:
            shift_pressed = true;
            break;
    }

	if (scancode | 0x80) {	// if released
		switch (scancode) {
			case BACKSPACE:
                if (strlen(input_buffer) > 0) {
				    backspace(input_buffer);
				    print_backspace();
                }
				break;
            
            case ENTER:
                if (!isEditingHorizontally) print_char_at(0, get_cursor().x, get_cursor().y, WHITE);  // remove cursor
                if(isEditingHorizontally) {
                    invert_char_colors(get_cursor().x, get_cursor().y);
                    print_char_at(0, (2 + strlen(input_buffer)) % (SCREEN_WIDTH / 8), get_cursor().y, WHITE);
                }
                
                isEditingHorizontally = false;

                print("\n");
                log("[KB Driver] Input Buffer: %s\n", input_buffer);
                user_input(input_buffer);

                strcpy(prev_input, input_buffer);
                strclr(input_buffer);

                last_line_start = get_cursor().y;
                input_mover = 0;

                break;
            
            case LSHIFT | 0x80:     // we released shift
                shift_pressed = false;
                break;
            
            case UP:
                clear_input_line();
                strcpy(input_buffer, prev_input);
                print(prev_input);
                break;
            
            case DOWN:
                if (last_line_start > get_cursor().y + 5) break;    // safety check
                clear_input_line();
                break;
            
            #if HORIZONTAL_EDITING

            case LEFT:
                // TODO: Implement that scrolly negative x stuff into cursor functions directly
                if (get_cursor().x <= 2) break; // 2 cuz "$ "
                isEditingHorizontally = true;
                if (input_mover > 0) invert_char_colors(get_cursor().x, get_cursor().y);
                move_cursor(-1, 0);
                input_mover++;
                invert_char_colors(get_cursor().x, get_cursor().y);
                log("Input Mover: %i\n", input_mover);
                break;
            
            case RIGHT:
                if (strlen(input_buffer) + 2 <= get_cursor().x) break;
                isEditingHorizontally = true;
                if (input_mover <= strlen(input_buffer)) invert_char_colors(get_cursor().x, get_cursor().y);
                move_cursor(+1, 0);
                if (input_mover > 0) input_mover--;
                invert_char_colors(get_cursor().x, get_cursor().y);
                log("Input Mover: %i\n", input_mover);
                break;
            
            #endif

			default:
                char c = getch(scancode);
                if (shift_pressed) {
                    if (shifted_US[scancode] == 0)
                        c = upper_char(c);
                    else
                        c = shifted_US[scancode];
                }

                if (scancode != LSHIFT) {
                    kprintf("%c", c);

                    if (input_mover == 0)
				        append(input_buffer, c); // thats the simple way, but we can do it the HARD way
                    // input_buffer[input_buffer_index] = c;   // YEAAAHHH! THE HARD WAAAYYYYYY
                    // input_buffer[input_buffer_index + 1] = '\0';
                    // input_buffer_index++;
                    else {
                        input_buffer[strlen(input_buffer) - input_mover] = c;
                        invert_char_colors(get_cursor().x, get_cursor().y);
                    }
                }
                iowait();
                print_cursor(true);
                break;
        }
	}
	

	return regs;
}

char getch(u8 scancode) {
	return ascii_US[scancode];
}

void clear_input_line() {
    if (strlen(input_buffer) <= 0) return;
    for (size_t i = 0; i < strlen(input_buffer); i++)
        print_backspace();
    print_char_at(0, 1, last_line_start, WHITE); // TODO: remove hardcoded cursor removing
    set_cursor(2, last_line_start);
    strclr(input_buffer);
}

void keyboard_init() {
    kprintf("[KB Driver] Installing keyboard handler at %x to IRQ1...\n", &keyboard_handler);
	IRQ_installHandler(1, keyboard_handler);
    prev_input = (char*) malloc(sizeof(char) * MAX_BUFFER);
    set_cursor(2, 2);   // TODO: Hardcoded Cursor Pos Setting
    last_line_start = get_cursor().y;
}

void user_input(char* input) {
    handle_command(input);
    last_line_start = get_cursor().y;
}