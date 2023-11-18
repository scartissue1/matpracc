#include "command.h"

status_codes loadProcessing(Node **root, const char *filename, const char array_name) {
    FILE *stream = fopen(filename, "r");
    if (!stream) {
        return NO_FILE;
    }
    Array *arr = NULL;
    findNode((*root), &arr, array_name);
    int new_flag = 0;
    if (!arr) {
        if (initArray(&arr, array_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    else {
        arr->size = 0;
        arr->capacity = 2;
    }
    switch (loadArray(arr, stream)) {
        case OK:
            fclose(stream);
            break;
        case NO_MEMORY:
            fclose(stream);
            return NO_MEMORY;
        case INVALID_INPUT:
            fclose(stream);
            return INVALID_INPUT;
    }
    if (new_flag) {
        if (addNode(root, arr) == NO_MEMORY) {
            fclose(stream);
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes saveProcessing(Node *root, const char *filename, const char array_name) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return INVALID_INPUT;
    }
    FILE *stream = fopen(filename, "w");
    if (!stream) {
        return NO_FILE;
    }
    saveArray(arr, stream);
    fclose(stream);
    return OK;
}

status_codes randProcessing(Node **root, const char array_name, int count, int left_bound, int right_bound) {
    Array *arr = NULL;
    findNode((*root), &arr, array_name);
    int new_flag = 0;
    if (!arr) {
        if (initArray(&arr, array_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    else {
        arr->size = 0;
        arr->capacity = 2;
    }
    if (randArray(arr, count, left_bound, right_bound) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (new_flag) {
        if (addNode(root, arr) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes concatProcessing(Node **root, const char dest_name, const char src_name) {
    Array *dest = NULL;
    Array *src = NULL;
    findNode((*root), &dest, dest_name);
    findNode((*root), &src, src_name);
    if (!src) {
        return INVALID_INPUT;
    }
    int new_flag = 0;
    if (!dest) {
        if (initArray(&dest, dest_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    } 
    if (concatArray(dest, src) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (new_flag) {
        if (addNode(root, dest) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes freeProcessing(Node *root, const char array_name) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return INVALID_INPUT;
    }
    freeArray(arr);
    return OK;
}

status_codes removeProcessing(Node *root, const char array_name, const int start, const int count) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return INVALID_INPUT;
    }
    if (start + count >= arr->size || start < 0 || count < 0) {
        return OUT_OF_BOUNDS;
    }
    removeArray(arr, start, count);
    return OK;
}

status_codes copyProcessing(Node **root, const char src_name, const char dest_name, const int start, const int end) {
    if (end < start) {
        return INVALID_INPUT;
    }
    Array *src = NULL;
    findNode((*root), &src, src_name);
    if (!src) {
        return INVALID_INPUT;
    }
    if (start >= src->size || end >= src->size || start < 0 || end < 0) {
        return OUT_OF_BOUNDS;
    }
    Array *dest = NULL;
    findNode((*root), &dest, dest_name);
    int new_flag = 0;
    if (!dest) {
        if (initArray(&dest, dest_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    else {
        dest->size = 0;
        dest->capacity = 2;
    }
    if (copyArray(src, start, end, dest) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (new_flag) {
        if (addNode(root, dest) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes sortProcessing(Node *root, const char array_name, int sort_flag) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return INVALID_INPUT;
    }
    (sort_flag == 0) ? sort_increaseArray(arr) : (sort_flag == 1) ? sort_decreaseArray(arr) : shuffleArray(arr);
    return OK;
}

status_codes statsProcessing(Node *root, const char array_name) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return INVALID_INPUT;
    }
    statsArray(arr);
    return OK;
}

status_codes printProcessing(Node *root, const char array_name, int start, int end) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return INVALID_INPUT;
    }
    if (start == -1 && end == -1) {
        start = 0;
        end = arr->size - 1;
    }
    if (start >= arr->size || end >= arr->size || start < 0 || end < 0) {
        return OUT_OF_BOUNDS;
    }
    printArray(arr, start, end);
    return OK;
}