#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>

#include "status_codes.h"

typedef struct Stack {
    void *head;
    struct Stack *tail;
} Stack;

status_codes pushStack(Stack **ptr, void *data);

status_codes popStack(Stack **ptr, void **data);

void freeStack(Stack *ptr);

#endif 