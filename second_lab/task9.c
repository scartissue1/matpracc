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

int double_length(double number, const double EPS) {
    double tmp = number;
    int number_size = 0;
    while (fabs((int)tmp - tmp) > EPS) {
        tmp *= 10;
        number_size++;
    }
    return number_size;
    
}

int is_dividing(int number, int base) {
    return (number % base == 0 || base % number == 0);
}

int greatest_common_divisor(int first, int second) {
    return second ? greatest_common_divisor(second, first % second) : first;
}

void common_fraction(double number, int * numerator, int * denominator, const double EPS) {
    int size = double_length(number, EPS);
    int power = 1;
    for (int i = 0; i < size; i++) {
        power *= 10;
        *numerator = *numerator * 10 + ((int)(number * power) % 10);
        *denominator *= 10;
    }
    int gsc = greatest_common_divisor(*numerator, *denominator);
    *numerator /= gsc;
    *denominator /= gsc;
}

double * final_representation_in_base (const double EPS, size_t * result_size, const int base, int quantity, ...) {
    va_list fractions;
    va_start(fractions, quantity);
    *result_size = 0;
    double * result = (double *)malloc(sizeof(double) * quantity);
    for (int i = 0; i < quantity; i++) {
        int numerator = 0;
        int denominator = 1;
        double dnumber = va_arg(fractions, double);
        if (dnumber > 1 - EPS || dnumber < EPS) return NULL;
        common_fraction(dnumber, &numerator, &denominator, EPS);
        if (!is_dividing(denominator, base)) continue;
        (*result_size)++;
        result[(*result_size) - 1] = dnumber;
    }
    va_end(fractions);
    return result;
}

int main(int argc, char * argv[]) {
    size_t final_rep_size = 0;
    const double EPS = 1e-10;
    double * final_rep = final_representation_in_base(EPS, &final_rep_size, 8, 5, 0.00001, 0.0002, 0.125, 0.25, 0.5);
    if (final_rep) {
        if (final_rep_size) {
            for (size_t i = 0; i < final_rep_size; i++) {
                printf("%lf\n", final_rep[i]);
            }
        }
        else printf("No such number\n");
        free(final_rep);
    }
    
    return 0;
}
