#include "BST.h"

status_codes addNode(Node **ptr, Array *arr) {
    if (!(*ptr)) {
        (*ptr) = (Node *)malloc(sizeof(Node));
        if (!(*ptr)) {
            return NO_MEMORY;
        }
        (*ptr)->data = arr;
        (*ptr)->left = NULL;
        (*ptr)->right = NULL;
        return OK;
    }
    if ((*ptr)->data->name > arr->name) {
        return addNode(&((*ptr)->left), arr);
    }
    if ((*ptr)->data->name < arr->name) {
        return addNode(&((*ptr)->right), arr);
    }
    return OK;
}

void findNode(Node *ptr, Array **result, char name) {
    if (!ptr) {
        return;
    }
    if (ptr->data->name == name) {
        (*result) = ptr->data;
        return;
    }
    if (ptr->data->name > name) {
        findNode(ptr->left, result, name);
    }
    else {
        findNode(ptr->right, result, name);
    }
}

void freeNode(Node *ptr) {
    if (!ptr) {
        return;
    }
    freeNode(ptr->left);
    freeNode(ptr->right);
    free(ptr->data->data);
    free(ptr->data);
    free(ptr);
}