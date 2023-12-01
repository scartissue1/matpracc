#ifndef POLY_H__
#define POLY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER,
} status_codes;

typedef struct Polynomial {
    int coefficient;
    int power;
    struct Polynomial * next;
} Polynomial;

void printPolynomial(Polynomial * head, int first_flag);

void freePolynomial(Polynomial * head);

status_codes push_forwardPolynomial(Polynomial ** head, int _coefficient, int _power);

status_codes push_backPolynomial(Polynomial * head, int _coefficient, int _power);

status_codes add(Polynomial ** result, Polynomial * first, Polynomial * second);

status_codes sub(Polynomial ** result, Polynomial * first, Polynomial * second);

status_codes copyPolynomial(Polynomial ** dest, Polynomial * src);

status_codes mult(Polynomial ** result, Polynomial * first, Polynomial * second);

void multconst(Polynomial * result, const int coef);

status_codes divmod(Polynomial ** div_result, Polynomial ** mod_result, Polynomial * first, Polynomial * second);

double binary_pow(double base, int power);

void eval(double *result, Polynomial * poly, const double value);

status_codes cmps(Polynomial ** result, Polynomial * first, Polynomial * second);

void diff(Polynomial * poly);

int isZero(Polynomial *poly);

#endif