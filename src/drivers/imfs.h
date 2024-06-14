#ifndef IMFS_H
#define IMFS_H
#pragma once

#include <include/types.h>

#define MAX_FILES_COUNT 	100
#define MAX_FILENAME_LENGTH 100
#define MAX_FILE_SIZE 		1024

typedef struct {
	char 	filename[MAX_FILENAME_LENGTH];
	char	data[MAX_FILE_SIZE];
	size_t	size;
} FileEntry;

typedef struct {
	FileEntry 	files[MAX_FILES_COUNT];
	size_t 		count;
} IMFS;


/* functions */

IMFS* 		IMFS_create			();
bool 		IMFS_file_exists	(IMFS* imfs, char* filename);
int 		IMFS_file_create	(IMFS* imfs, char* filename);
int 		IMFS_file_write		(IMFS* imfs, char* filename, char* data);
int 		IMFS_file_read		(IMFS* imfs, char* filename, char* buffer);
int 		IMFS_file_delete	(IMFS* imfs, char* filename);
FileEntry* 	IMFS_file_get		(IMFS* imfs, char* filename);

#endif