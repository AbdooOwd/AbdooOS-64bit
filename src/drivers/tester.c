#include "tester.h"
#include <lib/print.h>
#include <lib/string.h>
#include <lib/mem.h>
#include <mm/bm_alloc.h>
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
	char* test_filename = "minecraft.txt";
        
    kprintf("Creating IMFS...\n");
    IMFS* imfs = IMFS_create();

    if (imfs == NULL) {
        kprintf("Failed to create IMFS\n");
        return -1;
    }

    kprintf("Created IMFS at %x with size of %x\n", &imfs, sizeof(IMFS));

    if (IMFS_file_create(imfs, test_filename) == 0)
        kprintf("Creating file '%s'\n", test_filename);
    else {
        kprintf("Failed to create '%s'\n", test_filename);
        return -2;
    }

    if (IMFS_file_write(imfs, test_filename, "COCO MARIO HAHA DIAMONDS") == 0)
        kprintf("Wrote 'COCO MARIO HAHA DIAMONDS' to '%s'\n", test_filename);
    else {
        kprintf("Failed to write to '%s'\n", test_filename);
        return -3;
    }

    char le_buffer[MAX_FILENAME_LENGTH];
    if (IMFS_file_read(imfs, test_filename, le_buffer) == 0) {
        kprintf("Read '%s': \"%s\"\n", test_filename, le_buffer);
    } else {
        kprintf("Failed to read '%s'\n", test_filename);
        return -4;
    }

    if (IMFS_file_delete(imfs, test_filename) == 0) 
        kprintf("Deleted '%s'\n", test_filename);
    else {
        kprintf("Failed to deleted '%s'\n", test_filename);
		return -5;
	}

    kprintf("> IMFS Test: Successful!\n");

    free(imfs);

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