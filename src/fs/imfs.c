#include "imfs.h"
#include <mm/bm_alloc.h>
#include <lib/string.h>
#include <lib/print.h>

// TODO: Use hash tables to find files by filename

IMFS* IMFS_create() {
	IMFS* new_imfs = malloc(sizeof(IMFS));

	if (new_imfs)
		new_imfs->count = 0;

	return new_imfs;
}

bool IMFS_file_exists(IMFS* imfs, char* filename) {
	for (size_t i = 0; i < imfs->count; i++) {
		if (strsame(imfs->files[i].filename, filename))
			return true;
	}

	return false;
}

int IMFS_file_create(IMFS* imfs, char* filename) {
	/* errer handling */
	
	if (IMFS_file_exists(imfs, filename))
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

int IMFS_file_write(IMFS* imfs, char* filename, char* data) {
	if (strlen(data) > MAX_FILE_SIZE)
		return -1;
	
	if (!IMFS_file_exists(imfs, filename))
		return -2;

	// if (!IMFS_file_exists(imfs, filename))
	// 	IMFS_file_create(imfs, filename);
	
	FileEntry* file = IMFS_file_get(imfs, filename);

	strlcpy(file->data, data, MAX_FILE_SIZE);
	file->size = strlen(data);

	return 0;
}

int IMFS_file_read(IMFS* imfs, char* filename, char* buffer) {
	if (IMFS_file_exists(imfs, filename)) {
		FileEntry* file = IMFS_file_get(imfs, filename);
		strlcpy(buffer, file->data, MAX_FILE_SIZE);
		return 0;
	}
	return -1;
}

int IMFS_file_delete(IMFS* imfs, char* filename) {
	if (!IMFS_file_exists(imfs, filename))
		return -1;
	
	FileEntry* file = IMFS_file_get(imfs, filename);
	if (file != NULL) file = NULL;
	
	return 0;
}

FileEntry* IMFS_file_get(IMFS* imfs, char* filename) {
	for (size_t i = 0; i < imfs->count; i++) {
		if (strsame(filename, imfs->files[i].filename))
			return &imfs->files[i];
	}

	return NULL;
}