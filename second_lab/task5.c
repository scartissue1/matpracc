#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

size_t _strlen(char * string) {
    size_t size = 0;
    while (string[size++] != '\0');
    return size - 1;
}

char * to_roman(int number) {
    int roman_numbers[13] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
    char roman_letters[13][2] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
    size_t in_roman_size = 0, in_roman_capacity = 2;
    char * in_roman = (char *)malloc(sizeof(char) * in_roman_capacity);
    if (!in_roman) {
        return NULL;
    }
    if (number < 0) {
        number = abs(number);
        in_roman[0] = '-';
        in_roman_size = 1;
    }
    while (number) {
        int index = 13;
        while (roman_numbers[--index] > number);
        for (int i = 0; i < 2; i++) {
            if (roman_letters[index][i] == 'I' || roman_letters[index][i] == 'V' || roman_letters[index][i] == 'X' || 
            roman_letters[index][i] == 'L' || roman_letters[index][i] == 'C' || roman_letters[index][i] == 'D' || roman_letters[index][i] == 'M') {
                in_roman[in_roman_size] = roman_letters[index][i];
                in_roman_size++;
                if (in_roman_size == in_roman_capacity) {
                    in_roman_capacity *= 2;
                    char * tmp = (char *)realloc(in_roman, in_roman_capacity);
                    if (!tmp) {
                        printf("No memory\n");
                        free(in_roman);
                        return NULL;
                    }
                    in_roman = tmp;
                }
            }
        }
        number -= roman_numbers[index];
    }
    in_roman[in_roman_size] = '\0';
    return in_roman;

}

void fibonacci(unsigned int result[2], unsigned int number) {
    unsigned int count = 0;
    unsigned int tmp1 = 0, tmp2 = 1;
    unsigned int next = tmp1 + tmp2;
    while (next <= number) {
        tmp1 = tmp2;
        tmp2 = next;
        next = tmp1 + tmp2;
        count++;
    }
    result[0] = count;
    result[1] = tmp2;
}

int _isupper(char * number, size_t number_size) {
    for (int i = 0; i < number_size; i++) {
        if ((number[i] >= 'a') && (number[i] <= 'z')) return 0;
    }
    return 1;
}

int _islower(char * number, size_t number_size) {
    for (int i = 0; i < number_size; i++) {
        if ((number[i] >= 'A') && (number[i] <= 'Z')) return 0;
    }
    return 1;
}

long long int from_base_to_int(char * number, int number_size, int base, int upper_flag) {
    if (!number) return 0;
    if ((base < 2) || (base > 36)) base = 10;
    
    int power = 1, sign = 1, sign_flag = 0;
    long long int decimal = 0;
    if (number[0] == '-') {
        sign = -1;
        sign_flag = 1;
    }
    else if (number[0] == '+') sign_flag = 1;
    for (int i = number_size - 1; i >= sign_flag; i--) {
        decimal += power * ((isdigit(number[i])) ? number[i] - '0' : (upper_flag) ? number[i] - 'A' + 10 : number[i] - 'a' + 10);
        power *= base;
    }
    return decimal * sign;
}

char * from_decimal_to_base(long long int number, int base, int upper_flag) {
    if ((base < 2) || (base > 36)) base = 10;
    
    int num_in_base_length = 0, sign_flag = 0;
    if (number < 0) {
        num_in_base_length++;
        sign_flag = 1;
    }
    number = abs(number);
    num_in_base_length += (log(number) / log(base)) + 2;
    char * num_in_base = (char *)malloc(sizeof(char) * num_in_base_length);
    if (!num_in_base) return NULL;
    num_in_base[num_in_base_length - 1] = '\0';
    if (sign_flag) num_in_base[0] = '-';
    for (int i = num_in_base_length - 2; i >= sign_flag; i--) {
        num_in_base[i] = (number % base > 9) ? (upper_flag) ? number % base + 'A' - 10 : number % base + 'a' - 10: number % base + '0';
        number /= base;
    }
    return num_in_base;
}

