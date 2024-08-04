#ifndef SHELL_H
#define SHELL_H

#pragma once

#include <include/types.h>

typedef struct {
	char* command;
	char* description;
} command_t;

void handle_command(char* command);

#endif