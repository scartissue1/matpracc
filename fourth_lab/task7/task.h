#ifndef __TASK_H__
#define __TASK_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER,
    ZERO_SIZE,
    NEGATIVE_MOD,
    INVALID_SYMBOL,
    INVALID_NAME,
    DIVIDE_BY_ZERO,
    UNEQUAL_BRACKET,
    EMPTY_BRACKET,
    UNINITIALIZED_VALUE,
    UNUSED_VARS_OR_OPERATORS
} status_codes;

int int_validation(const char *number);

int name_validation(const char *name);

#endif