void zeckendorf(size_t * result_size, unsigned int number, unsigned int result[number][2]) {
    (*result_size) = 0; 
    while (number) {
        fibonacci(result[*result_size], number);
        (*result_size)++;
        number -= result[(*result_size) - 1][1];
    }
}

int overfprintf(FILE * stream, char * format, ...) {
    if (!stream) return -1;
    size_t format_size = _strlen(format);
    if (!format_size) return -1;
    int arg_quantity = 0;
    for (size_t i = 0; i < format_size; i++) {
        if (format[i] == '%') arg_quantity++;
    }
    va_list args;
    va_start(args, arg_quantity);
    size_t format_index = 0;
    int accum = 0;
    while (format[format_index] != '%' && (format_index < format_size)) {
        accum += fprintf(stream, "%c", format[format_index]);
        format_index++;
    }
    char * flag = NULL;
    char * tmp_flag = NULL;
    for (int i = 0; i < arg_quantity; i++) {
        size_t flag_size = 1, flag_capacity = 2;
        flag = (char *)malloc(sizeof(char) * flag_capacity);
        flag[0] = '%';
        if (tmp_flag) free(tmp_flag);
        while ((isalpha(format[++format_index]) || isdigit(format[format_index]) || (format[format_index] == '%' && format[format_index - 1] == '%') ||
        (format[format_index] == '.' && (isalpha(format[format_index + 1]) || isdigit(format[format_index + 1]) || format[format_index + 1] == '*'))
        || (format[format_index] == '#') || (format[format_index] == '+') || (format[format_index] == '-') || (format[format_index] == '0')) && (format_index < format_size)) {
            flag[flag_size] = format[format_index];
            flag_size++;
            if (flag_size >= flag_capacity) {
                flag_capacity *= 2;
                char * tmp = (char *)realloc(flag, sizeof(char) * flag_capacity);
                if (!tmp) {
                    free(flag);
                    return -1;
                }
                flag = tmp;
            }
        }
        flag[flag_size] = '\0';
        if (strcmp(flag, "%Ro\0") == 0) {
            char * result = to_roman(va_arg(args, int));
            if (!result) {
                free(flag);
                return -1;
            }
            accum += fprintf(stream, "%s", result);
            free(result);
        }
        else if (strcmp(flag, "%Zr\0") == 0) {
            unsigned int number = va_arg(args, unsigned int);
            size_t result_size;
            unsigned int result[number][2];
            zeckendorf(&result_size, number, result);
            int count = 1;
            for (int i = result_size - 1; i >= 0;) {
                if (count == result[i][0]) {
                    accum += fprintf(stream, "1");
                    i--;
                    count++;
                }
                else {
                    accum += fprintf(stream, "0");
                    count++;
                }
            }
            accum += fprintf(stream, "1");
        }
        else if (strcmp(flag, "%Cv\0") == 0 || strcmp(flag, "%CV\0") == 0) {
            long long int to_convert = va_arg(args, long long int);
            if (to_convert == 0) {
                fprintf(stream, "%d", 0);
                continue;
            }
            arg_quantity++;
            int base = va_arg(args, int);
            char * converted = NULL;
            if (strcmp(flag, "%Cv\0") == 0) converted = from_decimal_to_base(to_convert, base, 0);
            else converted = from_decimal_to_base(to_convert, base, 1);
            if (!converted) {
                free(flag);
                return -1;
            }
            accum += fprintf(stream, "%s", converted);
            free(converted);
        }
        else if ((strcmp(flag, "%to\0") == 0) || (strcmp(flag, "%TO\0") == 0)) {
            char * to_convert = va_arg(args, char *);
            size_t to_convert_size = _strlen(to_convert);
            arg_quantity++;
            int base = va_arg(args, int);
            long long int result = 0;
            if (strcmp(flag, "%to\0") == 0) {
                if (!_islower(to_convert, to_convert_size)) {
                    accum += fprintf(stream, "\nLetters must be lowercase\n");
                    return -1;
                }
                result = from_base_to_int(to_convert, to_convert_size, base, 0);
            }
            else {
                if (!_isupper(to_convert, to_convert_size)) {
                    accum += fprintf(stream, "\nLetters must be uppercase\n");
                    return -1;
                }
                result = from_base_to_int(to_convert, to_convert_size, base, 1);
            }
            accum += fprintf(stream, "%lld", result);

        }
        else if (strcmp(flag, "%mi") == 0) {
            int value = va_arg(args, int);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(int); i++) {
                accum += fprintf(stream, "%u", *ptr % 2);
                ptr++;
            }
        }
        else if (strcmp(flag, "%mu") == 0) {
            unsigned int value = va_arg(args, unsigned int);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(unsigned int); i++) {
                accum += fprintf(stream, "%u", *ptr % 2);
                ptr++;
            }
        }
        else if (strcmp(flag, "%md") == 0) {
            double value = va_arg(args, double);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(double); i++) {
                accum += fprintf(stream, "%u", *ptr % 2);
                ptr++;
            }
        }
        else if (strcmp(flag, "%mf") == 0) {
            float value = va_arg(args, double);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(float); i++) {
                accum += fprintf(stream, "%u", *ptr % 2);
                ptr++;
            }
        }
        else {
            accum += vfprintf(stream, flag, args);
        }
        while (format[format_index] != '%' && (format_index < format_size)) {
            accum += fprintf(stream, "%c", format[format_index]);
            format_index++;
        }
        tmp_flag = flag;
    }
    if (tmp_flag) free(tmp_flag);
    va_end(args);
    return accum;
}

