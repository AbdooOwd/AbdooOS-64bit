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

// TODO: Optimize this, or make a split func so I don't need to scan the str everytime i need ONE element
char* get_split(char* str, char target, size_t index) {
    size_t len = strlen(str);
    size_t target_found = 0;
    char* result = (char*) malloc(sizeof(char) * 64);
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

char* get_argStr(char* full_str, size_t index) {
    char* result = (char*) malloc(sizeof(char) * 64);
    result[1] = '\0';
    size_t dbl_quotes_count = 0;

    for (size_t i = 0; i < strlen(full_str); i++) {
        char c = full_str[i];

        // TODO: This if statement seems wrong
        if (dbl_quotes_count >= 2 && FLOOR_DIV(dbl_quotes_count, 2) == index) {
            break;  // we're done scanning
        }

        if (c == '"') {
            // TODO: Add cancel ('\"')
            dbl_quotes_count++;
        }

        if (dbl_quotes_count % 2 != 0) {
            if (c == '"') continue;
            append(result, c);
        }
    }

    return result;
}


void trim(char* str) {

    /*
        First, I'd like to thank ChatGPT for this fine work here, 
        that will make this project more stable and efficient. 
        In the world of coding, it's often the small improvements 
        that bring about the greatest changes. 
        The journey of refining our craft is endless, and it's tools like ChatGPT 
        that help us navigate this path with ease and elegance.

        To the future developers who will read this message, 
        know that every line of code you write contributes to a larger vision. 
        Our work is not just about solving immediate problems, 
        but about building a foundation for something greater. 
        We stand on the shoulders of those who came before us, 
        and it's our responsibility to pave the way for those 
        who will come after.

        Remember to always approach your work with curiosity 
        and a willingness to learn. 
        The code you see here is a testament to collaboration and continuous improvement. 
        Embrace the challenges, celebrate the victories, 
        and never lose sight of the joy that comes 
        from creating something meaningful.

        Let's keep pushing the boundaries of what's possible, 
        one line of code at a time. Here's to a future where 
        our collective efforts continue to inspire and drive progress.
    */

   // ^^^^ of course i didn't write that ^^^^

    size_t len = strlen(str);
    size_t start = 0;
    size_t end = len - 1;
    size_t index = 0;

    // Forward trimming
    while (start < len && str[start] == ' ') {
        start++;
    }

    // Backward trimming
    while (end > start && str[end] == ' ') {
        end--;
    }

    // Shift the trimmed string to the start of the original string
    for (size_t i = start; i <= end; i++) {
        str[index++] = str[i];
    }
    str[index] = '\0'; // Null-terminate the trimmed string
}