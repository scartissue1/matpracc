#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

size_t _strlen(char * string) {
    int index = 0;
    while (string[index++] != '\0');
    return index - 1;
}

int _islower(char sym) { return (sym >= 'a') && (sym <= 'z'); }

int in_base_validation(char * number, const int base) {
    size_t number_size = _strlen(number);
    int index = 0;
    while (index < number_size) {
        if (!isdigit(number[index])) return 0;
        int curr = (isdigit(number[index])) ? number[index] - '0' : 
            _islower(number[index]) ? number[index] - 'A' - 22 : 
            number[index] - 'A' + 10;
        if (curr >= base) return 0;
        index++;
    }
    return 1;
}

char * delete_lead_zeros(char * string, size_t size) {
    int lead_zero_count = 0;
    for (int i = 0; i < size; i++) {
        if (string[i] == '0') lead_zero_count++;
        else break;
    }
    size_t new_size = (size - lead_zero_count + 1 > 1) ? size - lead_zero_count + 1 : 2; 
    char * wo_lead_zeros = (char *)malloc(sizeof(char) * new_size);
    if (new_size == 2) {
        wo_lead_zeros[0] = string[size - 1];
        wo_lead_zeros[1] = '\0';
        return wo_lead_zeros;
    }
    for (int i = 0; i < new_size; i++) {
        wo_lead_zeros[i] = string[i + lead_zero_count];
    }
    return wo_lead_zeros;
}

char * summ(char * string1, char * string2, size_t string1_size, size_t string2_size, const int base) {
    int tmp = 0;
    int result_size = ((string1_size > string2_size) ? string1_size : string2_size) + 2;
    int result_index = result_size - 1;
    char * result = (char *)malloc(sizeof(char) * result_size);
    int index1 = string1_size - 1, index2 = string2_size - 1, _1flag = 1, _2flag = 1;
    while (index1 >= 0 || index2 >= 0) {
        if (index1 < 0) _1flag = 0;
        if (index2 < 0) _2flag = 0;
        int curr = ((_1flag) ? isdigit(string1[index1]) ? string1[index1] - '0' : 
             _islower(string1[index1]) ? string1[index1] - 'A' - 22 : string1[index1] - 'A' + 10 : 0) +
            ((_2flag) ? isdigit(string2[index2]) ? string2[index2] - '0' : 
             _islower(string2[index2]) ? string2[index2] - 'A' - 22 : string2[index2] - 'A' + 10 : 0) + 
            tmp;
        tmp = 0;
        while (curr >= base) {
            curr -= base;
            tmp++;
        }
        result[--result_index] = (curr < 10) ? curr + '0' : curr + 'A' - 10;
        index1--;
        index2--;
    }
    result[0] = tmp + '0';
    result[result_size - 1] = '\0';
    return result;
}

typedef enum {
    OK,
    WRONG_BASE,
    WRONG_NUM,
    NO_MEMORY
} status_codes;

status_codes in_base_summ(char ** result, const int base, int quantity, ...) {
    if (base <= 2 || base >= 36) return WRONG_BASE;
    va_list number;
    va_start(number, quantity);
    *result = (char *)malloc(sizeof(char) * 2);
    if (!*result) return NO_MEMORY;
    (*result)[0] = '0';
    (*result)[1] = '\0';
    char * tmp = NULL;
    for (int i = 0; i < quantity; i++) {
        char * new_string = va_arg(number, char *);
        if (!in_base_validation(new_string, base)) {
            free(*result);
            return WRONG_NUM;
        }
        tmp = *result;
        *result = summ(*result, new_string, _strlen(*result), _strlen(new_string), base);
        free(tmp);
        tmp = NULL;
    }
    va_end(number);
    tmp = *result;
    *result = delete_lead_zeros(*result, _strlen(*result));
    free(tmp);
    return OK;
}

int main(int argc, char * argv[]) {
    char * result = NULL;
    status_codes st = in_base_summ(&result, 10, 2, "000000000000000000", "00000000");
    if (st == OK) {
        printf("%s\n", result);
        free(result);
    }
    return 0;
}