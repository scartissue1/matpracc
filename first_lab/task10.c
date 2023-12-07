#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool pre_num_validation (const char * number) {
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

bool num_validity (const char * number, const int number_length, const int base) {
    if (!pre_num_validation(number)) return false;
    for (int i = 0; i < number_length - 1; i++) {
        if (!(('A' <= number[i] && number[i] <= 'Z') || isdigit(number[i]) || number[i] == '-' || number[i] == '+')) return false;
        int base_of_char = (isdigit(number[i])) ? number[i] - '0' : number[i] - 'A' + 10;
        if (base_of_char >= base) return false;
    }
    return true;
}

long long int from_base_to_int (char * number, int number_size, int base) {
    if (!number) {
        return 0;
    }
    int power = 1, sign = 1, sign_flag = 0;
    long long int decimal = 0;
    if (number[0] == '-') {
        sign = -1;
        sign_flag = 1;
    }
    else if (number[0] == '+') sign_flag = 1;
    for (int i = number_size - 1; i >= sign_flag; i--) {
        decimal += power * ((isdigit(number[i])) ? number[i] - '0' : number[i] - 'A' + 10);
        power *= base;
    }
    return decimal * sign;
}

char * from_decimal_to_base (int number, int base) {
    int num_in_base_length = 0, sign_flag = 0;
    if (number < 0) {
        num_in_base_length++;
        sign_flag = 1;
    }
    number = fabs(number);
    num_in_base_length += (log(number) / log(base)) + 2;
    char * num_in_base = (char *)malloc(sizeof(char) * num_in_base_length);
    if (!num_in_base) return NULL;
    num_in_base[num_in_base_length - 1] = '\0';
    if (sign_flag) num_in_base[0] = '-';
    for (int i = num_in_base_length - 2; i >= sign_flag; i--) {
        num_in_base[i] = (number % base > 9) ? number % base + 'A' - 10 : number % base + '0';
        number /= base;
    }
    return num_in_base;
}

int main (int argc, char * argv[]) {
    int base;
    printf("Enter the base in [2, 36]:\n");
    scanf("%d", &base);
    if (!(2 <= base && base <= 36)) {
        printf("in [2, 36]!!!!!!!\n");
        return -1;
    }
    printf("Enter numbers in that base. All letters should be in uppercase. Type Stop to stop\nWrong numbers will be skipped\n");
    char max[BUFSIZ];
    int max_size = 2; 
    long long int int_max = -1;
    char number[BUFSIZ];
    while (scanf("%s", number)) {
        if (strcmp(number, "Stop") != 0) {
            size_t num_size = strlen(number);
            if (num_validity(number, num_size, base)) {
                int number_int = from_base_to_int(number, num_size, base);
                if (number_int > int_max || int_max == -1) {
                    int_max = number_int;
                    strcpy(max, number);
                }
            }
        }
        else break;   
    }
    printf("%s\n", max);
    printf("in 10: %lld\n", int_max);
    if (int_max) {
        char *in_9 = from_decimal_to_base(int_max, 9);
        if (!in_9) {
            printf("No memory\n");
            return -1;
        }
        char *in_18 = from_decimal_to_base(int_max, 18);
        if (!in_18) {
            free(in_9);
            printf("No memory\n");
            return -1;
        }
        char *in_27 = from_decimal_to_base(int_max, 27);
        if (!in_27) {
            free(in_9);
            free(in_18);
            printf("No memory\n");
            return -1;
        }
        char *in_36 = from_decimal_to_base(int_max, 36);
        if (!in_36) {
            free(in_9);
            free(in_18);
            free(in_27);
            printf("No memory\n");
            return -1;
        }
        printf("in 9: %s\n", in_9);
        printf("in_18: %s\n", in_18);
        printf("in_27: %s\n", in_27);
        printf("in_36: %s\n", in_36);
        free(in_9);
        free(in_18);
        free(in_27);
        free(in_36);
    }
    else printf("0 in every base is 0\n");
    fflush(stdin);
    return 0;
}
