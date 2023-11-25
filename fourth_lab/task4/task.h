#ifndef __TASK_H__
#define __TASK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "boolean_vector.h"
#include "tree.h"

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER,
    UNINITIALIZED_VALUE
} status_codes;

typedef enum {
    disjunction,
    conjuction,
    implication,
    reverse_implication,
    equivalence,
    xor, 
    coimplication,
    schaeffer,
    pierce
} binary_actions;

status_codes getInteger(int *result, const unsigned char *number, const int base);

status_codes getBase(unsigned char **result, int number, const int base);

unsigned char binary_operation(const unsigned char _1, const unsigned char _2, const binary_actions action);

#endif