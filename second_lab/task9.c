#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>



size_t _strlen(char * string) {
    size_t size = 0;
    while (string[size++] != '\0');
    return size - 1;
}

int fraction_validation(char * number) {
    size_t size = _strlen(number);
    if (size < 3) return 0;
    if (number[0] != '0' || number[1] != '.') return 0;
    for (int i = 2; i < size; i++) {
        if (!isdigit(number[i])) return 0;
    }
    return 1;
}

int is_power(int number, const int base) {
    while (number > 1) {
        if (number % base != 0) return 0;
        number /= base;
    }
    return 1;
}

int greatest_common_divisor(int first, int second) {
    return second ? greatest_common_divisor(second, first % second) : first;
}

int common_fraction(char * number, int * numerator, int * denominator) {
    size_t num_size = _strlen(number);
    for (size_t i = 2; i < num_size; i++) {
        *numerator = *numerator * 10 + (number[i] - '0');
        *denominator *= 10;
    }
    int gsc = greatest_common_divisor(*numerator, *denominator);
    *numerator /= gsc;
    *denominator /= gsc;
}

char ** final_representation_in_base (size_t * result_size, const int base, int quantity, ...) {
    va_list fractions;
    va_start(fractions, quantity);
    *result_size = 0;
    size_t result_capacity = 2;
    char ** result = (char **)malloc(sizeof(char *) * result_capacity);
    for (int i = 0; i < quantity; i++) {
        int numerator = 0;
        int denominator = 1;
        char * number = va_arg(fractions, char *);
        if (!fraction_validation(number)) {
            printf("Wrong number\n");
            return NULL;
        }
        common_fraction(number, &numerator, &denominator);
        if (!is_power(denominator, base)) continue;
        (*result_size)++;
        result[(*result_size) - 1] = number;
        if ((*result_size) >= result_capacity) {
            result_capacity *= 2;
            char ** tmp = (char **)realloc(result, result_capacity);
            if (!tmp) {
                free(result);
                printf("No memory\n");
                return NULL;
            }
            result = tmp;
        }
    }
    va_end(fractions);
    return result;
}

int main(int argc, char * argv[]) {
    size_t final_rep_size = 0;
    char ** final_rep = final_representation_in_base(&final_rep_size, 2, 5, "0.00001", "0.0002", "0.125", "0.25", "0.5");
    if (final_rep) {
        for (size_t i = 0; i < final_rep_size; i++) {
            printf("%s\n", final_rep[i]);
        }
        free(final_rep);
    }
    return 0;
}