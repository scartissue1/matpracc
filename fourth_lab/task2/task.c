#include "task.h"

int int_validation(const char *number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    if (!isdigit(number[0]) && number[0] != '-') return 0;
    for (size_t i = 1; i < size; i++) {
        if (!isdigit(number[i])) return 0;
    }
    return 1;
}

int comp_increase(int *first, int *second) {
    return *first - *second;
}

int comp_decrease(int *first, int *second) {
    return *second - *first;
}

int comp_random(int *first, int *second) {
    return rand() % 3 - 1;
}