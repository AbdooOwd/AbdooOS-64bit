#ifndef UTIL_H
#define UTIL_H

typedef struct {
    int x;
    int y;
} __attribute__((packed)) Vector2;

void array_copy(unsigned char* source, unsigned char* dest);
char* int_to_str(int num);

#endif