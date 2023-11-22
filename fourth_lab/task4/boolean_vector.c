#include "boolean_vector.h"

status_codes initVector(BooleanVector **ptr, const size_t size) {
    BooleanVector *tmp = (BooleanVector *)malloc(sizeof(BooleanVector));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->size = size;
    tmp->capacity = size;
    tmp->array = (int *)malloc(sizeof(int) * tmp->capacity);
    if (!tmp->array) {
        free(tmp);
        return NO_MEMORY;
    }
    for (int i = 0; i < tmp->size; i++) {
        tmp->array[i] = 0;
    }
    (*ptr) = tmp;
    return OK;
}

