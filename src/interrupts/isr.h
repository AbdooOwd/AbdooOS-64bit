#ifndef ISR_H
#define ISR_H
#pragma once

#include <include/types.h>
#include "idt.h"

InterruptRegisters* ISR_handler(InterruptRegisters* regs);

#endif