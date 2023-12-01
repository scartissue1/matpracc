#include "compute.h"

int isOperator(const char symbol) {
    return ((symbol == '(') || (symbol == ')') || (symbol == '+') || (symbol == '-') || (symbol == '/') || (symbol == '*') || (symbol == '%'));
}

int getPriority(const char operator) {
    switch (operator) {
        case '(':
            return 0;
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        case '%':
            return 2;
        case '~':
            return 2;
        default:
            return -1;
    }
}

status_codes getPostfix(char **postfix, const char *infix) {
    size_t infix_size = strlen(infix);
    (*postfix) = (char *)malloc(sizeof(char) * infix_size);
    if (!(*postfix)) {
        return NO_MEMORY;
    }
    size_t postfix_size = 0;
    Stack *operators = NULL;
    int bracket_level = 0;
    int val_in_bracket = 0;
    for (int i = 0; i < infix_size; i++) {
        if (!isOperator(infix[i])) {
            (*postfix)[postfix_size] = infix[i];
            postfix_size++;
            if (isOperator(infix[i + 1])) {
                (*postfix)[postfix_size] = ' ';
                postfix_size++;
            }
            if (bracket_level) {
                val_in_bracket = 1;
            }
            else {
                val_in_bracket = 1;
            }
        }
        else if (infix[i] == ')') {
            int got_an_operator = 0;
            bracket_level--;
            while (1) {
                long long from_stack;
                if (popStack(&operators, &from_stack) == INVALID_PARAMETER) {
                    free(*postfix);
                    freeStack(operators);
                    return UNEQUAL_BRACKET;
                }
                if (from_stack == '(') {
                    break;
                }
                got_an_operator = 1;
                (*postfix)[postfix_size] = from_stack;
                postfix_size++;
            }
            if (!got_an_operator && !val_in_bracket) {
                free(*postfix);
                freeStack(operators);
                return EMPTY_BRACKET;
            }
        }
        else if (infix[i] == '(') {
            int to_stack = infix[i];
            if (pushStack(&operators, to_stack) == NO_MEMORY) {
                freeStack(operators);
                free(*postfix);
                return NO_MEMORY;
            }
            bracket_level++;
        }
        else {
            char copy = infix[i];
            if (copy == '-') {
                int binary = 0;
                for (int j = i - 1; j >= 0; j--) {
                    if (isOperator(infix[j]) && infix[j] != ')') {
                        break;
                    }
                    if (isdigit(infix[j]) || isalpha(infix[j])) {
                        binary = 1;
                        break;
                    }
                }
                if (!binary) {
                    copy = '~';
                }
            }
            int copy_priority = getPriority(copy);
            while (operators && (getPriority(operators->head) >= copy_priority)) {
                long long from_stack;
                if (popStack(&operators, &from_stack) == INVALID_PARAMETER) {
                    break;
                }
                (*postfix)[postfix_size] = from_stack;
                postfix_size++;
            }
            int to_stack = copy;
            if (pushStack(&operators, to_stack) == NO_MEMORY) {
                freeStack(operators);
                free(*postfix);
                return NO_MEMORY;
            }
        }
    }
    long long from_stack;
    while (popStack(&operators, &from_stack) != INVALID_PARAMETER) {
        if (from_stack == '(') {
            freeStack(operators);
            free(*postfix);
            return UNEQUAL_BRACKET;
        }
        (*postfix)[postfix_size] = from_stack;
        postfix_size++;
    }
    (*postfix)[postfix_size] = '\0';
    return OK;
}

