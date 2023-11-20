#include "list.h"

status_codes createItem(HashItem **new, const char *key, const char *value, int hash_flag, int key_hash, unsigned long long int (*hash_function)(const char *key)) {
    (*new) = (HashItem *)malloc(sizeof(HashItem));
    if (!(*new)) {
        return NO_MEMORY;
    }
    size_t key_size = strlen(key);
    (*new)->key = (char *)malloc(sizeof(char) * key_size);
    if (!(*new)->key) {
        free(*new);
        return NO_MEMORY;
    }
    memcpy((*new)->key, key, key_size);
    size_t value_size = strlen(value);
    (*new)->value = (char *)malloc(sizeof(char) * value_size);
    if (!(*new)->value) {
        free((*new)->key);
        free(*new);
        return NO_MEMORY;
    }
    memcpy((*new)->value, value, value_size);
    if (!hash_flag) {
        (*new)->key_hash = hash_function(key);
    }
    else {
        (*new)->key_hash = key_hash;
    }
    return OK;
}

status_codes addList(List **head, HashItem *item) {
    List *tmp = (List *)malloc(sizeof(List));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->value = item;
    tmp->next = (*head);
    if (!tmp->next) {
        tmp->size = 1;
    }
    else {
        tmp->size = (*head)->size + 1;
    }
    (*head) = tmp;
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