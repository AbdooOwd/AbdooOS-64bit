#ifndef UTIL_H
#define UTIL_H

#pragma once

typedef struct {
    int x;
    int y;
} __attribute__((packed)) Vector2;

void array_copy(unsigned char* source, unsigned char* dest);
char* int_to_str(int num);


#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

#endif