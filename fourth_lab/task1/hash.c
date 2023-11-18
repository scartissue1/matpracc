#include "hash.h"

unsigned long long int hashFunction(const char *key) {
    size_t key_size = strlen(key);
    unsigned long long int result = 0;
    int power = 1;
    for (int i = key_size; i >= 0; i--) {
        result += power * (isdigit(key[i])) ? key[i] - '0' : ('A' <= key[i] && key[i] <= 'Z') ? key[i] - 'A' + 10 : key[i] - 'a' + 36;
        power *= 62;
    }
    return result;
}

status_codes createItem(HashItem **new, const char *key, const char *value, int hash_flag, int key_hash) {
    (*new) = (HashItem *)malloc(sizeof(HashItem));
    if (!(*new)) {
        return NO_MEMORY;
    }
    size_t key_size = strlen(key);
    (*new)->key = (char *)malloc(sizeof(char) * key_size);
    if (!(*new)->key) {
        return NO_MEMORY;
    }
    memcpy((*new)->key, key, key_size);
    size_t value_size = strlen(value);
    (*new)->value = (char *)malloc(sizeof(char) * value_size);
    if (!(*new)->value) {
        return NO_MEMORY;
    }
    memcpy((*new)->value, value, value_size);
    if (!hash_flag) {
        (*new)->key_hash = hashFunction(key);
    }
    else {
        (*new)->key_hash = key_hash;
    }
    return OK;
}

status_codes createList(List **new, HashItem *item) {
    List *tmp = (List *)malloc(sizeof(List));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->value = item;
    tmp->next = (*new);
    (*new) = tmp;
    return OK;
}

status_codes addList(List *root, HashItem *item) {
    while (root->next) {
        root = root->next;
    }
    List *tmp = (List *)malloc(sizeof(List));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->value = item;
    tmp->next = NULL;
    root->next = tmp;
    return OK;
}

status_codes createTable(HashTable **new, const int size) {
    (*new) = (HashTable *)malloc(sizeof(HashTable));
    if (!(*new)) {
        return NO_MEMORY;
    }
    (*new)->hashsize = size;
    (*new)->count = 0;
    (*new)->size_array = (int *)malloc(sizeof(int) * size);
    if (!(*new)->size_array) {
        free(*new);
        return NO_MEMORY;
    }
    for (int i = 0; i < size; i++) {
        (*new)->size_array[i] = 0;
    }
    (*new)->data = (List **)malloc(sizeof(List *) * size);
    if (!(*new)->data) {
        free((*new)->size_array);
        free(*new);
        return NO_MEMORY;
    }
    for (int i = 0; i < (*new)->hashsize; i++) {
        (*new)->data[i] = NULL;
    }
    return OK;
}

status_codes resizeTable(HashTable **new, HashTable *old) {
    int new_hashsize = getPrime(old->hashsize);
    if (createTable(new, new_hashsize) == NO_MEMORY) {
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

void freeItem(HashItem *data) {
    free(data->key);
    free(data->value);
    free(data);
}

void freeList(List *root) {
    if (!root) {
        return;
    }
    freeList(root->next);
    freeItem(root->value);
    free(root);
}

void freeTable(HashTable *data) {
    for (int i = 0; i < data->hashsize; i++) {
        freeList(data->data[i]);
    }
    free(data->data);
    free(data->size_array);
    free(data);
}

int getListlength(List *root) {
    if (!root) {
        return 0;
    }
    int result = 1;
    while (root->next) {
        root = root->next;
        result++;
    }
    return result;
}

int checkTable(HashTable *table) {
    int min = -1;
    int max = -1;
    for (int i = 0; i < table->hashsize; i++) {
        if (min > table->size_array[i] || min == -1) {
            min = table->size_array[i];
        }
        if (table->size_array[i] > max) {
            max = table->size_array[i];
        }
    }
    if (min != 0 && max / min >= 2) {
        return 1;
    }
    return 0;
}

status_codes insertTable(HashTable *table, const char *key, const char *value, int hash_flag, int key_hash) {
    HashItem *new = NULL;
    if (createItem(&new, key, value, hash_flag, key_hash) == NO_MEMORY) {
        return NO_MEMORY;
    }
    int index = new->key_hash % table->hashsize;
    if (!table->data[index]) {
        if (createList(&(table->data[index]), new) == NO_MEMORY) {
            return NO_MEMORY;
        }
        table->count++;
    }
    else {
        if (addList(table->data[index], new) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    table->size_array[index] = getListlength(table->data[index]);
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

char *findTable(HashTable *table, const char *key) {
    int key_hash = hashFunction(key);
    int index = key_hash % table->hashsize;
    while (table->data[index]) {
        if (strcmp(table->data[index]->value->key, key) == 0) {
            return table->data[index]->value->value;
        }
    } 
    return NULL;
}