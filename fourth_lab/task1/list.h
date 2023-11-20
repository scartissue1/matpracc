#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

#include "task.h"
#include "string.h"

typedef struct {
    char *key;
    char *value;
    unsigned long long int key_hash;
} HashItem;

typedef struct List {
    HashItem *value;
    int size;
    struct List *next;
} List;

status_codes createItem(HashItem **new, const char *key, const char *value, int hash_flag, int key_hash, unsigned long long int (*hash_function)(const char *key));

status_codes addList(List **head, HashItem *item);

void freeItem(HashItem *data);

void freeList(List *root);

#endif