int oversprintf(char * buf, char * format, ...) {
    size_t format_size = _strlen(format);
    if (!format_size) return -1;
    int arg_quantity = 0;
    for (size_t i = 0; i < format_size; i++) {
        if (format[i] == '%') arg_quantity++;
    }
    va_list args;
    va_start(args, arg_quantity);
    size_t format_index = 0;
    int accum = 0;
    while (format[format_index] != '%' && (format_index < format_size)) {
        accum += sprintf(buf + accum, "%c", format[format_index]);
        format_index++;
    }
    char * flag = NULL;
    char * tmp_flag = NULL;
    for (int i = 0; i < arg_quantity; i++) {
        size_t flag_size = 1, flag_capacity = 2;
        flag = (char *)malloc(sizeof(char) * flag_capacity);
        flag[0] = '%';
        if (tmp_flag) free(tmp_flag);
        while ((isalpha(format[++format_index]) || isdigit(format[format_index]) || (format[format_index] == '%' && format[format_index - 1] == '%') ||
        (format[format_index] == '.' && (isalpha(format[format_index + 1]) || isdigit(format[format_index + 1]) || format[format_index + 1] == '*'))
        || (format[format_index] == '#') || (format[format_index] == '+') || (format[format_index] == '-') || (format[format_index] == '0')) && (format_index < format_size)) {
            flag[flag_size] = format[format_index];
            flag_size++;
            if (flag_size >= flag_capacity) {
                flag_capacity *= 2;
                char * tmp = (char *)realloc(flag, sizeof(char) * flag_capacity);
                if (!tmp) {
                    free(flag);
                    return -1;
                }
                flag = tmp;
            }
        }
        flag[flag_size] = '\0';
        if (strcmp(flag, "%Ro\0") == 0) {
            char * result = to_roman(va_arg(args, int));
            if (!result) {
                free(flag);
                return -1;
            }
            accum += sprintf(buf + accum, "%s", result);
            free(result);
        }
        else if (strcmp(flag, "%Zr\0") == 0) {
            unsigned int number = va_arg(args, unsigned int);
            size_t result_size;
            unsigned int result[number][2];
            zeckendorf(&result_size, number, result);
            int count = 1;
            for (int i = result_size - 1; i >= 0;) {
                if (count == result[i][0]) {
                    accum += sprintf(buf + accum, "1");
                    i--;
                    count++;
                }
                else {
                    accum += sprintf(buf + accum, "0");
                    count++;
                }
            }
            accum += sprintf(buf + accum, "1");
        }
        else if (strcmp(flag, "%Cv\0") == 0 || strcmp(flag, "%CV\0") == 0) {
            long long int to_convert = va_arg(args, long long int);
            if (to_convert == 0) {
                accum += sprintf(accum + buf, "%d", 0);
                continue;
            }
            arg_quantity++;
            int base = va_arg(args, int);
            char * converted = NULL;
            if (strcmp(flag, "%Cv\0") == 0) converted = from_decimal_to_base(to_convert, base, 0);
            else converted = from_decimal_to_base(to_convert, base, 1);
            if (!converted) {
                free(flag);
                return -1;
            }
            accum += sprintf(buf + accum, "%s", converted);
            free(converted);
        }
        else if ((strcmp(flag, "%to\0") == 0) || (strcmp(flag, "%TO\0") == 0)) {
            char * to_convert = va_arg(args, char *);
            size_t to_convert_size = _strlen(to_convert);
            arg_quantity++;
            int base = va_arg(args, int);
            long long int result = 0;
            if (strcmp(flag, "%to\0") == 0) {
                if (!_islower(to_convert, to_convert_size)) {
                    accum += sprintf(buf + accum, "\nLetters must be lowercase\n");
                    return -1;
                }
                result = from_base_to_int(to_convert, to_convert_size, base, 0);
            }
            else {
                if (!_isupper(to_convert, to_convert_size)) {
                    accum += sprintf(buf + accum, "\nLetters must be uppercase\n");
                    return -1;
                }
                result = from_base_to_int(to_convert, to_convert_size, base, 1);
            }
            accum += sprintf(buf + accum, "%lld", result);

        }
        else if (strcmp(flag, "%mi") == 0) {
            int value = va_arg(args, int);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(int); i++) {
                accum += sprintf(buf + accum, "%u", *ptr % 2);
                ptr++;
            }
        }
        else if (strcmp(flag, "%mu") == 0) {
            unsigned int value = va_arg(args, unsigned int);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(unsigned int); i++) {
                accum += sprintf(buf + accum, "%u", *ptr % 2);
                ptr++;
            }
        }
        else if (strcmp(flag, "%md") == 0) {
            double value = va_arg(args, double);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(double); i++) {
                accum += sprintf(buf + accum, "%u", *ptr % 2);
                ptr++;
            }
        }
        else if (strcmp(flag, "%mf") == 0) {
            float value = va_arg(args, double);
            unsigned char * ptr = (unsigned char *)&value;
            for (int i = 0; i < sizeof(float); i++) {
                accum += sprintf(buf + accum, "%u", *ptr % 2);
                ptr++;
            }
        }
        else {
            accum += vsprintf(buf + accum, flag, args);
        }
        while (format[format_index] != '%' && (format_index < format_size)) {
            accum += sprintf(buf + accum, "%c", format[format_index]);
            format_index++;
        }
        tmp_flag = flag;
    }
    if (tmp_flag) free(tmp_flag);
    va_end(args);
    return accum;
}

int main(int argc, char * argv[]) {
    // int x = 0;
    // FILE * out = fopen("1.txt", "w");

    // fprintf(out, "%d %\n", x);
    // fclose(out);
    overfprintf(stdout, "%mi %mi %mi %mi\n", 65, 65, 5, 2);
    //char * buf = (char *)malloc(sizeof(char) * BUFSIZ);
    //oversprintf(buf, "%Ro\n\n\n%d\n\n", 10, 123123, 232.423, 1231.123);
    //printf("%s\n", buf);
    //free(buf);
    return 0;
}
