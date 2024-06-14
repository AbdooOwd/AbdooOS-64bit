#include "hash.h"
#include <mm/umm_malloc.h>
#include <lib/string.h>


u64 hash(const char *key) {
    u64 value = 0;
    size_t i = 0;
    size_t key_len = strlen((char*) key);

    // do several rounds of multiplication
    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    value = value % HASH_MAX_CAPACITY;

    return value;
}



HashTable* hash_create() {
	HashTable* hash = malloc(sizeof(HashTable));
	hash->hash_entries = malloc(sizeof(HashEntry) * HASH_MAX_CAPACITY);

	for (size_t i = 0; i < HASH_MAX_CAPACITY; i++)
		hash->hash_entries[i] = NULL;
	
	return hash;
}

HashEntry* hash_pair(char* key, char* value) {
	HashEntry* entry = malloc(sizeof(HashEntry));

	entry->key = malloc(strlen(key) + 1);
	entry->value = malloc(strlen(value) + 1);

	strcpy(entry->key, key);
	strcpy(entry->value, value);

	entry->next = NULL;

	return entry;
}

void hash_set(HashTable* hash_table, char* key, char* value) {
	size_t index = hash(key);

	HashEntry* entry = hash_table->hash_entries[index];

	// if no hash-table collision
	if (entry == NULL) {
		hash_table->hash_entries[index] = hash_pair(key, value);
		return;
	}

	HashEntry* prev;

	while (entry != NULL) {
		// found already existing key. Overwrite it
		if (strsame(entry->key, key)) {
			free(entry->value);
			entry->value = malloc(strlen(value) + 1);
			strcpy(entry->value, value);
			return;
		}

		prev = entry;
		entry = (HashEntry*) prev->next;
	}

	prev->next = (struct HashEntry*) hash_pair(key, value);
}

char* hash_get(HashTable* hash_table, char* key) {
	size_t index = hash(key);
	if (index >= HASH_MAX_CAPACITY)
		return NULL;
	
	HashEntry* entry = hash_table->hash_entries[index];

	if (entry == NULL) {
		// u gettin no entry bruh (its empty)
		return NULL;
	}


	/**
	 * If a Hash Table collision happens, then we have a list
	 * of items with the same key. Which means that to find the
	 * requested value of x key, we iterate through the linked list.
	 */

	while (entry != NULL) {
		if (strsame(entry->key, key))
			return entry->value;
		entry = (HashEntry*) entry->next;
	}

	// ther'ws nothin' bruh
	return NULL;
}