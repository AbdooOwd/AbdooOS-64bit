#ifndef LOW_LEVEL_H
#define LOW_LEVEL_H

#pragma once

#include <include/types.h>

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
unsigned short inw(unsigned short port);
void outw(unsigned short port, unsigned short data);

static inline void outb(u16 port, u8 val);
static inline u8 inb(u16 port);

#endif