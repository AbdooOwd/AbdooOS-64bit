#include "tester.h"
#include <lib/print.h>
#include <lib/string.h>
#include <lib/mem.h>
#include "imfs.h"


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

int test_imfs() {
	char* ff = "minecraft.txt";
        
    kprintf("Creating IMFS...\n");
    IMFS* imfs = IMFS_create();

    if (imfs == NULL) {
        kprintf("Failed to create IMFS\n");
        return -1;
    }

    kprintf("Created IMFS at %x with size of %x\n", &imfs, sizeof(IMFS));

    if (IMFS_file_create(imfs, ff) == 0)
        kprintf("Creating file '%s'\n", ff);
    else {
        kprintf("Failed to create '%s'\n", ff);
        return -2;
    }

    if (IMFS_file_write(imfs, ff, "COCO MARIO HAHA DIAMONDS") == 0)
        kprintf("Wrote 'COCO MARIO HAHA DIAMONDS' to '%s'\n", ff);
    else {
        kprintf("Failed to write to '%s'\n", ff);
        return -3;
    }

    char le_buffer[MAX_FILENAME_LENGTH];
    if (IMFS_file_read(imfs, ff, le_buffer) == 0) {
        kprintf("Read '%s':\n", ff);
        kprintf("%s\n", le_buffer);
    } else {
        kprintf("Failed to read '%s'\n", ff);
        return -4;
    }

    if (IMFS_file_delete(imfs, ff) == 0) 
        kprintf("Deleted '%s'\n", ff);
    else {
        kprintf("Failed to deleted '%s'\n", ff);
		return -5;
	}

    return 0;
}

bool run_tests() {
    kprintf("Running IMFS tests\n\n");
    test_imfs();
    kprintf("\n\n");
    kprintf("Running Memory tests\n");
	test_memory();

	return true;
}