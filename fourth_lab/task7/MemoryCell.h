#ifndef __CELL_H__
#define __CELL_H__

#include <stdlib.h>
#include <string.h>

#include "task.h"

typedef struct {
    char *name;
    long long value;
} MemoryCell, **ArrayCell;

void freeMemoryCell(MemoryCell *cell);

void freeArrayCell(ArrayCell array, size_t size);

int comp(const MemoryCell **first, const MemoryCell ** second);

status_codes initializeArray(ArrayCell *array, size_t *size, size_t *capacity);

status_codes addCell(ArrayCell *array, size_t *size, size_t *capacity, const char *name, const long long value);

int binarySearch(ArrayCell array, const size_t size, const char *to_find);

#endif