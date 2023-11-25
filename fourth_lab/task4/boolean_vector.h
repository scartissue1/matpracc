#ifndef __BINARY_VECTOR_H__
#define __BINARY_VECTOR_H__

#include <stdlib.h>

#include "task.h"

typedef struct {
    char name;
    unsigned char *array;
    size_t size;
} BooleanVector;

status_codes initVector(BooleanVector **ptr, const size_t size, const char name);

void freeVector(BooleanVector *ptr);

status_codes readVector(BooleanVector **ptr, const char *number, const int base);

status_codes writeVector(BooleanVector **ptr, const char **result, const int base);

status_codes binary_actionVector(BooleanVector **result, const char result_name, BooleanVector *first, BooleanVector *second, binary_actions action);

void inverseVector(BooleanVector *ptr);

void printVector(FILE *stream, BooleanVector *ptr);

#endif