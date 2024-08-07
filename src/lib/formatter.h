#ifndef FORMATTER_H
#define FORMATTER_H

#pragma once

#include <include/types.h>
#include <stdarg.h>

#define MAX_STRING_FORMATTER_BUFFER_SIZE 1024
#define MAX_HEX_HANDLER_BUFFER_SIZE 64

void int_to_ascii(int n, char* str);
void hex_to_ascii(int n, char* str);
int ascii_to_int(char *str);
int format_handler(char format, char buffer[], int buffer_offset, va_list args);

#endif