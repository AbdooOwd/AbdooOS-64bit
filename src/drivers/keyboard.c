#include "keyboard.h"
#include "screen.h"
#include "tester.h"
#include <interrupts/irq.h>
#include <include/functions.h>
#include <include/globals.h>
#include <lib/print.h>
#include <lib/string.h>
#include <kernel/io.h>
#include <cpu/pic.h>
#include <cpu/cpu.h>


#define KBD_DATA_PORT 	0x60
#define KBD_STATUS_PORT KBD_DATA_PORT + 4	// 0x64

#define BACKSPACE 		0x0E
#define ENTER 			0x1C
#define LSHIFT 			0x2A

#define SC_MAX 			57

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

char ascii_US[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' ' };

command_t commands[] = {
    { "help", "Helps, duh?" },
    { "about", "Displays information about the OS."},
    { "chngfont", "Changes the font."},
    { "exit", "Halts the CPU, resulting in stopping all of its processing."}
};

static char input_buffer[256];
static bool shift_pressed;


InterruptRegisters* keyboard_handler(InterruptRegisters* regs) {
	u8 scancode = inb(KBD_DATA_PORT);

    // if we press an unhandled 
    if (scancode > SC_MAX && !(scancode == LSHIFT || scancode == (LSHIFT | 0x80))) return;


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
                print("\n");
                user_input(input_buffer);
                input_buffer[0] = '\0';
                break;
            
            case LSHIFT | 0x80:     // we released shift
                shift_pressed = false;
                break;

			default:
                char c = getch(scancode);
                if (shift_pressed) c = upper_char(c);
                if (scancode != LSHIFT) {
				    append(input_buffer, c);
				    kprintf("%c", c);
                }
        }
	}
	

	return regs;
}

char getch(u8 scancode) {
	return ascii_US[scancode];
}

void keyboard_init() {
	IRQ_installHandler(1, keyboard_handler);
}

void user_input(char* input) {

    lower(input);

    if (strsame(input, "help"))
        for (size_t cmd = 0; cmd < sizeof(commands) / sizeof(command_t); cmd++)
            kprintf(" - %s: %s\n", commands[cmd].command, commands[cmd].description);

    if (strsame(input, "about")) {
        kprintf(
            "AbdooOS is a hobby x86-64 operating system made for fun "
            "with the goal of learning about computer science "
            "both hardware and software.\n"
        );
    }

    if (strsame(input, "chngfont")) {
        use_moon_font = !use_moon_font;

        int old_font_height = font_dimensions.y;

        if (use_moon_font) {
            font_dimensions.x = 8;
            font_dimensions.y = 16;
        } else {
            font_dimensions.x = 8;
            font_dimensions.y = 8;
        }

        // Update cursor position based on new font height

        set_cursor(
            get_cursor().x,
            (get_cursor().y * old_font_height + font_dimensions.y) / font_dimensions.y
        );

        kprintf("Changed font successfullly!\n");
    }

    if (strsame(input, "runtests")) {
        run_tests();
    }

    if (strsame(input, "exit")) {
        kprintf("Halting CPU\n");
        halt();
    }

    print("$ ");
}