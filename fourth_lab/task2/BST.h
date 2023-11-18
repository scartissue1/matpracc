#ifndef BST_H__
#define BST_H__

#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "Array.h"

typedef struct Node {
    Array *data;
    struct Node *left;
    struct Node *right;
} Node;

status_codes addNode(Node **ptr, Array *arr);

void findNode(Node *ptr, Array **result, char name);

void freeNode(Node *ptr);

#endif