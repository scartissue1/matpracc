#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "status_codes.h"
#include "stack.h"
#include "tree.h"

int getPriority(const char operation);

status_codes getTree(Node **root, char *infix);

int computeTree(Node *root, const char *vars, const int *values, const int vars_count);

#endif