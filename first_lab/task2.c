#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

bool is_prime (int number) {
    int prime_checker = 2;
    while (prime_checker < (int)sqrt(number) + 1) {
        if (number % prime_checker == 0 && is_prime(prime_checker)) {
            return false;
        }
        prime_checker++;
    }
    return true;
}

double dichotomy (double (*func)(double argument), double left, double right, double eps) {
    double argument = 0.0;
    while (!(func(right) - func(left) < eps)) {
        argument = (left + right) / 2.0;
        if (func(left) * func(argument) < 0) right = argument;
        else left = argument;
    }
    return argument;
}

double newton_method (double (*func)(double argument), double (*func_derivative)(double argument), 
    double left, double right, double eps) {
    
    double x_curr = (left + right) / 2;
    double x_prev = 0;

    while (fabs(x_curr - x_prev) > eps) {
        x_prev = x_curr;
        x_curr -= func(x_curr) / func_derivative(x_curr);
    }

    return x_curr;
    }

int factorial (int number) {
    if (number == 0 || number == 1) return 1;
    else if (number > 1) return number * factorial(number - 1);
    else return -1;
}

double my_pow_double (const double base, const int power) {
    double result = base;
    for (int i = 0; i < power; i++) {
        result *= base;
    }
    return result;
}

int my_pow_int (const int base, const int power) {
    int result = base;
    for (int i = 0; i < power; i++) {
        result *= base;
    }
    return result;
}

double euler_limit (double eps) {
    double n_arg = 1.0;
    double prev = 0.0;
    double curr = pow(1.0 + (1.0/n_arg), n_arg);
    while (fabs(curr - prev) > eps) {
        n_arg++;
        prev = curr;
        curr = pow(1.0 + (1.0/n_arg), n_arg);
    }
    return curr;
}

double euler_summ (double eps) {
    int n_arg = 0;
    double prev = 0.0;
    double curr = 1.0 / (double)factorial(n_arg);
    while (fabs(curr - prev) > eps) {
        n_arg++;
        prev = curr;
        curr += 1.0 / (double)factorial(n_arg);
    }
    return curr;
}

double euler_equation (double x) { return (log(x) - 1); }

double pi_limit (double eps) {
    double n_arg = 0.0;
    double prev = 0.0;
    double curr = 4.0;
    while (fabs(curr - prev) > eps) {
        n_arg++;
        prev = curr;
        curr *= (4 * n_arg * (n_arg + 1)) / pow(2 * n_arg + 1, 2);
    }
    return curr;
}

double pi_summ (double eps) {
    int n_arg = 1;
    double prev = 0;
    double curr = (double)((pow(-1, n_arg - 1)) / (2 * n_arg - 1));
    while (fabs(curr - prev) > eps) {
        n_arg++;
        prev = curr;
        curr += (double)((pow(-1, n_arg - 1)) / (2 * n_arg - 1));
    }
    return 4 * curr;
}

double pi_equation (double x) { return cos(x) + 1; }

double pi_equation_derivative (double x) { return -sin(x); }

double log_2_limit (double eps) {
    double n_arg = 1.0;
    double prev = 0;
    double curr = n_arg * (pow(2, (double)(1.0 / n_arg)) - 1);
    while (fabs(prev - curr) > eps) {
        n_arg++;
        prev = curr;
        curr = n_arg * (pow(2, 1 / n_arg) - 1);
    }
    return curr;
}

double log_2_summ (double eps) {
    double n_arg = 1.0;
    double prev = 0;
    double curr = pow(-1.0, n_arg - 1) / n_arg;
    while (fabs(prev - curr) > eps) {
        n_arg++;
        prev = curr;
        curr += pow(-1.0, n_arg - 1) / n_arg;
    }
    return curr;
}

double log_2_equation (double x) { return exp(x) - 2; }

double sqrt_2_limit (double eps) {
    double prev = -0.5;
    double curr = prev - ((prev * prev)/2.0) + 1.0;
    while (fabs(prev - curr) > eps) {
        prev = curr;
        curr = prev - ((prev * prev)/2.0) + 1.0;
    }
    return curr;
}

