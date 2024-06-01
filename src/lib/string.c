#include "string.h"
#include "util/util.h"


void reverse(char* str) {
    size_t c, i, j;
    for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

size_t strlen(char* str) {
    size_t i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

void append(char* str, char c) {
    size_t len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

void backspace(char* str) {
    size_t len = strlen(str);
    str[len - 1] = '\0';
}

void backspaces(char* str, int times) {
    for (size_t i = 0; i < (size_t) times; i++) {
        backspace(str);
    }
}

void strcpy(char* dest, char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int strcmp(char* s1, char* s2) {
    size_t i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void merge_strings(char* str1, char* str2, char* result) {
    strcpy(result, str1);
    while (*str2) 
        append(result, *str2++);
}

bool has_char(char* str, char target) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == target) return true;
    }
    return false;
}

int count(char* str, char target) {
    size_t le_count = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == target) le_count++;
    }
    return le_count;
}

void lower(char* str) {
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 0x20;
}

void upper(char* str) {
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 0x20;
}

bool char_islower(char c) {
    return c >= 'a' && c <= 'z';
}

char upper_char(char c) {
    return char_islower(c) ? (c - 'a' + 'A') : c;
}