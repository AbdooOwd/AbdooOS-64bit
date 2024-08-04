#ifndef KERNEL_H
#define KERNEL_H

#pragma once

#include <include/types.h>

extern volatile struct limine_bootloader_info_request bootloader_info;

void kernel_init();
void kernel_main();

#endif