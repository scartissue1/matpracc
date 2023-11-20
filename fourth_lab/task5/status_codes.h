#ifndef __ST_CODES_H__
#define __ST_CODES_H__

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    OK,
    NO_MEMORY,
    NO_FILE,
    INVALID_PARAMETER,
    INVALID_BRACKET,
    ZERO_SIZE,
    DIVIDE_BY_ZERO,
    NEGATIVE_MOD,
    NEGATIVE_POWER,
    NOT_DIGIT_OR_OPERATOR,
    UNUSED_DIGITS_OR_OPERATORS
} status_codes;

#endif