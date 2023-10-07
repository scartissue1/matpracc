#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

bool num_validation (char const * number) {
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

int comp (const double * num1, const double * num2) { return *num1 - *num2; }

void swap (double coefficients[3], int first, int second) {
    double tmp = coefficients[first];
    coefficients[first] = coefficients[second];
    coefficients[second] = tmp;
}

bool permutations (double coefficients[3]) {
    int first_index = 1;
    while (first_index != -1 && coefficients[first_index] >= coefficients[first_index + 1]) first_index--;
    if (first_index == -1) return false;
    int second_index = 2;
    while (coefficients[first_index] >= coefficients[second_index]) second_index--;
    swap(coefficients, first_index, second_index);
    int left = first_index + 1, right = 2;
    while (left < right) swap (coefficients, left++, right--);
    return true;
}

void quadratic_equation (double coefficients[3], double eps, double result[2], int *result_flag) {
    double discriminant = coefficients[1] * coefficients[1] - 4 * coefficients[0] * coefficients[2];
    if (discriminant > eps) {
        result[0] = (-coefficients[1] + sqrt(discriminant)) / 2 * coefficients[0];
        result[1] = (-coefficients[1] - sqrt(discriminant)) / 2 * coefficients[0];
        *result_flag = 2;
    }
    else if (fabs(discriminant) < eps) {
        result[0] = -coefficients[1] / 2 * coefficients[0];
        result[1] = 0.0;
        *result_flag = 1;
    }
    else {
        result[0] = 0.0, result[1] = 0.0;
        *result_flag = 0;
    }
}

void print_result (double coefficients[3], double result[2], int *result_flag) {
    printf("For equation %lf * x^2 + %lf * x + %lf = 0\n", coefficients[0], coefficients[1], coefficients[2]);
    switch (*result_flag) {
        case 2:
            printf("Solutions are x1 = %lf, x2 = %lf\n", result[0], result[1]);
            break;
        case 1:
            printf("Solution is x = %lf\n", result[0]);
            break;
        case 0:
            printf("No solutions\n");
            break;
    }
}

bool multiple_checker (int first_number, int second_number) {
    return first_number % second_number == 0;
}

bool triangle_checker (double sides[3], double eps) {
    if (sides[0] > sides[1] && sides[0] > sides[2]) {
        return (fabs(sides[1] * sides[1] + sides[2] * sides[2] - sides[0] * sides[0]) < eps);
    }
    else if (sides[1] > sides[2] && sides[1] > sides[0]) {
        return (fabs(sides[0] * sides[0] + sides[2] * sides[2] - sides[1] * sides[1]) < eps);
    }
    else return (fabs(sides[0] * sides[0] + sides[1] * sides[1] - sides[2] * sides[2]) < eps);
}

int main (int argc, char * argv[]) {
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'q' && argc == 6) {
            double eps = atof(argv[2]);
            double coefficients[3];
            coefficients[0] = atof(argv[3]);
            coefficients[1] = atof(argv[4]);
            coefficients[2] = atof(argv[5]);
            if (!(num_validation(argv[2]) || num_validation(argv[3]) || num_validation(argv[4]) || num_validation(argv[5]))) {
                printf("Wrong number\n");
                return -1;
            }
            int result_flag = 0;
            double result[2];
            result[0] = 0.0, result[1] = 0.0;
            
            qsort(coefficients, 3, sizeof(double), (int(*)(const void *, const void *))comp);
            quadratic_equation(coefficients, eps, result, &result_flag);
            print_result(coefficients, result, &result_flag);
            while (permutations(coefficients)) {
                quadratic_equation(coefficients, eps, result, &result_flag);
                print_result(coefficients, result, &result_flag);
            }
        }
        else if (argv[1][1] == 'm' && argc == 4) {
            int first_number = atoi(argv[2]);
            int second_number = atoi(argv[3]);

            if (!(num_validation(argv[2]) || num_validation(argv[3]))) {
                printf("Wrong number\n");
                return -1;
            }

            if (first_number != 0 && second_number != 0) {
                if (multiple_checker(first_number, second_number))
                    printf("%d is a multiple of %d\n", first_number, second_number);
                else printf("%d is not a multiple of %d\n", first_number, second_number);
            }
            else printf("One (or two) number(s) is 0\n");
        }

        else if (argv[1][1] == 't' && argc == 6) {
            double eps = atof(argv[2]);
            double sides[3];
            sides[0] = atof(argv[3]);
            sides[1] = atof(argv[4]);
            sides[2] = atof(argv[5]);

            if (!(num_validation(argv[2]) || num_validation(argv[3]) || num_validation(argv[4]) || num_validation(argv[5]))) {
                printf("Wrong number\n");
                return -1;
            }

            if (sides[0] > eps && sides[1] > eps && sides[2] > eps) {
                if (triangle_checker(sides, eps)) { printf("Can be a triangle\n"); }
                else printf("Can't be a triangle\n");
            }
            else printf("Every number have to be positive\n");
            
        }
        else printf("Wrong flag\n");
    }
    else printf("Wrong flag\n");
    return 0;
} 
