#include "MemoryCell.h"

void freeMemoryCell(MemoryCell *cell) {
    if (!cell) {
        return;
    }
    free(cell->name);
    free(cell);
}

void freeArrayCell(ArrayCell array, size_t size) {
    for (int i = 0; i < size; i++) {
        freeMemoryCell(array[i]);
    }
    free(array);
}

int comp(const MemoryCell **first, const MemoryCell ** second) {
    return strcmp((*first) -> name, (*second) -> name);
}

status_codes initializeArray(ArrayCell *array, size_t *size, size_t *capacity) {
    (*size) = 0;
    (*capacity) = 2;
    (*array) = (MemoryCell **)malloc(sizeof(MemoryCell *) * (*capacity));
    if (!(*array)) {
        return NO_MEMORY;
    }
    return OK;
}

status_codes addCell(ArrayCell *array, size_t *size, size_t *capacity, const char *name, const long long value) {
    MemoryCell *tmp = (MemoryCell *)malloc(sizeof(MemoryCell));
    if (!tmp) {
        return NO_MEMORY;
    }
    size_t name_size = strlen(name) + 1;
    tmp->name = (char *)malloc(sizeof(char) * name_size);
    if (!(tmp->name)) {
        free(tmp);
        return NO_MEMORY;
    }
    memcpy(tmp->name, name, name_size);
    tmp->name[name_size] = '\0';
    tmp->value = value;
    (*array)[*size] = tmp;
    (*size)++;
    if ((*size) >= (*capacity)) {
        (*capacity) *= 2;
        MemoryCell **tmp2 = NULL;
        if (!(tmp2 = (MemoryCell **)realloc((*array), sizeof(MemoryCell *) * (*capacity)))) {
            freeArrayCell(*array, (*size));
            return NO_MEMORY;
        }
        (*array) = tmp2;
    }
    qsort((*array), (*size), sizeof((*array)[0]), (int(*) (const void *, const void *))comp);
    return OK;
}

int binarySearch(ArrayCell array, const size_t size, const char *to_find) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int median = (left + right) / 2;
        if (strcmp(array[median]->name, to_find) > 0) {
            right = median - 1;
        }
        else if (strcmp(array[median]->name, to_find) < 0) {
            left = median + 1;
        }
        else {
            return median;
        }
    }
    return -1;
}