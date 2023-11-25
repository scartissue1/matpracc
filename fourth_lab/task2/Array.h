#ifndef ARRAY_H__
#define ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "task.h"

typedef struct {
    char name;
    int *data;
    size_t size;
    size_t capacity;
} Array;

status_codes initArray(Array **arr, char _name);

status_codes push_backArray(Array *arr, int number);

status_codes loadArray(Array *arr, FILE *stream);

void saveArray(Array *arr, FILE *stream);

status_codes randArray(Array *arr, int count, int left_bound, int right_bound);

status_codes concatArray(Array *dest, Array *src);

status_codes freeArray(Array *arr);

void removeArray(Array *arr, int start, int count);

status_codes copyArray(Array *src, int start, int end, Array *dest);

void sort_increaseArray(Array *arr);

void sort_decreaseArray(Array *arr);

void shuffleArray(Array *arr);

void statsArray(Array *arr);

void printArray(Array *arr, int start, int end);

#endif