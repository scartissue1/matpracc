#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

bool eps_validation (char * eps) {
    int eps_length = strlen(eps);
    bool fl = false;
    for (int i = 0; i < eps_length; i++) {
        if (isdigit(eps[i]) || eps[i] == 'e' || eps[i] == '.'|| eps[i] == '-') fl = true;
        else { fl = false; break; }
    }
    return fl;
}

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
        double value = atof(argv[1]);
        double eps = atof(argv[2]);

        switch (eps_validation(argv[2])) {
            case true:
                printf("a. %lf\n", first_row(value, eps));
                printf("b. %lf\n", second_row(value, eps));
                printf("c. %lf\n", third_row(value, eps));
                printf("d. %lf\n", fourth_row(value, eps));
                break;
            case false:
                break;
        }
        
        
    }
    else printf("Wrong flag!\n");
    return 0;
}
