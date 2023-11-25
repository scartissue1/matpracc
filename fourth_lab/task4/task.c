#include "task.h"

status_codes getInteger(int *result, const unsigned char *number, const int base) {
    size_t number_size = strlen(number);
    if (!number || !strlen(number)) {
        return INVALID_PARAMETER;
    }
    int power = 1;
    for (int i = number_size; i >= 0; i --) { 
        (*result) += power * ((isdigit(number[i])) ? number[i] - '0' : 
                    ('A' <= number[i] && number[i] <= 'Z') ? number[i] - 'A' + 10 : 
                    number[i] - 'a' + 10);
        power *= base;
    }
    return OK;
}
status_codes getBase(unsigned char **result, int number, const int base) {
    if (*result) {
        return INVALID_PARAMETER;
    }
    int sign_flag = 0;
    size_t result_size = log(number) / log(base) + 1;
    (*result) = (unsigned char *)malloc(sizeof(unsigned char));
    if (!(*result)) {
        return NO_MEMORY;
    }
    (*result)[result_size] = '\0';
    int index = result_size - 1;
    while (number) {
        (*result)[index--] = (number % base) + '0';
        number /= base;
    }
    return OK;
}

unsigned char negation(const unsigned char number) {
    return (number == '0') ? '1' : '0';
}
unsigned char binary_operation(const unsigned char _1, const unsigned char _2, const binary_actions action) {
    switch (action) {
        case disjunction:
            return _1 | _2;
        case conjuction:
            return _1 & _2;
        case implication:
            return _1 <= _2;
        case reverse_implication:
            return _1 >= _2;
        case equivalence:
            return _1 == _2;
        case xor:
            return _1 != _2;
        case coimplication:
            return _1 > _2;
        case schaeffer:
            return negation(_1 & _2);
        case pierce:
            return negation(_1 | _2);
    }
}

status_codes binary_actionProcessing(Node **root, FILE *trace_stream, const char result_name, const char first_name, const char second_name) {
    BooleanVector *first = NULL;
    BooleanVector *second = NULL;
    findNode((*root), &first, first_name);
    findNode((*root), &second, second_name);
    if (!first || !second) {
        fclose(trace_stream);
        freeNode((*root));
        return  UNINITIALIZED_VALUE;
    }
    printVector(trace_stream, first);
    printVector(trace_stream, second);
    

}
