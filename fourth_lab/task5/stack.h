#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>

#include "status_codes.h"

typedef struct Stack {
    long long head;
    struct Stack *tail;
} Stack;

status_codes pushStack(Stack **ptr, const long long data);

status_codes popStack(Stack **ptr, long long *out);

void freeStack(Stack *ptr);

#endif