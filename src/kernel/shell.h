#ifndef SHELL_H
#define SHELL_H

#pragma once

#include <include/types.h>

#define MAX_BUFFER			256
#define	SYSFETCH_X_POS		4

typedef struct {
	char* command;
	char* description;
} command_t;

void handle_command(char* command);

#endif