status_codes computePostfix(long long *result, const char *postfix, ArrayCell array, size_t array_size) {
    Stack *operands = NULL;
    size_t postfix_size = strlen(postfix);
    if (!postfix || !postfix_size) {
        return ZERO_SIZE;
    }
    size_t var_size = 0;
    size_t var_capacity = 2;
    char *var = (char *)malloc(sizeof(char) * var_capacity);
    if (!var) {
        return NO_MEMORY;
    }
    for (int i = 0; i < postfix_size; i++) {
        if (postfix[i] == ' ') {
            continue;
        }
        if (!isdigit(postfix[i]) && !isalpha(postfix[i]) && !isOperator(postfix[i]) && postfix[i] != ' ') {
            
            free(var);
            freeStack(operands);
            return INVALID_SYMBOL;
        }
        if (isdigit(postfix[i]) || isalpha(postfix[i])) {
            while (i < postfix_size && (isdigit(postfix[i]) || isalpha(postfix[i]))) {
                var[var_size] = postfix[i];
                var_size++;
                if (var_size >= var_capacity) {
                    var_capacity *= 2;
                    char *tmp = NULL;
                    if (!(tmp = (char *)realloc(var, sizeof(char) * var_capacity))) {
                        free(var);
                        freeStack(operands);
                        return NO_MEMORY;
                    }
                    var = tmp;
                }
                i++;
            }
            i--;
            var[var_size] = '\0';
            if (int_validation(var)) {
                if (pushStack(&operands, atoll(var)) == NO_MEMORY) {
                    free(var);
                    freeStack(operands);
                    return NO_MEMORY;
                }
            }
            else {
                if (!name_validation(var)) {
                    free(var);
                    freeStack(operands);
                    return INVALID_NAME;
                }
                int index = binarySearch(array, array_size, var);
                if (index == -1) {
                    free(var);
                    freeStack(operands);
                    return UNINITIALIZED_VALUE;
                }
                if (pushStack(&operands, array[index]->value) == NO_MEMORY) {
                    free(var);
                    freeStack(operands);
                    return NO_MEMORY;
                }
            }
            char *tmp = NULL;
            var_size = 0;
            var_capacity = 2;
            if (!(tmp = (char *)realloc(var, sizeof(char) * var_capacity))) {
                free(var);
                freeStack(operands);
                return NO_MEMORY;
            }
            var = tmp;
        }
        else {
            long long first_value, second_value;
            if (popStack(&operands, &first_value) == INVALID_PARAMETER) {
                free(var);
                freeStack(operands);
                return UNUSED_VARS_OR_OPERATORS;
            }
            long long res = 0;
            if (postfix[i] == '~') {
                res = -first_value;
            }
            else {
                if (popStack(&operands, &second_value) == INVALID_PARAMETER) {
                    free(var);
                    freeStack(operands);
                    return UNUSED_VARS_OR_OPERATORS;
                }
                switch (postfix[i]) {
                    case '~':
                        break;
                    case '+':
                        res = second_value + first_value;
                        break;
                    case '-':
                        res = second_value - first_value;
                        break;
                    case '*':
                        res = second_value * first_value;
                        break;
                    case '/':
                        if (!first_value) {
                            free(var);
                            freeStack(operands);
                            return DIVIDE_BY_ZERO;
                        }
                        res = second_value / first_value;
                        break;
                    case '%':
                        if (!first_value) {
                            free(var);
                            freeStack(operands);
                            return DIVIDE_BY_ZERO;
                        }
                        if (first_value < 0 || second_value < 0) {
                            free(var);
                            freeStack(operands);
                            return NEGATIVE_MOD;
                        }
                        res = second_value % first_value;
                        break;
                    default:
                        free(var);
                        freeStack(operands);
                        return INVALID_SYMBOL;
                }
            }
            if (pushStack(&operands, res) == NO_MEMORY) {
                free(var);
                freeStack(operands);
                return NO_MEMORY;
            }
        }
    }
    if (popStack(&operands, result) == INVALID_PARAMETER) {
        free(var);
        freeStack(operands);
        return UNUSED_VARS_OR_OPERATORS;
    }
    if (operands) {
        freeStack(operands);
        free(var);
        return UNUSED_VARS_OR_OPERATORS;
    }
    free(var);
    return OK;
}