#ifndef VFS_H
#define VFS_H
#pragma once

#include <include/types.h>


#define MAX_MOUNTPOINT_COUNT	8
#define MOUNTPOINT_NAME_LENGTH	32

#define DEFAULT_MAX_NAME_LENGTH	64

#define MAX_FILENAME_LENGTH		128
#define MAX_FILE_COUNT			10
#define MAX_FILE_SIZE			1024

#define FS_OPERATIONS_COUNT		4


typedef struct {
	int (*open)(char* filename);
	int (*create)(char* filename);
	int (*write)(char* filename, void* data, size_t size);
	int (*read)(char* filename, void* buffer, size_t size);
} fs_operations_t;

typedef struct {
	char	filename[MAX_FILENAME_LENGTH];
	void* 	data[MAX_FILE_SIZE];
	size_t 	size;
} file_t;

typedef struct {
	char			name[MOUNTPOINT_NAME_LENGTH];
	char			type[DEFAULT_MAX_NAME_LENGTH];
	file_t**		files;	// can't store files in memory
	size_t			files_count;
	fs_operations_t* operations;
} mountpoint_t;

typedef struct {
	mountpoint_t** 	mountpoints;
	size_t			mountpoints_count;
} vfs_t;

extern vfs_t* vfs;

void vfs_init();
void mount(char* name, char* type);
void register_operations(mountpoint_t* mnt);
void register_fs();
void clean_up();

int vfs_create(char* mnt_name, char* filename);
int vfs_write(char* mnt_name, char* filename, void* data, size_t size);
int vfs_read(char* mnt_name, char* filename, void* buffer, size_t size);

#endif