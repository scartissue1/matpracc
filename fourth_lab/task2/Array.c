#include "Array.h"

status_codes initArray(Array **arr, const char _name) {
    (*arr) = (Array *)malloc(sizeof(Array));
    if (!(*arr)) {
        return NO_MEMORY;
    }
    (*arr)->name = _name;
    (*arr)->size = 0;
    (*arr)->capacity = 2;
    (*arr)->data = (int *)malloc(sizeof(int) * (*arr)->capacity);
    if (!(*arr)->data) {
        free(*arr);
        return NO_MEMORY;
    }
    return OK;
}

status_codes push_backArray(Array *arr, const int number) {
    arr->data[arr->size] = number;
    arr->size++;
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        int *tmp = NULL;
        if (!(tmp = (int *)realloc(arr->data, sizeof(int) * arr->capacity))) {
            return NO_MEMORY;
        }
        arr->data = tmp;
    }
    return OK;
}

status_codes loadArray(Array *arr, FILE *stream) {  
    char symbol = fgetc(stream);
    size_t size = 0;
    size_t capacity = 2;
    char *string = (char *)malloc(sizeof(char) * capacity);
    if (!string) {
        return NO_MEMORY;
    }
    while (symbol != EOF) {
        if (symbol != ' ' && symbol != '\t' && symbol != '\n' && symbol != EOF) {
            string[size] = symbol;
            size++;
            if (size >= capacity) {
                capacity *= 2;
                char *tmp = NULL;
                if (!(tmp = (char *)realloc(string, sizeof(char) * capacity))) {
                    free(string);
                    return NO_MEMORY;
                }
                string = tmp;
            }
        }
        else {
            string[size] = '\0';
            if (!int_validation(string)) {
                free(string);
                return INVALID_INPUT;
            }
            int new_number = atoi(string);
            if (push_backArray(arr, new_number) == NO_MEMORY) {
                free(string);
                return NO_MEMORY;
            }
            size = 0;
            capacity = 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc(string, sizeof(char) * capacity))) {
                free(string);
                return NO_MEMORY;
            }
            string = tmp;
        }
        symbol = fgetc(stream);
    }
    string[size] = '\0';
    if (!int_validation(string)) {
        return INVALID_INPUT;
    }
    int new_number = atoi(string);
    if (push_backArray(arr, new_number) == NO_MEMORY) {
        free(string);
        return NO_MEMORY;
    }
    free(string);
    return OK;
}

void saveArray(Array *arr, FILE *stream) {
    for (int i = 0; i < arr->size; i++) {
        fprintf(stream, "%d ", arr->data[i]);
    }
}

status_codes randArray(Array *arr, const int count, const int left_bound, const int right_bound) {
    arr->size = count;
    arr->capacity = arr->size;
    if (!arr->data) {
        arr->data = (int *)malloc(sizeof(int) * arr->capacity);
        if (!arr->data) {
            return NO_MEMORY;
        }
    }
    for (int i = 0; i < arr->size; i++) {
        arr->data[i] = rand() % (right_bound - left_bound + 1) + left_bound;
    } 
    return OK;
}

status_codes concatArray(Array *dest, Array *src) {
    dest->capacity += src->capacity;
    int *tmp = (int *)realloc(dest->data, sizeof(int) * dest->capacity);
    if (!tmp) {
        return NO_MEMORY;
    }
    dest->data = tmp;
    for (int i = dest->size; i < src->size + dest->size; i++) {
        dest->data[i] = src->data[i - dest->size];
    }
    dest->size += src->size;
    return OK;
}

void freeArray(Array *arr) {
    arr->size = 0;
    arr->capacity = 2;
}

void removeArray(Array *arr, const int start, const int count) {
    for (int i = start; i < start + count; i++) {
        arr->data[i] = 0;
    }
    for (int i = start + count; i < arr->size; i++) {
        arr->data[i - count] = arr->data[i];
        arr->data[i] = 0;
    }
    arr->size -= count;
}

status_codes copyArray(Array *src, const int start, const int end, Array *dest) {
    for (int i = start; i <= end; i++) {
        if (push_backArray(dest, src->data[i]) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

void sort_increaseArray(Array *arr) {
    qsort(arr->data, arr->size, sizeof(int), (int(*)(const void *, const void *))comp_increase);
}

void sort_decreaseArray(Array *arr) {
    qsort(arr->data, arr->size, sizeof(int), (int(*)(const void *, const void *))comp_decrease);
}

void shuffleArray(Array *arr) {
    qsort(arr->data, arr->size, sizeof(int), (int(*)(const void *, const void *))comp_random);
}

void statsArray(Array *arr) {
    int max = arr->data[0], max_index = 0;
    int min = arr->data[0], min_index = 0;
    int max_count_elem = arr->data[0], max_count = 1;
    int summ = 0;
    for (int i = 0; i < arr->size; i++) {
        if (max < arr->data[i]) {
            max = arr->data[i];
            max_index = i;
        }
        if (min > arr->data[i]) {
            min = arr->data[i];
            min_index = i;
        }
        summ += arr->data[i];
        int curr_count = 0;
        for (int j = i + 1; j < arr->size; j++) {
            if (arr->data[i] == arr->data[j]) {
                curr_count++;
            }
        } 
        if (curr_count > max_count || (curr_count == max_count && arr->data[i] > max_count_elem)) {
            max_count_elem = arr->data[i];
            max_count = curr_count;
        }
    }
    double avg = (double)summ / (double)arr->size;
    double max_deviation = -1.0;
    for (int i = 0; i < arr->size; i++) {
        if (fabs(arr->data[i] - avg) > max_deviation) {
            max_deviation = fabs(arr->data[i] - avg);
        }
    }
    printf("___________________________________\n");
    printf("\t%c stat\n", arr->name);
    printf("Max elem is %d in index %d\n", max, max_index);
    printf("Min elem is %d in index %d\n", min, min_index);
    printf("Array average is %f\n", avg);
    printf("Max deviation from average is %f\n", max_deviation);
    printf("___________________________________\n");
}

void printArray(Array *arr, const int start, const int end) {
    printf("%c: ", arr->name);
    for (int i = start; i <= end; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}