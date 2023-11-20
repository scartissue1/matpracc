#ifndef __HASH_H__
#define __HASH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "task.h"
#include "list.h"

typedef struct {
    List **data;
    unsigned long long (*hash_function)(const char *key);
    size_t hashsize;
} HashTable;

unsigned long long int hashFunction(const char *key);

status_codes createTable(HashTable **new, const int size, unsigned long long int (*hash_function)(const char *key));

status_codes resizeTable(HashTable **new, HashTable *old);

void freeTable(HashTable *data);

int checkTable(HashTable *table);

status_codes insertTable(HashTable *table, const char *key, const char *value, int hash_flag, int key_hash);

char *findTable(HashTable *table, const char *key);

#endif