#include "hash.h"

unsigned long long int hashFunction(const char *key) {
    size_t key_size = strlen(key);
    if (!key_size) {
        return 0;
    }
    unsigned long long int result = 0;
    int power = 1;
    for (int i = key_size; i >= 0; i--) {
        if (key[i] == '\0') {
            continue;
        }
        result += power * ((isdigit(key[i])) ? key[i] - '0' : ('A' <= key[i] && key[i] <= 'Z') ? key[i] - 'A' + 10 : key[i] - 'a' + 36);
        power *= 62;
    }
    return result;
}

status_codes createTable(HashTable **new, const int size, unsigned long long int (*hash_function)(const char *key)) {
    (*new) = (HashTable *)malloc(sizeof(HashTable));
    if (!(*new)) {
        return NO_MEMORY;
    }
    (*new)->hashsize = size;
    (*new)->data = (List **)malloc(sizeof(List*) * size);
    if (!(*new)->data) {
        free(*new);
        *new = NULL;
        return NO_MEMORY;
    }
    for (int i = 0; i < (*new)->hashsize; i++) {
        (*new)->data[i] = NULL;
    }
    (*new)->hash_function = hash_function;
    return OK;
}

status_codes resizeTable(HashTable **new, HashTable *old) {
    int new_hashsize = getPrime(old->hashsize);
    if (createTable(new, new_hashsize, old->hash_function) == NO_MEMORY) {
        return NO_MEMORY;
    }
    for (int i = 0; i < old->hashsize; i++) {
        if (!old->data[i]) {
            continue;
        }
        while (old->data[i]->next) {
            if (insertTable((*new), old->data[i]->value->key, old->data[i]->value->value, 1, old->data[i]->value->key_hash) == NO_MEMORY) {
                return NO_MEMORY;
            }
            old->data[i] = old->data[i]->next;
        }
    }
    return OK;
}

void freeTable(HashTable *data) {
    for (int i = 0; i < data->hashsize; i++) {
        freeList(data->data[i]);
    }
    free(data->data);
    free(data);
}

int checkTable(HashTable *table) {
    int min = -1;
    int max = -1;
    for (int i = 0; i < table->hashsize; i++) {
        if (table->data[i]) {
            if (min > table->data[i]->size || min == -1) {
                min = table->data[i]->size;
            }
            if (table->data[i]->size > max) {
                max = table->data[i]->size;
            }
        }
        else {
            min = 0;
        }
    }
    if (min != 0) {
        if (max / min >= 2) {
            return 1;
        }
    }
    return 0;
}

status_codes insertTable(HashTable *table, const char *key, const char *value, int hash_flag, int key_hash) {
    HashItem *new = NULL;
    if (createItem(&new, key, value, hash_flag, key_hash, table->hash_function) == NO_MEMORY) {
        return NO_MEMORY;
    }
    int index = new->key_hash % table->hashsize;
    if (addList(&table->data[index], new) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (checkTable(table)) {
        HashTable *new_table = NULL;
        if (resizeTable(&new_table, table) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freeTable(table);
        table = new_table;
    }
    return OK;
}

char *compare(List *root, const char *key) {
    if (!root) {
        return NULL;
    }
    if (strcmp(root->value->key, key) == 0) {
        return root->value->value;
    }
    compare(root->next, key);
}

char *findTable(HashTable *table, const char *key) {
    int key_hash = table->hash_function(key);
    int index = key_hash % table->hashsize;
    return compare(table->data[index], key);
}