#ifndef KEYBOARD_H
#define KEYBOARD_H
#pragma once

#include <include/types.h>
#include <interrupts/idt.h>


typedef struct {
	char* command;
	char* description;
} command_t;


InterruptRegisters* keyboard_handler(InterruptRegisters* regs);
void keyboard_init();
char getch(u8 scancode);

void user_input(char* input);

#endif