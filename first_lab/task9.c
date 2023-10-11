#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define SIZE 7

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

int comp (const int * num1, const int * num2) { return *num1 - *num2; }

void min_max_swapper (int array[SIZE], int left, int right) {
    int min_value = right + 1, max_value = left - 1, min_index = 0, max_index = 0;
    
    for (int i = 0; i < SIZE; i++) {
        if (array[i] > max_value) {
            max_value = array[i];
            max_index = i;
        }
        if (array[i] < min_value) {
            min_value = array[i];
            min_index = i;
        }
    }
    array[min_index] = max_value;
    array[max_index] = min_value;
}

int binary_nearest_search (int to_find, int array_size, int array[array_size]) {
    int left = 0, right = array_size, gap = 1001;
    while (left < right) {
        int m = (left + right) / 2;
        if (m + 1 < array_size && m - 1 >= 0) {
            if (fabs(array[m - 1] - to_find) < gap && fabs(array[m - 1] - to_find) < fabs(array[m + 1] - to_find)
                && fabs(array[m - 1] - to_find) < fabs(array[m] - to_find)) {
                right = m - 1;
                gap = fabs(array[m - 1] - to_find);
            }
            else if (fabs(array[m + 1] - to_find) < gap && fabs(array[m + 1] - to_find) < fabs(array[m - 1] - to_find)
                && fabs(array[m + 1] - to_find) < fabs(array[m] - to_find)) {
                left = m + 1;
                gap = fabs(array[m + 1] - to_find);
            }
            else {
                gap = fabs(array[m] - to_find);
                break;
            }
        }
        else if (!(m + 1 < array_size)) {
            if (fabs(array[m - 1] - to_find) < gap && fabs(array[m - 1] - to_find) < fabs(array[m] - to_find)) {
                right = m - 1;
                gap = fabs(array[m - 1] - to_find);
            }
            else { 
                gap = fabs(array[m] - to_find);
                break;
            }
        }
        else if (!(m - 1 >= 0)) {
            if (fabs(array[m + 1] - to_find) < gap && fabs(array[m + 1] - to_find) < fabs(array[m] - to_find)) {
                left = m + 1;
                gap = fabs(array[m + 1] - to_find);
            }
            else {
                gap = fabs(array[m] - to_find);
                break;
            }
        }
    }
    return gap;
}

void array_of_nearest (int array_A_size, int array_B_size,
    int array_A[array_A_size], int array_B[array_B_size], int array_C[array_A_size]) {
    qsort(array_B, array_B_size, sizeof(int), (int(*)(const void *, const void *))comp);
    for (int i = 0; i < array_A_size; i++) {
        array_C[i] = array_A[i] + binary_nearest_search(array_A[i], array_B_size, array_B);
    }
}

int main(int argc, char * argv[]) {
    if (argc == 3) {
        int left = atoi(argv[1]);
        int right = atoi(argv[2]);
        
        if (!num_validation(argv[1]) || !num_validation(argv[2])) {
            printf("NaN\n");
            return -1;
        }
        int array[SIZE];

        for (int i = 0; i < SIZE; i++) {
            array[i] = rand() % (right - left) + left;
            printf("%d ", array[i]);
        }
        printf("\n");

        min_max_swapper(array, left, right);

        for (int i = 0; i < SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        int array_A_size = rand() % (10000 - 10) + 10;
        int array_B_size = rand() % (10000 - 10) + 10;
        int array_A[array_A_size];
        for (int i = 0; i < array_A_size; i++) {
            array_A[i] = rand() % 2000 - 1000; 
        }
        int array_B[array_B_size];
        for (int i = 0; i < array_B_size; i++) {
            array_B[i] = rand() % 2000 - 1000; 
        }
        int array_C[array_A_size];
        
        array_of_nearest(array_A_size, array_B_size, array_A, array_B, array_C);
        
        printf("A:\n");
        for (int i = 0; i < array_A_size; i++) {
            printf("%d ", array_A[i]);
        }
        printf("\n");
        printf("B:\n");
        for (int i = 0; i < array_B_size; i++) {
            printf("%d ", array_B[i]);
        }
        printf("\n");
        printf("C:\n");
        for (int i = 0; i < array_A_size; i++) {
            printf("%d ", array_C[i]);
        }
        printf("\n");

    }
    else {
        printf("Wrong flag\n");
        return -1;
    }
    return 0;
}