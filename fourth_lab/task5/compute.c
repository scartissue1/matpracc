#include "compute.h"

int binary_pow(int base, int power) {
    if (power == 0) {
        return 1;
    }
    if (power % 2 == 1) {
        return binary_pow(base, power - 1) * base;
    }
    else {
        int tmp = binary_pow(base, power / 2);
        return tmp * tmp;
    }
}

int isOperator(const char symbol) {
    return ((symbol == '(') || (symbol == '+') || (symbol == '*') || (symbol == '/') || (symbol == '-') ||(symbol == '%') || (symbol == '^') || (symbol == ')') || (symbol == '~'));
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
        case '^':
            return 3;
        case '~':
            return 2;
        default:
            return -1;
    }
}

status_codes to_postfix(char **postfix, const char *infix) {
    size_t infix_size = strlen(infix);
    (*postfix) = (char *)malloc(sizeof(char) * infix_size);
    if (!(*postfix)) {
        return NO_MEMORY;
    }
    size_t postfix_size = 0;
    Stack *operators = NULL;
    for (int i = 0; i < infix_size; i++) {
        if (infix[i] == ' ' || infix[i] == '\t' || infix[i] == '\n') {
            continue;
        }
        if (!isOperator(infix[i])) {
            (*postfix)[postfix_size] = infix[i];
            postfix_size++;
            if (isOperator(infix[i + 1]) || infix[i + 1] == ' ' || infix[i + 1] == '\t') {
                (*postfix)[postfix_size] = ' ';
                postfix_size++;
            }
        }
        else if (infix[i] == ')') {
            while (1) {
                long long from_stack;
                if (popStack(&operators, &from_stack) == INVALID_PARAMETER) {
                    free(*postfix);
                    freeStack(operators);
                    return INVALID_BRACKET;
                }
                if (from_stack == '(') {
                    break;
                }
                (*postfix)[postfix_size] = from_stack;
                postfix_size++;
            }
        }
        else if (infix[i] == '(') {
            int to_stack = infix[i];
            if (pushStack(&operators, to_stack) == NO_MEMORY) {
                freeStack(operators);
                free(*postfix);
                return NO_MEMORY;
            }
        }
        else {
            char copy = infix[i];
            if (copy == '-') {
                int binary = 0;
                for (int j = i - 1; j >= 0; j--) {
                    if (isOperator(infix[j])) {
                        break;
                    }
                    if (isdigit(infix[j])) {
                        binary = 1;
                        break;
                    }
                }
                if (!binary) {
                    copy = '~';
                }
            }
            int copy_priority = getPriority(copy);
            while (operators && (getPriority(operators->head) > copy_priority)) {
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
            return INVALID_BRACKET;
        }
        (*postfix)[postfix_size] = from_stack;
        postfix_size++;

    }
    (*postfix)[postfix_size] = '\0';
    freeStack(operators);
    return OK;
}

status_codes postfix_compute(long long *result, const char *postfix) {
    Stack *operands = NULL;
    size_t postfix_size = strlen(postfix);
    if (!postfix || !postfix_size) {
        return ZERO_SIZE;
    }
    size_t number_size = 0;
    size_t number_capacity = 2;
    char *number = (char *)malloc(sizeof(char) * number_capacity);
    if (!number) {
        freeStack(operands);
        return NO_MEMORY;
    }
    for (int i = 0; i < postfix_size; i++) {
        if (postfix[i] == ' ' || postfix[i] == '\t' || postfix[i] == '\n') {
            continue;
        }
        if (!isdigit(postfix[i]) && !isOperator(postfix[i])) {
            free(number);
            freeStack(operands);
            return NOT_DIGIT_OR_OPERATOR;
        }
        if (isdigit(postfix[i])) {
            while (i < postfix_size && (isdigit(postfix[i]))) {
                number[number_size] = postfix[i];
                number_size++;
                if (number_size >= number_capacity) {
                    number_capacity *= 2;
                    char *tmp = NULL;
                    if (!(tmp = (char *)realloc(number, sizeof(char) * number_capacity))) {
                        free(number);
                        freeStack(operands);
                        return NO_MEMORY;
                    }
                    number = tmp;
                }
                i++;
            }
            i--;
            number[number_size] = '\0';
            if (pushStack(&operands, atoll(number)) == NO_MEMORY) {
                free(number);
                freeStack(operands);
                return NO_MEMORY;
            }
            char *tmp = NULL;
            number_size = 0;
            number_capacity = 2;
            if (!(tmp = (char *)realloc(number, sizeof(char) * number_capacity))) {
                free(number);
                freeStack(operands);
                return NO_MEMORY;
            }
            number = tmp;
        }
        else {
            long long first_value, second_value;
            if (popStack(&operands, &first_value) == INVALID_PARAMETER) {
                free(number);
                freeStack(operands);
                return UNUSED_DIGITS_OR_OPERATORS;
            }
            long long res = 0;
            if (postfix[i] == '~') {
                res = -first_value;
            }
            else {
                if (popStack(&operands, &second_value) == INVALID_PARAMETER) {
                    free(number);
                    freeStack(operands);
                    return UNUSED_DIGITS_OR_OPERATORS;
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
                            free(number);
                            freeStack(operands);
                            return DIVIDE_BY_ZERO;
                        }
                        res = second_value / first_value;
                        break;
                    case '%':
                        if (!first_value) {
                            free(number);
                            freeStack(operands);
                            return DIVIDE_BY_ZERO;
                        }
                        if (first_value < 0 || second_value < 0) {
                            free(number);
                            freeStack(operands);
                            return NEGATIVE_MOD;
                        }
                        res = second_value % first_value;
                        break;
                    case '^':
                        if (first_value < 0) {
                            free(number);
                            freeStack(operands);
                            return NEGATIVE_POWER;
                        }
                        if (!first_value && !second_value) {
                            free(number);
                            freeStack(operands);
                            return NEGATIVE_POWER;
                        }
                        res = binary_pow(second_value, first_value);
                        break;
                    default:
                        free(number);
                        freeStack(operands);
                        return INVALID_PARAMETER;
                }
            }
            if (pushStack(&operands, res) == NO_MEMORY) {
                free(number);
                freeStack(operands);
                return NO_MEMORY;
            }
        }
    }
    if (popStack(&operands, result) == INVALID_PARAMETER) {
        free(number);
        freeStack(operands);
        return UNUSED_DIGITS_OR_OPERATORS;
    }
    if (operands) {
        freeStack(operands);
        free(number);
        return UNUSED_DIGITS_OR_OPERATORS;
    }
    free(number);
    return OK;
}

