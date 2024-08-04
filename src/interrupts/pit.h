#ifndef PIT_H
#define PIT_H

#pragma once

#include <include/types.h>
#include "idt.h"

extern u64 ticks;
extern u64 seconds;

void PIT_init();
InterruptRegisters* PIT_handler(InterruptRegisters* regs);

#endif