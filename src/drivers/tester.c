#include "tester.h"
#include <lib/print.h>
#include <lib/string.h>
#include <lib/mem.h>

#define TEST_COUNT	4


void test_keyboard() {
}

void test_display() {
}

void test_alloc() {
}

void test_interrupts() {
}

bool test_memory() {
	bool result;

	kprintf("\nStart of Memory test...\n");

	/*for (size_t i = 0; i < 1; i++) {
		size_t allocation_size = 8 * (i + 1);
		kprintf("Allocating %d bytes\n", allocation_size);
		result = (umm_malloc(allocation_size)) != NULL;
		umm_free(allocation_size);
		if (!result) return false;
	}*/
	if (result) kprintf("Memory test successful!\n");
	return result;
}

bool run_tests() {
	test_memory();

	return true;
}