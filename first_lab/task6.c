#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

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

double first_integral (double eps) {
    int n_arg = 2;
    double prev = 0;
    double curr = 1;
    int sign = -1;

    while (fabs(curr - prev) > eps) {
        n_arg++;
        prev = curr;
        curr += sign * (1.0 / (double)(n_arg * n_arg));
        sign = -sign;
    }
    return curr;
}

double second_integral (double eps) {
    int n_arg = 0;
    double prev = 0;
    double summ = pow(-1.0, n_arg) * (pow(1.0, (double)(2 * n_arg + 1))) / (pow(2.0, (double)n_arg) * (double)factorial(n_arg) * (double)(2 * n_arg + 1));

    while (fabs(summ - prev) > eps) {
        n_arg++;
        prev = summ;
        summ += pow(-1.0, n_arg) * (pow(1.0, (double)(2 * n_arg + 1))) / (pow(2.0, (double)n_arg) * (double)factorial(n_arg) * (double)(2 * n_arg + 1));
    }
    return summ;
}

double third_integral (double eps) {
    int n_arg = 1;
    double prev = 0;
    double curr = 1.0 / (n_arg * n_arg + n_arg);

    while (fabs(curr - prev) > eps) {
        n_arg++;
        prev = curr;
        curr += 1.0 / (n_arg * n_arg + n_arg);
    }
    return curr;
}

// https://en.wikipedia.org/wiki/Sophomore%27s_dream
double fourth_integral (double eps) {
    int n_arg = 1;
    double prev = 0;
    double summ = pow(-n_arg, (double)-n_arg);
    while (fabs(summ - prev) > eps) {
        n_arg++;
        prev = summ;
        summ += pow(-n_arg, (double)-n_arg);
    }
    return -summ;
}

int main (int argc, char * argv[]) {
    if (argc == 2) {
        double eps = atof(argv[1]);
        switch (eps_validation(argv[1])) {
            case true:
                printf("a. %lf\n", first_integral(eps));
                printf("b. %lf\n", second_integral(eps));
                printf("c. %lf\n", third_integral(eps));
                printf("d. %lf\n", fourth_integral(eps));
                break;
            case false:
                printf("Wrong epsilon\n");
                break;
        }
    }
    else printf("Wrong flag\n");
    
    return 0;
}