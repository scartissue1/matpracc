#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>

#include "status_codes.h"

typedef struct Stack {
    int head;
    struct Stack *tail;
} Stack;

status_codes pushStack(Stack **ptr, const int data);

status_codes popStack(Stack **ptr, int *out);

void freeStack(Stack *ptr);

#endif