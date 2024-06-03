#pragma once

#include <include/types.h>
#include <cpu/cpu.h>


execution_context* ISR_handler(execution_context* regs);
void ISR_initGates();