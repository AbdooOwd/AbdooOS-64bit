#include "imfs.h"
#include <lib/mem.h>
#include <lib/string.h>
#include <lib/print.h>

// TODO: Use hash tables to find files by filename

IMFS* imfs = NULL;

void IMFS_create() {
	imfs = (IMFS*) malloc(sizeof(IMFS));

	if (imfs) {
		memset(imfs, 0, sizeof(IMFS));
		imfs->count = 0;
	} else 
		imfs = NULL;
}

bool IMFS_file_exists(char* filename) {
	if (imfs->count <= 0 || imfs->count > MAX_FILES_COUNT) return false;

	for (size_t i = 0; i < imfs->count; i++) {
		if (strsame(imfs->files[i].filename, filename))
			return true;
	}

	return false;
}

int IMFS_file_create(char* filename) {
	/* errer handling */
	p("1\n");
	if (imfs->count >= MAX_FILES_COUNT)
		return -1;
	p("2\n");

	if (strlen(filename) > MAX_FILENAME_LENGTH)
		return -2;
	p("3\n");
	
	if (IMFS_file_exists(filename))
		return -3;
	p("4\n");
	
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