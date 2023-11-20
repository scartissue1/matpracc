#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <stdio.h>

#include "status_codes.h"

typedef struct Node {
    char data;
    struct Node *left;
    struct Node *right;
} Node;

status_codes createNode(Node **ptr, const char data, Node *left, Node *right);

void freeTree(Node *ptr);

void printTree(Node *ptr, int level);

#endif