#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum multiple_by_status_codes {
    mbsc_ok,
    mbsc_no_number_found,
    mbsc_overflow,
    mbsc_underflow
};

enum multiple_by_status_codes multiple_by (int number, int * result, int result_length) {
    int to_hundred = number;
    int flag = 0, i = 0;

    if (number < 0) return mbsc_underflow;
    while (to_hundred <= 100) {
        if (i == result_length) return mbsc_overflow;
        result[i] = to_hundred;
        i++;
        to_hundred += number;
        flag = 1;
    }

    if (!flag) return mbsc_no_number_found;
    else return mbsc_ok;
}

enum is_prime_status_codes {
    ipsc_one,
    ipsc_underflow,
    ipsc_true,
    ipsc_false
};

enum is_prime_status_codes is_prime (int number) {
    if (number <= 0) return ipsc_underflow;
    if (number == 1) return ipsc_one;

    int prime_checker = 2;
    while (prime_checker < (int)sqrt(number) + 1) {
        if (number % prime_checker == 0 && is_prime(prime_checker)) {
            return ipsc_false;
        }
        prime_checker++;
    }
    return ipsc_true;
}

int digits_count (int number) {
    int digits_count = 0, check = number;
    while (check != 0) { 
        digits_count++;
        check /= 10;
    }
    return digits_count;
}
enum number_split_status_codes {
    nssc_ok,
    nssc_overflow,
    nssc_invalid_parameter
};

enum number_split_status_codes number_split(int number, int * result, int digits) {
    if (number < 0) return nssc_invalid_parameter;
    int index = 0;
    while (digits) {
        int splitter = 10, splitter_power = 1;
        while (splitter_power < digits) {
            splitter *= 10;
            splitter_power++;
        }
        if (index == digits) return nssc_overflow;
        result[index] = number % splitter / (splitter / 10);
        index++;
        digits--;
    }
    return nssc_ok;
}

enum power_table_status_codes {
    ptsc_ok,
    ptsc_invalid_parameter,
    ptsc_overflow
};

enum power_table_status_codes power_table(int number, int ** result) {
    if (1 <= number && number <= 10) {
        for (int base = 1; base < 11; base++) {
            int start_base = base;
            for (int power = 1; power < number; power++) {
                start_base *= base;
                if (base - 1< 10 && power - 1 < number) result[base - 1][power - 1] = start_base;
                else return ptsc_overflow;
            }
        }
        return ptsc_ok;
    }
    else return ptsc_invalid_parameter;
}

enum sum_to_the_number_status_codes {
    sttnsc_ok,
    sttnsc_invalid_parameter
};

enum sum_to_the_number_status_codes sum_to_the_number (int number, long long int * result) {
    if (number < 0) return sttnsc_invalid_parameter;
    
    *result = ((1 + number) / 2) * number;
    return sttnsc_ok;
}

enum factorial_status_codes {
    fsc_ok,
    fsc_invalid_parameter
};

enum factorial_status_codes factorial (int number, long long int * result) {
    if (number == 0 || number == 1) { 
        *result = 1;
        return fsc_ok;
    }
    else if (number > 1) {
        enum factorial_status_codes recursive_status_code;
        recursive_status_code = factorial(number - 1, result);
        if (recursive_status_code == fsc_ok) *result *= number;
        return recursive_status_code;
    }
    else return fsc_invalid_parameter;
}

bool is_num (char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (!isdigit(string[i])) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int validation_flag = 1;
    if (argc == 3 && is_num(argv[1])) {
        int number = atoi(argv[1]);
        if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'h')  { 
            int result_length = 100 / number;
            int * multipliers_result = (int*)malloc(sizeof(int) * result_length);                
            switch (multiple_by(number, multipliers_result, result_length)) {
                case mbsc_ok: 
                    for (int i = 0; i < result_length; i++) {
                        printf("%d ", multipliers_result[i]);
                    }
                    printf("\n");
                    break;
                case mbsc_no_number_found:
                    printf("No number in [1, 100] can be multiplied by %d\n", number);
                    break;
                case mbsc_overflow:
                    printf("%d is not enough to contain multipliers of %d\n", result_length, number);
                    break;
                case mbsc_underflow:
                    printf("Number must be positive\n");
                    break;
            }
            
            }    
        else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'p') {
            switch (is_prime(number)) {
                case ipsc_true: printf("%d is prime\n", number); break;
                case ipsc_false: printf("%d is not prime\n", number); break;
                case ipsc_underflow: printf("Number have to be positive\n"); break;
                case ipsc_one: printf("1 is prime and not prime\n"); break;
                    
            }
        }

        else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 's') {
            int d_count = digits_count(number);
            int * splitter_result = (int*)malloc(sizeof(int) * d_count);
            switch(number_split(number, splitter_result, d_count)) { 
                case nssc_ok:
                    for (int i = 0; i < d_count; i++) {
                        printf("%d ", splitter_result[i]);
                    }
                    printf("\n");
                    break;
                case nssc_overflow:
                    printf("%d is not enough to contain digits of %d\n", d_count, number);
                    break;
                case nssc_invalid_parameter:
                    printf("Invalid parameter\n");
                    break;
            }
        }
        else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'e')  {
            int ** power_table_result = (int**)malloc(sizeof(int*) * 10);
            for (int i = 0; i < 10; i++) {
                power_table_result[i] = (int*)malloc(sizeof(int) * number);
            }
            switch (power_table(number, power_table_result)) {
                case ptsc_ok:
                    for (int i = 1; i <= 10; i++) {
                        printf("%d | ", i);
                        for (int j = 1; j < number; j++) {
                            printf("%d| ", power_table_result[i - 1][j - 1]);
                        }
                        printf("\n");
                    }
                    break;
                case ptsc_invalid_parameter:
                    printf("Invalid parameter\n");
                    break;
                case ptsc_overflow:
                    printf("Overflow\n");
                    break;
            }
        }
        else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'a') {
            long long int sum_result = 0;
            switch(sum_to_the_number(number, &sum_result)) {
                case sttnsc_ok:
                    printf("%lld\n", sum_result);
                    break;
                case sttnsc_invalid_parameter:
                    printf("Invalid parameter\n");
                    break;
            }
        }

        else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'f') {
            long long int factorial_result;
            switch(factorial(number, &factorial_result)) {
                case fsc_ok:
                    printf("%lld\n", factorial_result);
                    break;
                case fsc_invalid_parameter:
                    printf("Invalid parameter\n");
                    break;
            }
        }
            
        else validation_flag = 0;
        
    }
    else validation_flag = 0;
    if (!validation_flag) printf("Wrong flag!\n");

    return 0;
}
