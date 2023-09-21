#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void multiple_by (int number) {
    int to_hundred = 0;
    int flag = 0;

    while (to_hundred < 100) {
        to_hundred += number;
        if (to_hundred <= 100) {
            printf("%d ", to_hundred);
            flag = 1;
        }
    }

    if (!flag) printf("No number in [1, 100] can be multiplied by %d", number);
    printf("\n");
}

bool is_prime (int number) {
    int prime_checker = 2;
    while (prime_checker < number / 2 + 1) {
        if (number % prime_checker == 0) return false;
        prime_checker++;
    }
    return true;
}

void number_split(int number) {
    int digits_count = 0, check = number;
    while (check != 0) { 
        digits_count++;
        check /= 10;
    }
    
    while (digits_count) {
        int splitter = 10, splitter_power = 1;
        while (splitter_power < digits_count) {
            splitter *= 10;
            splitter_power++;
        }
        printf("%d ", number % splitter / (splitter / 10));
        digits_count--;

    }
    printf("\n");
}

void power_table(int number) {
    if (1 <= number && number <= 10) {
        printf("base | powers from 1 to %d\n", number);
        for (int base = 1; base < 11; base++) {
            printf("%d |", base);
            int start_base = base;
            for (int power = 1; power < number; power++) {
                start_base *= base;
                printf("%d | ", start_base);
            }
            printf("\n");
        }
    }
    else printf("Number have to be in [1, 10]\n");
}

long long int sum_to_the_number (int number) {
    long long int sum = 0;
    int curr = 1;
    while (curr <= number) {
        sum += curr;
        curr++;
    }
    return sum;
}

long long int factorial (int number) {
    if (number == 0) return 1;
    else if (number > 0) return number * factorial(number - 1);
    else return -1;
}

int main(int argc, char* argv[]) {
    int validation_flag = 1;
    if (argc == 3 && strlen(argv[2])) {
        int number = atoi(argv[1]);
        if (number) {
            if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'h') multiple_by(number);

            else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'p') {
                if (is_prime(number)) printf("%d is prime \n", number);
                else printf("%d is not prime \n", number);
            }

            else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 's') number_split(number);

            else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'e') power_table(number);

            else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'a') printf("%lld\n", sum_to_the_number(number));

            else if ((argv[2][0] == '-' || argv[2][0] == '/') && argv[2][1] == 'f') printf("%lld\n", factorial(number));
            else validation_flag = 0;
        }
        else validation_flag = 0;
    }
    else validation_flag = 0;
    if (!validation_flag) printf("Wrong flag!\n");

    return 0;
}