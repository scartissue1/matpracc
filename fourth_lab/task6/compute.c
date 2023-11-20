#include "compute.h"

int getPriority(const char operation) {
    if (operation == '~') {
        return 3;
    }
    if (operation == '?' || operation == '!' || operation == '+' || operation == '&') {
        return 2;
    }
    if (operation == '|' || operation == '-' || operation == '<' || operation == '=') {
        return 1;
    }
    return -1;
}

status_codes getTree(Node **root, char *infix) {
    size_t size = strlen(infix);
    char *postfix = (char *)malloc(sizeof(char) * size);
    if (!postfix) {
        return NO_MEMORY;
    }
    size_t postfix_size = 0;
    Stack *operators = NULL;
    for (int i = 0; i < size; i++) {
        if (infix[i] == ' ' || infix[i] == '\n' || infix[i] == '\t') {
            continue;
        }
        if (isalpha(infix[i]) || infix[i] == '0' || infix[i] == '1') {
            postfix[postfix_size] = infix[i];
            postfix_size++;
        }
        else {
            if (infix[i] == ')') {
                while (1) {
                    void *ptr = NULL;
                    if (popStack(&operators, &ptr) == INVALID_PARAMETER) {
                        free(postfix);
                        freeStack(operators);
                        return INVALID_BRACKET;
                    }
                    char from_stack = *(char *)ptr;
                    if (from_stack == '(') {
                        break;
                    }
                    postfix[postfix_size] = from_stack;
                    postfix_size++;
                }
            }
            else if (infix[i] == '('){
                if (pushStack(&operators, &infix[i]) == NO_MEMORY) {
                    free(postfix);
                    freeStack(operators);
                    return NO_MEMORY;
                }
            }
            else {
                if (infix[i] == '+' || infix[i] == '-' || infix[i] == '<') {
                    if (infix[i + 1] != '>') {
                        return INVALID_PARAMETER;
                    }
                }
                if (infix[i] == '>') {
                    if (infix[i - 1] != '+' && infix[i - 1] != '-' && infix[i - 1] != '<') {
                        return INVALID_PARAMETER;
                    }
                    continue;
                }
                int current_priority = getPriority(infix[i]);
                if (current_priority == -1) {
                    freeStack(operators);
                    free(postfix);
                    return INVALID_PARAMETER;
                }
                while (operators && getPriority(*(char *)(operators->head)) > current_priority) {
                    void *ptr = NULL;
                    if (popStack(&operators, &ptr) == INVALID_PARAMETER) {
                        break;
                    }
                    char from_stack = *(char *)ptr;
                    postfix[postfix_size] = from_stack;
                    postfix_size++;
                }
                if (pushStack(&operators, &infix[i]) == NO_MEMORY) {
                    freeStack(operators);
                    free(postfix);
                    return NO_MEMORY;
                }

            }
        }

    }
    void *ptr = NULL;
    while (popStack(&operators, &ptr) != INVALID_PARAMETER) {
        char from_stack = *(char *)ptr;
        if (from_stack == '(') {
            freeStack(operators);
            free(postfix);
            return INVALID_BRACKET;
        }
        postfix[postfix_size] = from_stack;
        postfix_size++;
    }
    postfix[postfix_size] = '\0';
    freeStack(operators);
    Stack *nodes = NULL;
    for (int i = 0; i < postfix_size; i++) {
        if (isalpha(postfix[i]) || postfix[i] == '0' || postfix[i] == '1') {
            Node *new = NULL;
            if (createNode(&new, postfix[i], NULL, NULL) == NO_MEMORY) {
                freeStack(nodes);
                free(postfix);
                return NO_MEMORY;
            }
            if (pushStack(&nodes, new) == NO_MEMORY) {
                freeStack(nodes);
                free(postfix);
                return NO_MEMORY;
            }
        }
        else {
            void *ptr = NULL;
            if (popStack(&nodes, &ptr) == INVALID_PARAMETER) {
                freeStack(nodes);
                free(postfix);
                return UNUSED_OPERATORS_OR_OPERANDS;
            }
            Node *first_operand = (Node *)ptr;
            Node *res = NULL;
            if (postfix[i] == '~') {
                if (createNode(&res, postfix[i], first_operand, NULL) == NO_MEMORY) {
                    freeStack(nodes);
                    free(postfix);
                    return NO_MEMORY;
                }
            }
            else {
                void *ptr = NULL;
                if (popStack(&nodes, &ptr) == INVALID_PARAMETER) {
                    freeStack(nodes);
                    free(postfix);
                    return UNUSED_OPERATORS_OR_OPERANDS;
                }
                Node *second_operand = (Node *)ptr;
                if (createNode(&res, postfix[i], second_operand, first_operand) == NO_MEMORY) {
                    freeStack(nodes);
                    free(postfix);
                    return NO_MEMORY;
                }
            }
            if (pushStack(&nodes, res) == NO_MEMORY) {
                freeStack(nodes);
                free(postfix);
                return NO_MEMORY;
            }
        }
    }
    ptr = NULL;
    if (popStack(&nodes, &ptr) == INVALID_PARAMETER) {
        freeStack(nodes);
        free(postfix);
        return UNUSED_OPERATORS_OR_OPERANDS;
    }
    (*root) = (Node *)ptr;
    if (nodes) {
        freeStack(nodes);
        free(postfix);
        return UNUSED_OPERATORS_OR_OPERANDS;
    }
    free(postfix);
    return OK;
}

int computeTree(Node *root, const char *vars, const int *values, const int vars_count) {
    if (!root) {
        return 0;
    }
    int left = computeTree(root->left, vars, values, vars_count);
    int right = computeTree(root->right, vars, values, vars_count);
    if (isalpha(root->data)) {
        for (int i = 0; i < vars_count; i++) {
            if (root->data == vars[i]) {
                return values[i];
            }
        }
    }
    if (root->data == '0' || root->data == '1') {
        return root->data - '0';
    }
    switch (root->data) {
        case '&':
            return left & right;
        case '|':
            return left | right;
        case '~':
            return ~left;
        case '-':
            return ~left | right;
        case '+':
            return ~(~left | right);
        case '<':
            return left != right;
        case '=':
            return left == right;
        case '!':
            return ~(left & right);
        case '?':
            return ~(left | right);
    }
}