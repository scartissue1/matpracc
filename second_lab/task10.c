#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

void polyderivative(double coefs[], double power, int iteration);

double polysolve(double argument, double coefs[], double power);

status_codes new_coefs(double argument, double ** coefs_result, int power, ...);

int main(int argc, char * argv[]) {
    double * coefs = NULL;
    int power = 3;
    switch (new_coefs(1, &coefs, power, 1.0, 1.0, 1.0, 1.0)) {
        case OK:
            for (int i = 0; i < power + 1; i++) {
                printf("%lf ", coefs[i]);
            }
            printf("\n");
            break;
        case NO_MEMORY:
            printf("\n");
            return -1;
        case INVALID_PARAMETER:
            printf("\n");
            return -1;
    }
    return 0;
}

status_codes new_coefs(double argument, double ** coefs_result, int power, ...) {
    if (power < 0) return INVALID_PARAMETER;
    // (*coefs_result_size) = power + 1;
    (*coefs_result) = (double *)malloc(sizeof(double) * (power + 1));
    if (!(*coefs_result)) return NO_MEMORY;
    double coefs_old[power + 1];
    va_list ptr;
    va_start(ptr, power);
    for (int i = 0; i < power + 1; i++) coefs_old[i] = va_arg(ptr, double);
    va_end(ptr);
    double multiply = 1.0;
    for (int i = 0; i < power + 1; i++) {
        (*coefs_result)[i] = polysolve(argument, coefs_old, power);
        if (i > 1) {
            multiply *= i;
            (*coefs_result)[i] /= multiply; 
        }
        polyderivative(coefs_old, power, i);

    }
    return OK;
}

double polysolve(double argument, double coefs[], double power) {
    double result = 0;
    double current_power = 1;
    for (int i = 0; i < power + 1; i++) {
        current_power *= argument;
        result += current_power * coefs[i];
    }
    return result;
}

void polyderivative(double coefs[], double power, int iteration) {
    for (int i = 0; i < power + 1; i++) coefs[i] *= i - iteration;
}