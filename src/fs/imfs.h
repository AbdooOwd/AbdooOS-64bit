#ifndef IMFS_H
#define IMFS_H
#pragma once

#include <include/types.h>

#define MAX_FILES_COUNT 	100
#define MAX_IMFS_FILENAME_LENGTH 100
#define MAX_FILE_SIZE 		1024

typedef struct {
	char 	filename[MAX_IMFS_FILENAME_LENGTH];
	char	data[MAX_FILE_SIZE];
	size_t	size;
} FileEntry;

typedef struct {
	FileEntry 	files[MAX_FILES_COUNT];
	size_t 		count;
} IMFS;


extern IMFS* imfs;

/* functions */

void 		IMFS_create			();
bool 		IMFS_file_exists	(char* filename);
int 		IMFS_file_create	(char* filename);
int 		IMFS_file_write		(char* filename, void* data, size_t size);
int 		IMFS_file_read		(char* filename, void* buffer, size_t size);
int 		IMFS_file_delete	(char* filename);
FileEntry* 	IMFS_file_get		(char* filename);

#endif