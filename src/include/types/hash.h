#ifndef HASH_H
#define HASH_H
#pragma once

#include <include/types.h>

#define HASH_MAX_CAPACITY 64

typedef struct {
	char* key;
	char* value;
	struct HashEntry* next;
} HashEntry;

typedef struct {
	HashEntry** hash_entries;
} HashTable;


u64 hash(const char *key);
HashTable* hash_create();
HashEntry* hash_pair(char* key, char* value);
void hash_set(HashTable* hash, char* key, char* value);
char* hash_get(HashTable* hash, char* le_key);

#endif