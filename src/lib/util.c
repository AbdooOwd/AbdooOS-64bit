#include "util.h"
#include "../include/types.h"

void array_copy(unsigned char* source, unsigned char* dest) {
    for (size_t i = 0; i < sizeof(source); i++) {
        dest[i] = source[i];
    }
}

char* int_to_str(int num) {
    static char buffer[69]; // Static buffer to hold the string

    // Handle negative numbers
    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Convert digits to characters in reverse order
    int index = 0;
    do {
        buffer[index++] = '0' + num % 10;
        num /= 10;
    } while (num > 0);

    // Add negative sign if necessary
    if (is_negative) {
        buffer[index++] = '-';
    }

    // Null-terminate the string
    buffer[index] = '\0';

    // Reverse the string
    int i = 0, j = index - 1;
    while (i < j) {
        char temp = buffer[i];
        buffer[i++] = buffer[j];
        buffer[j--] = temp;
    }

    return buffer;
}