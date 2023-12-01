#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>

#include "task.h"

typedef struct Stack {
    long long head;
    struct Stack *tail;
} Stack;

status_codes pushStack(Stack **ptr, long long data);

status_codes popStack(Stack **ptr, long long *data);

void freeStack(Stack *ptr);

#endif