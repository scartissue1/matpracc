#ifndef __TASK_H__
#define __TASK_H__

#include <math.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER,
    NO_FILE
} status_codes;

int isPrime(int number);

int getPrime(int number);

#endif