#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

double binary_pow (double base, int power) {
    if (power < 0) {
        base = 1.0 / base;
        power = abs(power);
    }
    if (power == 0) return 1;
    if (power % 2 == 1) return binary_pow(base, power - 1) * base;
    else {
        double tmp = binary_pow(base, power / 2);
        return tmp * tmp;
    }
}

double geometric_mean(int quantity, ...) {
    double result = 1.0;
    va_list number;
    va_start(number, quantity);
    for (int i = 0; i < quantity; i++) {
        result *= va_arg(number, double);
    }
    va_end(number);
    if (quantity % 2 == 0 && result < 0) return -1.0;
    else result = pow(result, (1.0 / (double)quantity));
    return result;
}

int main(int argc, char * argv[]) {
    printf("%lf\n", geometric_mean(6, 1.5, 2.0, 3.0, 4.0, 5.0, -1.0));
    printf("%lf\n", binary_pow(3.0, -2.0));
    return 0;
}