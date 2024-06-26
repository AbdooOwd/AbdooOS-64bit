#include "imfs.h"
#include <mm/bm_alloc.h>
#include <lib/string.h>
#include <lib/print.h>

// TODO: Use hash tables to find files by filename

IMFS* imfs;

void IMFS_create() {
	imfs = malloc(sizeof(IMFS));

	if (imfs)
		imfs->count = 0;
}

bool IMFS_file_exists(char* filename) {
	for (size_t i = 0; i < imfs->count; i++) {
		if (strsame(imfs->files[i].filename, filename))
			return true;
	}

	return false;
}

int IMFS_file_create(char* filename) {
	/* errer handling */
	
	if (IMFS_file_exists(filename))
		return -1;
		
	if (strlen(filename) > MAX_FILENAME_LENGTH)
		return -2;
		
	if (imfs->count >= MAX_FILES_COUNT)
		return -3;
	
	
	strlcpy(imfs->files[imfs->count].filename, filename, MAX_FILENAME_LENGTH);
	imfs->files[imfs->count].size = 0;
	imfs->count++;

	return 0;
}

int IMFS_file_write(char* filename, void* data, size_t size) {
	if (strlen(data) > MAX_FILE_SIZE)
		return -1;
	
	if (!IMFS_file_exists(filename))
		return -2;

	// if (!IMFS_file_exists(filename))
	// 	IMFS_file_create(filename);
	
	FileEntry* file = IMFS_file_get(filename);

	strlcpy(file->data, data, MAX_FILE_SIZE);
	file->size = strlen(data);

	return 0;
}

int IMFS_file_read(char* filename, void* buffer, size_t size) {
	if (IMFS_file_exists(filename)) {
		FileEntry* file = IMFS_file_get(filename);
		strlcpy(buffer, file->data, MAX_FILE_SIZE);
		return 0;
	}
	return -1;
}

int IMFS_file_delete(char* filename) {
	if (!IMFS_file_exists(filename))
		return -1;
	
	FileEntry* file = IMFS_file_get(filename);
	if (file != NULL) file = NULL;
	
	return 0;
}

FileEntry* IMFS_file_get(char* filename) {
	for (size_t i = 0; i < imfs->count; i++) {
		if (strsame(filename, imfs->files[i].filename))
			return &imfs->files[i];
	}

	return NULL;
}