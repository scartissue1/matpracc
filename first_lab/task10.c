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
    if (!number) return 0;

    int power = 1, sign = 1, sign_flag = 0;
    long long int decimal = 0;
    if (number[0] == '-') {
        sign = -1;
        sign_flag = 1;
    }
    else if (number[0] == '+') sign_flag = 1;
    for (int i = number_size - 2; i >= sign_flag; i--) {
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

enum status_codes {
    ok,
    no_memory,
    wrong_data
};

enum status_codes get_a_number (FILE * in, char ** number, int * num_size, int base) {
    if (!in) return wrong_data;
    int index = 0;
    *num_size = 1;
    *number = (char *)malloc(sizeof(char) * (*num_size));
    if (!(*number)) return no_memory;

    char input = fgetc(in);
    while (input != '\n' && input != '\t' && input != ' ' && input != EOF) {
        if (!((*num_size) == 1 && input == '0')) {
            (*num_size)++;
            char * tmp = (char *)realloc((*number), sizeof(char) * (*num_size));
            if (!tmp) {
                free(*number);
                *number = NULL;
                return no_memory;
            }
            *number = tmp;
            (*number)[index] = input;
            index++;
        }

        input = fgetc(in);

    }
    (*number)[index] = '\0';
    return ok;

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
    char * max = (char *)malloc(sizeof(char) * 2);
    max[0] = '0';
    max[1] = '\0';
    int max_size = 2; 
    long long int int_max = 0;
    char * tmp = NULL;
    char * number = NULL;
    while (1) {
        int num_size = 0;
        enum status_codes status = get_a_number(stdin, &number, &num_size, base);
        if (status == ok) {
            if (strcmp(number, "Stop") != 0) {
                if (num_validity(number, num_size, base)) {
                    if (fabs(from_base_to_int(number, num_size, base)) > fabs(from_base_to_int(max, max_size, base))) {
                        tmp = max;
                        free(tmp);
                        tmp = NULL;
                        max = number;
                        max_size = num_size;
                        int_max = from_base_to_int(max, max_size, base);
                    }
                }
            } 
            else break;
            
        }
        else if (status == no_memory) {
            printf("No memory!\n");
            return -1;
        }
        else if (status == wrong_data) {
            printf("Smth wrong with stdin\n");
            return -1;
        }
        number = NULL;
    }
    printf("%s\n", max);
    printf("in 10: %lld\n", int_max);
    if (int_max) {
        printf("in 9: %s\n", from_decimal_to_base(int_max, 9));
        printf("in 18: %s\n", from_decimal_to_base(int_max, 18));
        printf("in 27: %s\n", from_decimal_to_base(int_max, 27));
        printf("in 36: %s\n", from_decimal_to_base(int_max, 36));
    }
    else printf("0 in every base is 0\n");
    free(max);
    free(number);
    fflush(stdin);
    return 0;
}
