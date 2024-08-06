#ifndef COWSAY_H
#define COWSAY_H

#pragma once

#include <include/types.h>
#include <lib/print.h>

static void cowsay(char* the_text){
    kprintf("\n    < ");
    kprintf("%s", the_text);
    kprintf(" > \n");
    kprintf("        \\   ^__^\n");
    kprintf("         \\  (oo)\\_______\n");
    kprintf("            (__)\\       )\\/\\ \n");
    kprintf("               ||----W | \n");
    kprintf("               ||     || \n");
}

#endif