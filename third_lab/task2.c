#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

typedef enum {
    OK,
    INVALID_PARAMETER,
    NO_MEMORY
} status_codes;

typedef struct {
    size_t size;
    double * coords;
} Vector;

status_codes init_vector(Vector * new, size_t new_size, ...);

void print_vector(Vector vec);

void delete_vector(Vector vec);

double binary_pow (double base, int power);

double limit_norm(Vector vec);

double gelder_norm(Vector vec, int p_arg);

double scalar_norm(Vector vec);

status_codes got_the_norm(Vector *** result, Vector new_vector, double new_vector_norm, double * norm_max, size_t * norm_result_size, size_t * norm_result_capacity, int norm_flag) {
    double eps = 1e-20;
    if (new_vector_norm - (*norm_max) > eps) {
        (*norm_result_size) = 1;
        (*result)[norm_flag][0] = new_vector;
        (*norm_max) = new_vector_norm;
    }
    else if (fabs(new_vector_norm - (*norm_max)) < eps) {
        (*result)[norm_flag][(*norm_result_size)] = new_vector;
        (*norm_result_size)++;
        if ((*norm_result_size) >= (*norm_result_capacity)) {
            (*norm_result_capacity) *= 2;
            Vector * norm_tmp = (Vector *)realloc((*result)[norm_flag], sizeof(Vector) * (*norm_result_capacity));
            if (!norm_tmp) {
                for (int i = 0; i < 3; i++) free((*result)[i]);
                free(*result);
                return NO_MEMORY;
            }
            (*result)[norm_flag] = norm_tmp;
        }
    }
    return OK;
}

status_codes max_norms(Vector *** result, size_t * limit_norm_result_size, size_t * gelder_norm_result_size, size_t * scalar_norm_result_size, size_t dimension, 
                    double (*limit_norm)(Vector vec), double (*gelder_norm)(Vector vec, int p_arg), double (*scalar_norm)(Vector vec), int quantity, ...) {
    (*result) = (Vector **)malloc(sizeof(Vector *) * 3);
    if (!(*result)) return NO_MEMORY;
    
    size_t limit_norm_result_capacity = 2;
    size_t gelder_norm_result_capacity = 2;
    size_t scalar_norm_result_capacity = 2;

    (*limit_norm_result_size) = 0;
    (*gelder_norm_result_size) = 0;
    (*scalar_norm_result_size) = 0;

    double limit_norm_max = -1.0;
    double gelder_norm_max = -1.0;
    double scalar_norm_max = -1.0;

    for (int i = 0; i < 3; i++) {
        (*result)[i] = (Vector *)malloc(sizeof(Vector) * 2);
        if (!(*result)[i]) {
            for (int j = i; j >= 0; j--) free((*result)[j]);
            free(*result);
            return NO_MEMORY;
        }
    }

    va_list vectors;
    va_start(vectors, quantity);
    for (int i = 0; i < quantity; i++) {
        Vector new_vector = va_arg(vectors, Vector);
        if (new_vector.size != dimension) {
            for (int i = 0; i < 3; i++) free((*result)[i]);
            free(*result);
            return INVALID_PARAMETER;
        }
        double new_vector_limit_norm = limit_norm(new_vector);
        if (got_the_norm(result, new_vector, new_vector_limit_norm, &limit_norm_max, limit_norm_result_size, &limit_norm_result_capacity, 0) == NO_MEMORY) {
            va_end(vectors);
            return NO_MEMORY;
        }
        double new_vector_gelder_norm = gelder_norm(new_vector, 3);
        if (got_the_norm(result, new_vector, new_vector_gelder_norm, &gelder_norm_max, gelder_norm_result_size, &gelder_norm_result_capacity, 1) == NO_MEMORY) {
            va_end(vectors);
            return NO_MEMORY;
        }
        double new_vector_scalar_norm = scalar_norm(new_vector);
        if (got_the_norm(result, new_vector, new_vector_scalar_norm, &scalar_norm_max, scalar_norm_result_size, &scalar_norm_result_capacity, 2) == NO_MEMORY) {
            va_end(vectors);
            return NO_MEMORY;
        }
    }
    va_end(vectors);
    return OK;
}

int main(int argc, char * argv[]) {
    Vector first, second, third;
    init_vector(&first, 3, 1.0, 2.0, 3.0);
    init_vector(&second, 3, 1.0, 1.0, 1.0);
    init_vector(&third, 3, 1.0, 2.0, 3.0);
    
    Vector ** norm_maximums = NULL;
    size_t limit_norm_size = 0, gelder_norm_size = 0, scalar_norm_size = 0;

    switch (max_norms(&norm_maximums, &limit_norm_size, &gelder_norm_size, &scalar_norm_size, 3, limit_norm, gelder_norm, scalar_norm, 3, first, second, third)) {
        case OK:
            printf("limit norm maximum(s)\n");
            for (int i = 0; i < limit_norm_size; i++) print_vector(norm_maximums[0][i]);
            printf("gelder norm maximum(s)\n");
            for (int i = 0; i < gelder_norm_size; i++) print_vector(norm_maximums[1][i]);
            printf("scalar norm maximum(s)\n");
            for (int i = 0; i < scalar_norm_size; i++) print_vector(norm_maximums[2][i]);
            break;
        case NO_MEMORY:
            delete_vector(first);
            delete_vector(second);
            delete_vector(third);
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            delete_vector(first);
            delete_vector(second);
            delete_vector(third);
            printf("Wrong parameter detected\n");
            return -1;
    }
    delete_vector(first);
    delete_vector(second);
    delete_vector(third);
    for (int i = 0; i < 3; i++) free(norm_maximums[i]);
    free(norm_maximums);
    return 0;
}

status_codes init_vector(Vector * new, size_t new_size, ...) {
    (*new).size = new_size;
    (*new).coords = (double *)malloc(sizeof(double) * new_size);
    if (!((*new).coords)) return NO_MEMORY;
    va_list values;
    va_start(values, new_size);
    for (int i = 0; i < new_size; i++) {
        (*new).coords[i] = va_arg(values, double);
    }
    va_end(values);
    return OK;
}

void print_vector(Vector vec) {
    printf("(");
    for (int i = 0; i < vec.size; i++) {
        printf("%lf", vec.coords[i]);
        if (i != vec.size - 1) printf(", ");
    }
    printf(")\n");
}

void delete_vector(Vector vec) {
    free(vec.coords);
}

double limit_norm(Vector vec) {
    double max = fabs(vec.coords[0]);
    for (int i = 1; i < vec.size; i++) {
        if (fabs(vec.coords[i]) > max) max = fabs(vec.coords[i]);
    }
    return max;
}

double gelder_norm(Vector vec, int p_arg) {
    double result = 0;
    for (int i = 1; i < vec.size; i++) {
        result += binary_pow(fabs(vec.coords[i]), p_arg);
    }
    result = pow(result, 1.0 / p_arg);
    return result;
}

double scalar_norm(Vector vec) {
    double result = 0;
    for (int i = 0; i < vec.size; i++) {
        result += vec.coords[i] * vec.coords[i];
    }
    result = sqrt(result);
    return result;
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