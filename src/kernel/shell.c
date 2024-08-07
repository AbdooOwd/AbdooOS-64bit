#include "shell.h"
#include <lib/string.h>
#include <lib/print.h>
#include <lib/formatter.h>
#include <include/globals.h>
#include <drivers/screen.h>
#include <drivers/tester.h>
#include <cpu/cpu.h>
#include <interrupts/pit.h>
#include <mm/pmm.h>
#include <kernel/kernel.h>
#include <programs/cowsay.h>

command_t commands[] = {
    { "help",               "Helps, duh?" },
    { "about",              "Displays information about the OS."},
    { "chngfont",           "Changes the font."},
    { "test | runtests",    "Run tests on the machine and OS."},
    { "info",               "Displays info about the system."},
    { "clear",              "Clears the screen."},
    { "random",             "Displays a random integer."},
    { "echo",               "Prints what's between double quotes in the first argument."},
    { "sysfetch",           "Like Linux's 'neofetch'."},
    { "crashme",            "Crashes the system by trying to divide by zero (supposed to be harmless)."},
    { "exit",               "Halts the CPU, resulting in stopping all of its processing."}
};

void handle_command(char* command) {
    if (strlen(command) <= 0) {
        print_color("$ ", GREEN);
        return;
    }

    command = trim(command);

    char* full_command = malloc(sizeof(char) * MAX_BUFFER);
    strcpy(full_command, command);
    
    command = get_split(command, ' ', 0);
    lower(command);

    size_t argc = count(full_command, ' ') + 1;
    // TODO: if user adds useless spaces at end/start of string, they are counted anyways

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
        if (argc < 2) kprintf("%i\n", random());
        else kprintf(
            "%i\n", 
            rand_range(
                ascii_to_int(get_split(full_command, ' ', 1)), 
                ascii_to_int(get_split(full_command, ' ', 2))
            )
        );
    }

    if (strsame(command, "echo")) {
        if (argc >= 2) {
            char* text = get_argStr(full_command, 0);
            kprintf("%s\n", text);
        }
    }

    if (strsame(command, "sysfetch")) {
        // TODO: Make this simpler
        print_string_at(
            " #### SYSFETCH ####\n", 
            SYSFETCH_X_POS, get_cursor().y + 1, RED
        );

        print_string_at(
            "- OS: AbdooOS\n- Bootloader: ", 
            SYSFETCH_X_POS + 2, get_cursor().y, WHITE
        );

        kprintf("%s", bootloader_info.response->name);
        print_string_at(
            "- Dev's Comment: ", 
            SYSFETCH_X_POS + 2, get_cursor().y + 1, WHITE
        );
        print_color("\"Enjoy my piece of work!\"\n\n", GREEN);
    }

    if (strsame(command, "cowsay")) {
        if (argc >= 2)
            cowsay(get_argStr(full_command, 0));
    }

    if (strsame(command, "crashme")) {
        crash_me();
    }

    if (strsame(command, "exit")) {
        kprintf("Halting CPU\n");
        halt();
    }

    if (strsame(command, "dev")) {
        char* teso = "    YOU E4T MY BARBER    ";
        kprintf("OG String: \"%s\"\n", teso);
        teso = trim(teso);
        kprintf("Trimmed String: \"%s\"\n", teso);
    }

    print_color("$ ", GREEN);
}