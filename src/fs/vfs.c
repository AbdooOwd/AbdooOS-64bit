#include "vfs.h"
#include <lib/string.h>
#include <lib/print.h>
#include <mm/pmm.h>
#include <fs/imfs.h>

vfs_t* vfs;

void vfs_init() {
	vfs = (vfs_t*) malloc(sizeof(vfs_t));
	if (vfs == NULL) return;
	vfs->mountpoints = (mountpoint_t**) malloc(sizeof(mountpoint_t) * MAX_MOUNTPOINT_COUNT);
	if (vfs->mountpoints == NULL) return;
	vfs->mountpoints_count = 0;

	mount("MemFS", "imfs");
}

void mount(char* name, char* type) {
	if (vfs->mountpoints_count >= MAX_MOUNTPOINT_COUNT) return;

	vfs->mountpoints[vfs->mountpoints_count] = (mountpoint_t*) malloc(sizeof(mountpoint_t));
	mountpoint_t* new_mnt = vfs->mountpoints[vfs->mountpoints_count];
	
	strcpy(new_mnt->name, name);
	strcpy(new_mnt->type, type);
	new_mnt->files = (file_t**) malloc(sizeof(file_t) * MAX_FILE_COUNT);

	if (new_mnt->files == NULL) return;

	register_operations(new_mnt);

	vfs->mountpoints_count++;
}

void register_fs() {
	for (size_t i = 0; i < vfs->mountpoints_count; i++) {
		register_operations(vfs->mountpoints[i]);
	}
}

void register_operations(mountpoint_t* mnt) {
	mnt->operations = malloc(sizeof(fs_operations_t) * FS_OPERATIONS_COUNT);
	if (mnt->operations == NULL) return;
	if (strsame(mnt->type, "imfs")) {
		mnt->operations->create = IMFS_file_create;
		mnt->operations->write  = IMFS_file_write;
		mnt->operations->read   = IMFS_file_read;
		mnt->operations->open	= NULL;
	}
}

void clean_up() {
	for (size_t i = 0; i < vfs->mountpoints_count; i++) {
		mfree(vfs->mountpoints[i]->operations);
		mfree(vfs->mountpoints[i]->files);
	}
	
	mfree(vfs->mountpoints);
	mfree(vfs);
}


/* operations */

int vfs_create(char* mnt_name, char* filename) {
	for (size_t i = 0; i < vfs->mountpoints_count; i++) {
		mountpoint_t* mnt = vfs->mountpoints[i];
		if (strsame(mnt_name, mnt->name))
			return mnt->operations->create(filename);
	}

	return -404;
}

int vfs_write(char* mnt_name, char* filename, void* data, size_t size) {
	for (size_t i = 0; i < vfs->mountpoints_count; i++) {
		mountpoint_t* mnt = vfs->mountpoints[i];
		if (strsame(mnt_name, mnt->name))
			return mnt->operations->write(filename, data, size);
	}

	return -404;
}

int vfs_read(char* mnt_name, char* filename, void* buffer, size_t size) {
	for (size_t i = 0; i < vfs->mountpoints_count; i++) {
		mountpoint_t* mnt = vfs->mountpoints[i];
		if (strsame(mnt_name, mnt->name))
			return mnt->operations->read(filename, buffer, size);
	}

	return -404;
}