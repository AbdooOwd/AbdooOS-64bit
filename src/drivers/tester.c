#include "tester.h"
#include <lib/print.h>
#include <lib/string.h>
#include <lib/mem.h>
#include <mm/alloc.h>
#include <fs/imfs.h>
#include <fs/vfs.h>


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
    kprintf("] IMFS TEST\n");

	char* test_filename = "minecraft.txt";
        
    kprintf("Creating IMFS...\n");
    IMFS_create();

    if (imfs == NULL) {
        kprintf("Failed to create IMFS\n");
        return -1;
    }

    kprintf("Created IMFS at %x with size of %x\n", &imfs, sizeof(IMFS));

    if (IMFS_file_create(test_filename) == 0)
        kprintf("Creating file '%s'\n", test_filename);
    else {
        kprintf("Failed to create '%s'\n", test_filename);
        return -2;
    }

    char* msg = "COCO MARIO HAHA DIAMONDS";

    if (IMFS_file_write(test_filename, msg, sizeof(char) * strlen(msg)) == 0)
        kprintf("Wrote '%s' to '%s'\n", msg, test_filename);
    else {
        kprintf("Failed to write to '%s'\n", test_filename);
        return -3;
    }

    char le_buffer[MAX_FILENAME_LENGTH];
    if (IMFS_file_read(test_filename, le_buffer, sizeof(char) * strlen(msg)) == 0) {
        kprintf("Read '%s': \"%s\"\n", test_filename, le_buffer);
    } else {
        kprintf("Failed to read '%s'\n", test_filename);
        return -4;
    }

    if (IMFS_file_delete(test_filename) == 0) 
        kprintf("Deleted '%s'\n", test_filename);
    else {
        kprintf("Failed to deleted '%s'\n", test_filename);
		return -5;
    }

    if (mfree(imfs) == 0) kprintf("Free'd IMFS from Memory\n");
    else {
        kprintf("Failed to free IMFS from Memory\n");
        return -6;
    }

    kprintf("> IMFS Test Successful!\n");

    return 0;
}

int test_vfs() {

    kprintf("] VFS TEST\n");

    kprintf("Initializing VFS\n");
    vfs_init();

    char* ff = "idiot.pp";
    char mnt_name[64];
    
    strcpy(mnt_name, vfs->mountpoints[0]->name);

    if (vfs_create(mnt_name, ff) == 0)
        kprintf("Created '%s'\n", ff);
    else {
        kprintf("Failed to create '%s'\n", ff);
        return -1;
    }

    char* msg = "Hello From VFS!";

    if (vfs_write(mnt_name, ff, msg, sizeof(char) * strlen(msg)) == 0)
        kprintf("Wrote '%s' to '%s'\n", msg, ff);
    else {
        kprintf("Failed to write '%s' to '%s'\n", msg, ff);
        return -2;
    }

    char* buf = malloc(sizeof(char) * 64);

    if (vfs_read(mnt_name, ff, buf, sizeof(char) * 64) == 0) {
        kprintf("Successful read from '%s'\n", ff);
        kprintf("Data: '%s'\n", buf);
    } else {
        kprintf("Failed to read from '%s'\n", ff);
        return -3;
    }

    kprintf("Cleaning up VFS...\n");
    clean_up();

    kprintf("> VFS Test Success!\n");

    return 0;
}

bool run_tests() {
    test_imfs();
    kprintf("\n\n");
    test_vfs();

	return true;
}