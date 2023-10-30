#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

int get_power(int base);

status_codes int_to_base(long long int number, int base, char ** in_base);

int main(int argc, char * argv[]) {
    char * in_base = NULL;
    switch (int_to_base(1631, 9, &in_base)) {
        case OK:
            printf("%s\n", in_base);
            break;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Base should be equal to 2^r where r is [1, 5]\n");
            break;
    }
    return 0;
}

status_codes int_to_base(long long int number, int base, char ** in_base) {
    int power = get_power(base);
    if (!(1 <= power && power <= 5)) return INVALID_PARAMETER;
    if (!number) {
        (*in_base) = (char *)malloc(sizeof(char) * 2);
        if (!(*in_base)) return NO_MEMORY;
        (*in_base)[0] = '0';
        (*in_base)[1] = '\0';
        return OK;
    }
    int sign_flag = 0;
    if (number < 0) {
        sign_flag = 1;
        number = -number;
    }
    size_t in_base_size = log(number) / log(base) + 2 + sign_flag;
    (*in_base) = (char *)malloc(sizeof(char) * in_base_size);
    if (!(*in_base)) return NO_MEMORY;
    int index = in_base_size - 2;
    while (number) {
        (*in_base)[index] = ((number & (base - 1)) < 10) ? (number & (base - 1)) + '0' : (number & (base - 1)) + 'A' - 10;
        number >>= power;
        index--;
    }
    if (sign_flag) (*in_base)[0] = '-';
    (*in_base)[in_base_size - 1] = '\0';
    return OK;
}

int get_power(int base) {
    int power = 0;
    if (base & 1) return -1;
    while (base > 1) {
        base >>= 1;
        power++;
    }
    return power;
}
