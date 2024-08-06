#include "shell.h"
#include <lib/string.h>
#include <lib/print.h>
#include <include/globals.h>
#include <drivers/screen.h>
#include <drivers/tester.h>
#include <cpu/cpu.h>
#include <mm/pmm.h>

command_t commands[] = {
    { "help", "Helps, duh?" },
    { "about", "Displays information about the OS."},
    { "chngfont", "Changes the font."},
    { "test | runtests", "Run tests on the machine and OS."},
    { "info", "Displays info about the system."},
    { "clear", "Clears the screen."},
    { "random", "Displays a random integer."},
    { "crashme", "Crashes the system by trying to divide by zero (supposed to be harmless)."},
    { "exit", "Halts the CPU, resulting in stopping all of its processing."}
};

void handle_command(char* command) {
	if (strsame(command, "help"))
        for (size_t cmd = 0; cmd < sizeof(commands) / sizeof(command_t); cmd++)
            kprintf(" - %s: %s\n", commands[cmd].command, commands[cmd].description);

    if (strsame(command, "about")) {
        kprintf(
            "AbdooOS is a hobby x86-64 operating system made for fun "
            "with the goal of learning about computer science "
            "both hardware and software.\n"
        );
    }

    if (strsame(command, "chngfont")) {
        use_altFont = !use_altFont;

        int old_font_height = font_dimensions.y;

        if (use_altFont) {
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

    if (strsame(command, "runtests") || strsame(command, "test")) {
        run_tests();
    }

    if (strsame(command, "info")) {
        kprintf("> MEMORY INFO\n");
        // kprintf(" - Usable Memory Size: %x\n", mem_size);
    }

    if (strsame(command, "clear")) {
        clear_screen();
        print_entry();
    }

    if (strsame(command, "random")) {
        kprintf("%i\n", random());
    }

    if (strsame(command, "crashme")) {
        crash_me();
    }

    if (strsame(command, "exit")) {
        kprintf("Halting CPU\n");
        halt();
    }

    if (strsame(command, "dev")) {
        char* test = "Abdoo Likes Women";
        kprintf("Testing Split with string \"%s\" with ' '\n", test);
        char** x = split(test, ' ');
        for (size_t i = 0; i < count(test, ' ') + 1; i++) {
            kprintf("%s\n", x[i]);
        }
    }

    print_color("$ ", GREEN);
}