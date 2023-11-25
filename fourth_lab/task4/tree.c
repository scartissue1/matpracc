#include "tree.h"

status_codes addNode(Node **ptr, BooleanVector *data) {
    if (!(*ptr)) {
        (*ptr) = (Node *)malloc(sizeof(Node));
        if (!(*ptr)) {
            return NO_MEMORY;
        }
        (*ptr)->data = data;
        (*ptr)->left = NULL;
        (*ptr)->right = NULL;
        return OK;
    }
    if ((*ptr)->data->name == data->name) {
        freeVector((*ptr)->data);
        (*ptr)->data = data;
        return INVALID_PARAMETER;
    }
    if ((*ptr)->data->name > data->name) {
        return addNode(&((*ptr)->data), data);
    }
    if ((*ptr)->data->name < data->name) {
        return addNode(&((*ptr)->data), data);
    }
}

void freeNode(Node *ptr) {
    if (!ptr) {
        return;
    }
    freeNode(ptr->left);
    freeNode(ptr->right);
    freeVector(ptr->data);
    free(ptr);
    ptr = NULL;
}

void findNode(Node *ptr, BooleanVector **result, const char name_to_find) {
    if (!ptr) {
        return;
    }
    if (ptr->data->name == name_to_find) {
        (*result) = ptr->data;
        return;
    }
    if (ptr->data->name > name_to_find) {
        findNode(ptr->left, result, name_to_find);
    }
    else {
        findNode(ptr->right, result, name_to_find);
    }
}