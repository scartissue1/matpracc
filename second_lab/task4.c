#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define EPS 1e-5

int sign(double x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

typedef struct point {
    double x;
    double y;
} point;

enum is_convex_status_codes {
    CONVEX,
    NOT_CONVEX,
    NOT_ENOUGH_POINTS
};

enum is_convex_status_codes is_convex(int quantity, ...) {
    quantity *= 2;
    va_list points;
    va_start(points, quantity);
    if (quantity < 6) return NOT_ENOUGH_POINTS;
    point prev;
    prev.x = va_arg(points, double);
    prev.y = va_arg(points, double);
    point first = prev;
    point curr;
    curr.x = va_arg(points, double);
    curr.y = va_arg(points, double);
    int prev_direction = 0, curr_direction;
    int flag = 0;
    point first_vector;
    point second_vector;
    for (int i = 0; i < (quantity - 4) / 2; i++) {
        point next;
        next.x = va_arg(points, double);
        next.y = va_arg(points, double);
        first_vector.x = curr.x - prev.x;
        first_vector.y = curr.y - prev.y;
        second_vector.x = next.x - curr.x;
        second_vector.y = next.y - curr.y;
        curr_direction = sign(first_vector.x * second_vector.y - second_vector.x * first_vector.y);
        if (curr_direction != 0) flag = 1;
        if (prev_direction != 0 && curr_direction != prev_direction) return NOT_CONVEX;
        prev_direction = curr_direction;
        prev = curr;
        curr = next;
    }
    va_end(points);
    first_vector.x = curr.x - prev.x;
    first_vector.y = curr.y - prev.y;
    second_vector.x = first.x - curr.x;
    second_vector.y = first.y - curr.y;
    curr_direction = sign(first_vector.x * second_vector.y - second_vector.x * first_vector.y);
    if (curr_direction != 0) flag = 1;
    if (prev_direction != 0 && curr_direction != prev_direction) return NOT_CONVEX;
    if (flag) return CONVEX;
    else return NOT_CONVEX;
}

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


double polynomial(double argument, int size, ...) {
    int power = size;
    size = abs(size) + 1;
    va_list ptr;
    va_start(ptr, size);
    double result = 0.0;
    if (power >= 0) {
        for (int i = 0; i < size; i++) result += va_arg(ptr, double) * binary_pow(argument, power--);
    }
    else if (power < 0) {
        for (int i = 0; i < size; i++) result += va_arg(ptr, double) * binary_pow(argument, power++);
    }
    va_end(ptr);
    return result;
}

int main(int argc, char * argv[]) {
    switch (is_convex(4, 1.0, 2.0, 1.0, 1.0, 2.0, 0.0, -1.0, 2.0)) {
        case CONVEX:
            printf("Is convex\n");
            break;
        case NOT_CONVEX:
            printf("Is not convex\n");
            break;
        case NOT_ENOUGH_POINTS:
            printf("Not enough points\n");
            break;
    }
    printf("%lf\n", polynomial(2.0, -3, 1.0, 2.0, 3.0, 4.0));
    return 0;
}