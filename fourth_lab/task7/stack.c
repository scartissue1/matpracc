#include "stack.h"

status_codes pushStack(Stack **ptr, long long data) {
    Stack *tmp = (Stack *)malloc(sizeof(Stack));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->head = data;
    tmp->tail = (*ptr);
    (*ptr) = tmp;
    return OK;
}

status_codes popStack(Stack **ptr, long long *data) {
    if (!(*ptr)) {
        return INVALID_PARAMETER;
    }
    Stack *tmp = (*ptr);
    (*ptr) = tmp->tail;
    (*data) = tmp->head;
    free(tmp);
    return OK;
}

void freeStack(Stack *ptr) {
    if (!ptr) {
        return;
    }
    freeStack(ptr->tail);
    free(ptr);
}