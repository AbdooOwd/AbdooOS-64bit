/**
 * Thanks to NanoByte for the code.
 * I don't quite understand PICs well yet...
 * 
 * The code has been "borrowed" from NanoByteOS' 
 * GitHub repo [here](https://github.com/nanobyte-dev/nanobyte_os/blob/videos/part10/src/kernel/arch/i686/pic.c)
*/

#include "pic.h"
#include <kernel/io.h>

#define PIC1_COMMAND_PORT	0x20
#define PIC1_DATA_PORT		0x21
#define PIC2_COMMAND_PORT	0xA0
#define PIC2_DATA_PORT		0xA1

enum {
    PIC_ICW1_ICW4           = 0x01,
    PIC_ICW1_SINGLE         = 0x02,
    PIC_ICW1_INTERVAL4      = 0x04,
    PIC_ICW1_LEVEL          = 0x08,
    PIC_ICW1_INITIALIZE     = 0x10
} PIC_ICW1;

enum {
    PIC_ICW4_8086           = 0x1,
    PIC_ICW4_AUTO_EOI       = 0x2,
    PIC_ICW4_BUFFER_MASTER  = 0x4,
    PIC_ICW4_BUFFER_SLAVE   = 0x0,
    PIC_ICW4_BUFFERRED      = 0x8,
    PIC_ICW4_SFNM           = 0x10,
} PIC_ICW4;


enum {
    PIC_CMD_END_OF_INTERRUPT    = 0x20,
    PIC_CMD_READ_IRR            = 0x0A,
    PIC_CMD_READ_ISR            = 0x0B,
} PIC_CMD;


void PIC_config(u8 offsetPic1, u8 offsetPic2) {
	// Let's Initialize the PICs! (initialization control word 1)
	outb(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
	iowait();
	outb(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
	iowait();

	// continue initialization (icw2, offsets) 
	outb(PIC1_DATA_PORT, offsetPic1);
	iowait();
	outb(PIC2_DATA_PORT, offsetPic2);
	iowait();

	// icw3 (tell PIC1 it is a master at IRQ2 (4th pin))
	outb(PIC1_DATA_PORT, 0x4);
	iowait();
	//		(tell PIC2 it is a slave)
	outb(PIC2_DATA_PORT, 0x2);
	iowait();

	// icw4 (tell PIC we're in 8086)
	outb(PIC1_COMMAND_PORT, PIC_ICW4_8086);
	iowait();
	outb(PIC2_COMMAND_PORT, PIC_ICW4_8086);
	iowait();

	// clear data registers
	outb(PIC1_DATA_PORT, 0);
	iowait();
	outb(PIC2_DATA_PORT, 0);
	iowait();
}

void PIC_eoi(int irq) {
	if (irq >= 8)
		outb(PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
	outb(PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
}

void PIC_disable() {
	outb(PIC1_DATA_PORT, 0xFF);	// MASK THEM ALL
	iowait();
	outb(PIC2_DATA_PORT, 0xFF);
	iowait();
}

void PIC_mask(int irq) {
	u8 port;

	if (irq < 7)
		port = PIC1_DATA_PORT;
	else {
		irq -= 8;
		port = PIC2_DATA_PORT;
	}

	u8 mask = inb(PIC1_DATA_PORT);
	outb(PIC1_DATA_PORT, mask | (1 << irq));
}

void PIC_unmask(int irq) {
	u8 port;

	if (irq < 7)
		port = PIC1_DATA_PORT;
	else {
		irq -= 8;
		port = PIC2_DATA_PORT;
	}

	u8 mask = inb(PIC1_DATA_PORT);
	outb(PIC1_DATA_PORT, mask & ~(1 << irq));
}

u16 PIC_readIRQRequestReg() {
	outb(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
	outb(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);

	return ( (u16) inb(PIC2_COMMAND_PORT)) | (( (u16) inb(PIC2_COMMAND_PORT)) << 8);
}

u16 PIC_readServiceReg() {
	outb(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
	outb(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);

	return ( (u16) inb(PIC2_COMMAND_PORT)) | (( (u16) inb(PIC2_COMMAND_PORT)) << 8);
}