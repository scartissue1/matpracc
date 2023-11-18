#ifndef __HASH_H__
#define __HASH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "task.h"

typedef struct {
    char *key;
    char *value;
    unsigned long long int key_hash;
} HashItem;

typedef struct List {
    HashItem *value;
    struct List *next;
} List;

typedef struct {
    List **data;
    int *size_array;
    size_t hashsize;
    size_t count;
} HashTable;

unsigned long long int hashFunction(const char *key);

status_codes createItem(HashItem **new, const char *key, const char *value, int hash_flag, int key_hash);

status_codes createList(List **new, HashItem *item);

status_codes addList(List *root, HashItem *item);

status_codes createTable(HashTable **new, const int size);

status_codes resizeTable(HashTable **new, HashTable *old);

void freeItem(HashItem *data);

void freeList(List *root);

void freeTable(HashTable *data);

int getListlength(List *root);

int checkTable(HashTable *table);

status_codes insertTable(HashTable *table, const char *key, const char *value, int hash_flag, int key_hash);

char *findTable(HashTable *table, const char *key);

#endif