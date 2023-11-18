#include "task.h"

int isPrime(int number) {
    if (number == 1 || number == 0) {
        return 0;
    }
    for (int i = 0; i < (int)sqrt(number) + 1; i++) {
        if (number % i == 0) {
            return 1;
        }
    }
    return 0;
}

int getPrime(int number) {
    if (number % 2 == 0) {
        number++;
    }
    while (!isPrime(number)) {
        number += 2;
    }
    return number;
}