#include "boolean_vector.h"

status_codes initVector(BooleanVector **ptr, const size_t size, const char name) {
    if (*ptr) {
        return INVALID_PARAMETER;
    }
    BooleanVector *tmp = (BooleanVector *)malloc(sizeof(BooleanVector));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->name = name;
    tmp->size = size + 1;
    tmp->array = (unsigned char *)malloc(sizeof(unsigned char) * tmp->size);
    if (!tmp->array) {
        free(tmp);
        return NO_MEMORY;
    }
    tmp->array[tmp->size] = '\0';
    (*ptr) = tmp;
    return OK;
}

void freeVector(BooleanVector *ptr) {
    if (!ptr) {
        return;
    }
    free(ptr->array);
    free(ptr);
    ptr = NULL;
}

status_codes readVector(BooleanVector **ptr, const char *number, const int base) {
    if (*ptr) {
        return INVALID_PARAMETER;
    }
    int converted = 0;
    if (getInteger(&converted, number, base) == INVALID_PARAMETER) {
        freeVector(*ptr);
        return INVALID_PARAMETER;
    }
    status_codes status = getBase(&(*ptr)->array, converted, 10);
    if (status != OK) {
        freeVector(*ptr);
        return status;
    }
    return OK;
}

status_codes writeVector(BooleanVector **ptr, const char **result, const int base) {
    if (!*ptr) {
        return INVALID_PARAMETER;
    }
    int converted = 0;
    if (getInteger(&converted, &(*ptr)->array, 2) == INVALID_PARAMETER) {
        freeVector(*ptr);
        return INVALID_PARAMETER;
    }
    status_codes status = getBase(result, converted, base);
    if (status != OK) {
        freeVector(*ptr);
        return INVALID_PARAMETER;
    }
    return OK;
}

status_codes binary_actionVector(BooleanVector **result, const char result_name, BooleanVector *first, BooleanVector *second, binary_actions action) {
    if (*result) {
        return INVALID_PARAMETER;
    }
    size_t result_size = (first->size > second->size) ? first->size : second->size;
    if (initVector(result, result_size, result_name) == NO_MEMORY) {
        freeVector(first);
        freeVector(second);
        return NO_MEMORY;
    }
    size_t first_index = first->size - 1;
    size_t second_index = second->size - 1;
    size_t result_index = result_size;
    while (result_index >= 0) {
        if (first_index >= 0 && second_index >= 0) {
            (*result)->array[result_index] = binary_operation(first->array[first_index], second->array[second_index], action);
            second_index--;
            result_index--;
        }
        else if (first_index < 0) {
            (*result)->array[result_index] = binary_operation('0', second->array[second_index], action);
            second_index--;
            result_index--;
        }
        else if (second_index < 0) {
            (*result)->array[result_index] = binary_operation(first->array[first_index], '0', action);
            first_index--;
            result_index--;
        }
        else {
            break;
        }
    }
    return OK;
}

void inverseVector(BooleanVector *ptr) {
    for (int i = 0; i < ptr->size; i++) {
        ptr->array[i] = (ptr->array[i] == '0') ? '1' : '0';
    }
}

void printVector(FILE *stream, BooleanVector *ptr) {
    if (!ptr) {
        return;
    }
    fprintf(stream, "%c: ", ptr->name);
    for (int i = 0; i < ptr->size; i++) {
        fprintf(stream, "%c", ptr->array[i]);
    }
    fprintf(stream, "\n");
}
