#ifndef HASH_H
#define HASH_H
#pragma once

#include "types.h"
#include <lib/mem.h>
#include <lib/string.h>

typedef struct {
    char* key;
    char* value;             // we don't know the type that we'll get sooooo
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry *entries;
    size_t size;
} HashTable;


u32 hash(const char *str) {
    u32 hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

HashTable hash_create() {
    HashTable new_hash = { 0 , 0 };
    return new_hash;
}

void hash_add(HashTable hash, char* key, char* value) {
    HashEntry new_entry = { key , value , 0 };
    hash.entries[hash.size].next = &new_entry;
    hash.size = hash.size + 1;
}

char* hash_get(HashTable hash, char* key) {
    for (size_t i = 0; i < hash.size ; i++)
        if (strsame(hash.entries[i].key, key))
            return hash.entries[i].value;
}


#endif