#ifndef STRING_H
#define STRING_H

#pragma once

#include <include/types.h>

#define DEFAULT_MAX_STRING_LENGTH 	128
#define NORMAL_STRING_LENGTH		32
#define NORMAL_SPLIT_ARRAY_SIZE		8


void int_to_ascii(int n, char* str);
void hex_to_ascii(int n, char* str);

void reverse(char* s);
size_t strlen(char* str);

void backspace(char* s);
void backspaces(char* str, int times);

void strcpy(char* dest, char* src);
void strlcpy(char* dest, char* src, size_t size);
char* strcat(char* str1, char* str2);
int strcmp(char* s1, char* s2);
bool strsame(char* str1, char* str2);
void merge_strings(char* str1, char* str2, char* result);
void append(char* str, char c);

void strclr(char* str);

size_t count(char* str, char target);
void lower(char* str);
void upper(char* str);
bool char_islower(char c);
char upper_char(char c);

// char** split(char* str, char target);
char* get_split(char* str, char target, size_t index);
char* get_argStr(char* full_str, size_t index);

#endif