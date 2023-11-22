#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "status_codes.h"
#include "stack.h"

int binary_pow(int base, int power);

int isOperator(const char symbol);

int getPriority(const char operator);

status_codes to_postfix(char **postfix, const char *infix);

status_codes postfix_compute(long long *result, const char *postfix);

#endif