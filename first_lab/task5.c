#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int factorial (int number) {
    if (number == 0 || number == 1) return 1;
    else if (number > 1) return number * factorial(number - 1);
    else return -1;
}

int two_factorial (int number) {
    if (number == 0 || number == 1) return 1;
    else if (number > 1) return number * two_factorial(number - 2);
    else return -1;
}

double first_row (double value, double eps) {
    double n_arg = 0.0;
    double prev = 0.0;
    double summ = pow(value, n_arg) / (double)factorial((int)n_arg);
    while (fabs(summ - prev) > eps) {
        n_arg++;
        prev = summ;
        summ += pow(value, n_arg) / (double)factorial((int)n_arg);
    }
    return summ;
}

double second_row (double value, double eps) {
    double n_arg = 0.0;
    double prev = 0.0;
    double summ = (pow(-1, n_arg) * pow(value, 2 * n_arg)) / ((double)factorial(2 * n_arg));
    while (fabs(summ - prev) > eps) {
        n_arg++;
        prev = summ;
        summ += (pow(-1, n_arg) * pow(value, 2 * n_arg)) / ((double)factorial(2 * n_arg));
    }
    return summ;
}

double third_row (double value, double eps) {
    double n_arg = 0.0;
    double prev = 0.0;
    double summ = (pow(3.0, 3.0 * n_arg) * pow((double)factorial(n_arg), 3.0) * (pow(value, 2.0 * n_arg))) / ((double)factorial(3.0 * n_arg));
    while (fabs(summ - prev) > eps) {
        n_arg++;
        prev = summ;
        summ += (pow(3.0, 3.0 * n_arg) * pow((double)factorial(n_arg), 3.0) * (pow(value, 2.0 * n_arg))) / ((double)factorial(3.0 * n_arg));
    }
    return summ;
}

double fourth_row (double value, double eps) {
    double n_arg = 1.0;
    double prev = 0.0;
    double summ = (pow(-1.0, n_arg) * (double)two_factorial(2 * n_arg - 1) * pow(value, 2 * n_arg)) / ((double)two_factorial(2 * n_arg));
    while (fabs(summ - prev) > eps) {
        n_arg++;
        prev = summ;
        summ += (pow(-1.0, n_arg) * (double)two_factorial(2 * n_arg - 1) * pow(value, 2 * n_arg)) / ((double)two_factorial(2 * n_arg));
    }
    return summ;
}

int main (int argc, char * argv[]) {
    if (argc == 3) {
        char * ptr;
        double value = strtod(argv[1], &ptr);
        double eps = strtod(argv[2], &ptr);

        printf("a. %lf\n", first_row(value, eps));
        printf("b. %lf\n", second_row(value, eps));
        printf("c. %lf\n", third_row(value, eps));
        printf("d. %lf\n", fourth_row(value, eps));
        
    }
    else printf("Wrong flag!\n");
    return 0;
}