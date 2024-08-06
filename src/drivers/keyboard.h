#ifndef KEYBOARD_H
#define KEYBOARD_H
#pragma once

#include <include/types.h>
#include <interrupts/idt.h>



InterruptRegisters* keyboard_handler(InterruptRegisters* regs);
void keyboard_init();
char getch(u8 scancode);

void clear_input_line();
void user_input(char* input);

#endif