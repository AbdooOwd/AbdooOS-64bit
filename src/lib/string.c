#include "string.h"
#include "util/util.h"
#include "mem.h"
#include <mm/pmm.h>


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

void strlcpy(char* dest, char* src, size_t size) {
    if (strlen(src) > size || strlen(dest) > size)
        return;

    while (*src) {
        *dest++ = *src++;
    }
    dest[size - 1] = '\0';
}

int strcmp(char* s1, char* s2) {
    size_t i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

bool strsame(char* str1, char* str2) {
    return strcmp(str1, str2) == 0;
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

size_t count(char* str, char target) {
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

void strclr(char* str) {
	backspaces(str, strlen(str));
}

char upper_char(char c) {
    return char_islower(c) ? (c - 'a' + 'A') : c;
}

#include <lib/print.h>


// // TODO: Optimize this
// void split(char* str, char target, char** splitten) {
//     /**
//      * 1. calculate needed allocation
//      * 2. then allocate and get the split
//      */
// 
//     size_t split_i = 0;
//     size_t len = strlen(str);
//     size_t temp_len = 0;
// 
//     for (size_t i = 0; i < len; i++) {
//         char c = str[i];
// 
//         if (c == target) {
//             append((char*) ((u64) splitten + temp_len), '\0');
//             temp_len++;
//             split_i++;
//             continue;
//         }
//         append((char*) ((u64) splitten + temp_len), c); // get address of next char
//         temp_len++;
//     }
// }

char* get_split(char* str, char target, size_t index) {
    size_t len = strlen(str);
    size_t target_found = 0;
    char* result = (char*) malloc(sizeof(char) * 16);
    size_t str_i = 0;

    for (size_t i = 0; i < len; i++) {
        char c = str[i];

        if (target_found == index && c != target) {
            result[str_i] = c;
            str_i++;
        }

        if (c == target || str[i + 1] == 0) {
            target_found++;

            if (target_found == index + 1) {
                result[str_i + 1] = '\0';
                return result;
            }
            continue;
        }
    }

    return "NaN";
}

char** gpt_split(char* str, char target) {
    size_t target_count = count(str, target);
	
    char** splitten = (char**)malloc((target_count + 2) * sizeof(char*));

    if (!splitten) {
        return NULL;
    }
    
    size_t split_i = 0;
    size_t start = 0;
    size_t i = 0;
    
    // Iterate through the string
    while (str[i] != '\0') {
        if (str[i] == target) {
            size_t part_len = i - start;
            splitten[split_i] = (char*)malloc((part_len + 1) * sizeof(char));
            if (!splitten[split_i]) {
                // Handle allocation failure
                for (size_t j = 0; j < split_i; j++) {
                    mfree(splitten[j]);
                }
                mfree(splitten);
                return NULL;
            }
            strlcpy(splitten[split_i], &str[start], part_len);
            splitten[split_i][part_len] = '\0';
            split_i++;
            start = i + 1;
        }
        i++;
    }
    
    // Handle the last part
    size_t part_len = i - start;
    splitten[split_i] = (char*)malloc((part_len + 1) * sizeof(char));
    if (!splitten[split_i]) {
        // Handle allocation failure
        for (size_t j = 0; j < split_i; j++) {
            mfree(splitten[j]);
        }
        mfree(splitten);
        return NULL;
    }
    strlcpy(splitten[split_i], &str[start], part_len);
    splitten[split_i][part_len] = '\0';
    split_i++;
    
    // Mark the end of the array
    splitten[split_i] = NULL;
    
    return splitten;
}