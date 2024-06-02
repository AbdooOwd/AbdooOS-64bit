#ifndef PIC_H
#define PIC_H
#pragma once

#include <include/types.h>

void PIC_config(u8 offsetPic1, u8 offsetPic2);
void PIC_eoi(int irq);
void PIC_disable();
void PIC_mask(int irq);
void PIC_unmask(int irq);
u16 PIC_readIRQRequestReg();
u16 PIC_readServiceReg();

#endif