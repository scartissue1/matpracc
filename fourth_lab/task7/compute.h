#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "MemoryCell.h"
#include "stack.h"
#include "task.h"

int isOperator(const char symbol);

int getPriority(const char operator);

status_codes getPostfix(char **postfix, const char *infix);

status_codes computePostfix(long long *result, const char *postfix, ArrayCell array, size_t array_size);

#endif