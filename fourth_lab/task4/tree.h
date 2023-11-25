#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include "boolean_vector.h"

typedef struct Node {
    BooleanVector *data;
    struct Node *left;
    struct Node *right;
} Node;

status_codes addNode(Node **ptr, BooleanVector *data);

status_codes freeNode(Node *ptr);

void findNode(Node *ptr, BooleanVector **result, const char name_to_find);

#endif