#ifndef STRING_H
#define STRING_H

#pragma once

#include <include/types.h>


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

int count(char* str, char target);
void lower(char* str);
void upper(char* str);
bool char_islower(char c);
char upper_char(char c);

#endif