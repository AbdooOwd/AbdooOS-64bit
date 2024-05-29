#include "formatter.h"
#include "string.h"

void int_to_ascii(int n, char* str) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(int n, char* str) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    i32 tmp;
    size_t i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}



void int_to_hex(u64 i, char* buffer) {
    if (i == 0) {
        buffer[0] = '0';
        return;
    }
    char     hex_chars[16] = "0123456789abcdef";
    u64 num = i;
    u8  rem = 0;

    u64 index = 0;

    while (num != 0 && index < MAX_HEX_HANDLER_BUFFER_SIZE) {
        rem = num % 16;
        num /= 16;
        buffer[index] = hex_chars[rem];
        index++;
    }

    u32 k = 0;
    u32 j = index - 1;
    while (k < j) {
        char temp = buffer[k];
        buffer[k] = buffer[j];
        buffer[j] = temp;
        k++;
        j--;
    }

    return;
}

int format_string(char* s, char buffer[], int buffer_offset) {
    while (*s && buffer_offset < MAX_STRING_FORMATTER_BUFFER_SIZE) {
        buffer[buffer_offset] = *s;
        buffer_offset++;
        s++;
    }
    return buffer_offset;
}

int format_char(int c, char buffer[], int buffer_offset) {
    if (buffer_offset < MAX_STRING_FORMATTER_BUFFER_SIZE) {
        buffer[buffer_offset] = c;
        buffer_offset++;
    }
    return buffer_offset;
}

int format_uint(u64 uint, char buffer[], int buffer_offset) {
    if (uint == 0) {
        if (buffer_offset < MAX_STRING_FORMATTER_BUFFER_SIZE) {
            buffer[buffer_offset] = '0';
            buffer_offset++;
        }
        return buffer_offset;
    }
    u32 len = 0;
    u64 remainder = uint;
    while (remainder != 0) {
        len++;
        remainder /= 10;
    }

    char number[len];

    for (u32 i = 0; i < len; i++) {
        number[i] = (uint % 10) + '0';
        uint /= 10;
    }

    for (int i = len - 1; i >= 0; i--) {
        if (buffer_offset < MAX_STRING_FORMATTER_BUFFER_SIZE) {
            buffer[buffer_offset] = number[i];
            buffer_offset++;
        } else {
            break;
        }
    }

    return buffer_offset;
}

int format_int(i64 integer, char buffer[], int buffer_offset) {
    if (integer == 0) {
        if (buffer_offset < MAX_STRING_FORMATTER_BUFFER_SIZE) {
            buffer[buffer_offset] = '0';
            buffer_offset++;
        }
        return buffer_offset;
    }
    u32 len = 0;
    i64  remainder = integer;
    while (remainder != 0) {
        len++;
        remainder /= 10;
    }

    char number[len];

    for (u32 i = 0; i < len; i++) {
        number[i] = (integer % 10) + '0';
        integer /= 10;
    }

    for (int i = len - 1; i >= 0; i--) {
        if (buffer_offset < MAX_STRING_FORMATTER_BUFFER_SIZE) {
            buffer[buffer_offset] = number[i];
            buffer_offset++;
        } else {
            break;
        }
    }

    return buffer_offset;
}

int format_hex(u64 hex_int, char buffer[], int buffer_offset) {
    char hex_string[MAX_HEX_HANDLER_BUFFER_SIZE] = {0};
    int_to_hex(hex_int, hex_string);
    return format_string(hex_string, buffer, buffer_offset);
}


int format_handler(char format, char buffer[], int buffer_offset,
                   va_list args) {
    switch (format) {
    // digit/int
    case 'd':
    case 'i':
        i64 int_arg = va_arg(args, i64);
        buffer_offset = format_int(int_arg, buffer, buffer_offset);
        break;

    // unsigned int
    case 'u':
        u64 uint_arg = va_arg(args, u64);
        buffer_offset = format_uint(uint_arg, buffer, buffer_offset);
        break;

    // unsigned octal
    case 'o':
        break;

    // unsigned hexadecimal
    case 'X':
    // parse to lowercase
    case 'p':
    case 'x':
        if (buffer_offset + 2 < MAX_STRING_FORMATTER_BUFFER_SIZE) {
            buffer[buffer_offset] = '0';
            buffer[buffer_offset + 1] = 'x';
            buffer_offset += 2;
            u64 hex_arg = va_arg(args, u64);
            buffer_offset = format_hex(hex_arg, buffer, buffer_offset);
        }
        break;

    // decimal floating point
    case 'F':
    case 'f':
        break;

    // scietific notation
    case 'E':
    case 'e':
        break;

    // I don't even know
    case 'G':
    case 'g':
        break;

    // hexadecimal floating point
    case 'A':
    case 'a':
        break;

    // char
    case 'c':
        int arg_char = va_arg(args, int);
        buffer_offset = format_char(arg_char, buffer, buffer_offset);
        break;

    // string
    case 's':
        char* arg_str = va_arg(args, char*);
        buffer_offset = format_string(arg_str, buffer, buffer_offset);
        break;
    default:
        break;
    }
    return buffer_offset;
}
