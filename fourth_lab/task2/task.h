#ifndef ST_CODES_H__
#define ST_CODES_H__

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    OK,
    NO_MEMORY,
    NO_FILE,
    OUT_OF_BOUNDS,
    INVALID_INPUT
} status_codes;

int int_validation(const char * number);

int comp_increase(int *first, int *second);

int comp_decrease(int *first, int *second);

int comp_random(int *first, int *second);

#endif