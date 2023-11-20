#include "tree.h"

status_codes createNode(Node **ptr, const char data, Node *left, Node *right) {
    if (*ptr) {
        return INVALID_PARAMETER;
    }
    (*ptr) = (Node *)malloc(sizeof(Node));
    if (!(*ptr)) {
        return NO_MEMORY;
    }
    (*ptr)->data = data;
    (*ptr)->left = left;
    (*ptr)->right = right;
    return OK;
}

void freeTree(Node *ptr) {
    if (!ptr) {
        return;
    }
    freeTree(ptr->left);
    freeTree(ptr->right);
    free(ptr);
}

void printTree(Node *ptr, int level) {
    if (!ptr) {
        return;
    }
    printTree(ptr->left, level + 1);
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%c\n", ptr->data);
    printTree(ptr->right, level + 1);
}