double sqrt_2_row (double eps) {
    double k_arg = 2.0;
    double prev = 0.0;
    double curr = pow(2, pow(2, -k_arg));
    while (fabs(curr - prev) > eps) {
        k_arg++;
        prev = curr;
        curr *= pow(2, pow(2, -k_arg));
    }
    return curr;
}

double sqrt_2_equation(double x)  { return x * x - 2; }

double gamma_limit (double eps) {
    int m_arg = 1;
    double curr = 0.0, prev;
    do {
        m_arg++;
        prev = curr;
        curr = 0.0;
        double logarithm = 0.0, combinations_count = m_arg, sum = 0.0;
        for (int k_arg = 2; k_arg <= m_arg; k_arg++) {
            combinations_count *= (m_arg - k_arg + 1.0) / k_arg;
            logarithm += log(k_arg);
            sum = combinations_count * logarithm / k_arg;
            if (k_arg % 2 == 1) sum *= -1;
            curr += sum;
        } 
    } while (fabs(curr - prev) > eps);
    return curr;
}

double gamma_summ (double eps) {
    double pi = pi_summ(eps);
    double k_arg = 2.0;
    double curr = -(pi * pi / 6.0), prev, tmp = 1.0;
    int sqrt_flag = 4;
    int flag_for_sqrt = 5;
    do {
        if (fabs(k_arg - sqrt_flag) < eps) {
            tmp = sqrt_flag;
            sqrt_flag += flag_for_sqrt;
            flag_for_sqrt += 2;
            k_arg++;
            continue;
        }
        prev = 1.0 / tmp - 1.0 / k_arg;
        curr += prev;
        k_arg++;
    } while (fabs(prev) > eps);
    return curr;
}

double gamma_equation (double eps) {
    int t_arg = 2;
    double prod = 0.5;
    double prev = 0.0;
    double curr = -log(log(2) * 0.5);
    
    do {
        t_arg++;
        prev = curr;
        if (is_prime(t_arg)) prod *= (t_arg - 1.0) / (double)t_arg;
        curr = -log(prod * log(t_arg));

    } while (fabs(curr - prev) > eps);

    return curr;
}

int main(int argc, char * argv[]) {
    if (argc == 2) {
        double eps = atof(argv[1]);
        switch (eps_validation(argv[1])) {
            case true:
                printf("Euler by limit: %lf\n", euler_limit(eps));
                printf("Euler by row: %lf\n", euler_summ(eps));
                printf("Euler by equation: %lf\n", dichotomy(euler_equation, 2.0, 3.0, eps));
                printf("\n");
                printf("Pi by limit: %lf\n", pi_limit(eps));
                printf("Pi by row: %lf\n", pi_summ(eps));
                printf("Pi by equation: %lf\n", newton_method(pi_equation, pi_equation_derivative, 3.0, 4.0, eps));
                printf("\n");
                printf("ln 2 by limit: %lf\n", log_2_limit(eps));
                printf("ln 2 by row: %lf\n", log_2_summ(eps));
                printf("ln 2 by equation: %lf\n", dichotomy(log_2_equation, 0.0, 1.0, eps));
                printf("\n");
                printf("sqrt(2) by limit: %lf\n", sqrt_2_limit(eps));
                printf("sqrt(2) by row: %lf\n", sqrt_2_row(eps));
                printf("sqrt(2) by equation: %lf\n", dichotomy(sqrt_2_equation, 1.0, 2.0, eps));
                printf("\n");
                printf("gamma by limit: %lf\n", gamma_limit(eps));
                printf("gamma by row: %lf\n", gamma_summ(eps));
                printf("gamma by equation: %lf\n", gamma_equation(eps));
                break;
            case false:
                printf("Wrong epsilon\n");
                break;
        }
        
    }
    else printf("Wrong flag!\n");
    return 0;
}