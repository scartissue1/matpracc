#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool eps_validation (char const * number) {
    const int size = strlen(number);
    int index = 0, e_count = 0, _count = 0, dot_count = 0;
    bool fl = false;
    while (index < size) {
        if (number[index] == 'e') e_count++;
        if (number[index] == '-') _count++;
        if (number[index] == '.') dot_count++;
        if (e_count > 1 || _count > 1 || dot_count > 1) return false;
        if (isdigit(number[index])) fl = true;
        index++;
    }
    return (true && fl);
}

double dichotomy (double (*func)(double argument), double left, double right, double eps) {
    if (left > right) return left + 1.0;
    if (func(left) * func(right) >= 0) return left - 1.0;
    double argument = 0.0;
    while (func(right) - func(left) > eps) {
        argument = (left + right) / 2.0;
        if (func(left) * func(argument) < 0) right = argument;
        else left = argument;
    }
    return argument;
}

double euler_equation (double x) { return (cos(x) + 1); }

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    double eps = atof(argv[1]);
    if (!eps_validation(argv[1])) return -1;
    double left = 3.0;
    double right = 4.0;
    double eq_answer = dichotomy(euler_equation, 3.0, 4.0, eps);
    if (eq_answer == left + 1.0) printf("left > right\n");
    else if (eq_answer == left - 1.0) printf("f(left) * f(right) >= 0\n");
    else printf("%lf\n", eq_answer);
    return 0;
}