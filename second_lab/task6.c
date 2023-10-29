#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

size_t _strlen(const char * string) {
    size_t size = 0;
    while (string[++size] != '\0');
    return size;
}

int in_register(const char * number, const size_t number_size, const char left, const char right);

int from_base_to_int(const char * number, const size_t number_size, int base, const int upper_flag);

int from_roman(const char * roman);

unsigned int from_zeckendorf(const char * zeckendorf);

int overfscanf(FILE * stream, const char * format, ...);

int oversscanf(char * buf, const char * format, ...);

int main(int argc, char * argv[]) {
    int roman;
    int zeckendorf;
    int from_base;
    //overfscanf(stdin, "%d %d %d", &roman, &zeckendorf, &from_base);
    char buf[] = "101011011 a";
    oversscanf(buf, "%Zr%Cv", &zeckendorf, &from_base, 16);
    printf("%d %d\n", zeckendorf, from_base);
    return 0;
}

int overfscanf(FILE * stream, const char * format, ...) {
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

    while (format[format_index++] != '%' && (format_index < format_size));
    char * flag = NULL;
    char * tmp_flag = NULL;
    
    for (int i = 0; i < arg_quantity; i++) {
        size_t flag_size = 1, flag_capacity = 2;
        flag = (char *)malloc(sizeof(char) * flag_capacity);
        flag[0] = '%';
        if (tmp_flag) free(tmp_flag);

        while ((isdigit(format[format_index])) || (isalpha(format[format_index]) || (format[format_index] == '%' && format[format_index - 1] == '%') || format[format_index] == '[' || format[format_index] == ']') && (format_index < format_size)) {
            flag[flag_size] = format[format_index];
            flag_size++;
            if (flag_size >= flag_capacity) {
                flag_capacity *= 2;
                char * tmp = (char *)realloc(flag, sizeof(char) * flag_capacity);
                if (!tmp) {
                    free(flag);
                    //printf("No memory\n");
                    return -1;
                }
                flag = tmp;
            }
            format_index++;
        }
        flag[flag_size] = '\0';
        if (strcmp(flag, "%Ro") == 0) {
            int * input = va_arg(args, int *);
            char roman[BUFSIZ];
            accum += fscanf(stream, "%s", roman);
            (*input) = from_roman(roman);
        }
        else if (strcmp(flag, "%Zr") == 0) {
            unsigned int * input = va_arg(args, unsigned int *);
            char zeckendorf[BUFSIZ];
            accum += fscanf(stream, "%s", zeckendorf);
            (*input) = from_zeckendorf(zeckendorf);
        }
        else if (strcmp(flag, "%Cv") == 0 || strcmp(flag, "%CV") == 0) {
            int * input = va_arg(args, int *);
            int base = va_arg(args, int);
            arg_quantity++;
            char in_base[BUFSIZ];
            accum += fscanf(stream, "%s", in_base);
            const size_t in_base_size = _strlen(in_base);
            if (flag[2] == 'V') {
                if (in_register(in_base, in_base_size, 'A', 'Z')) {
                    (*input) = from_base_to_int(in_base, in_base_size, base, 1);
                }
                else (*input) = 0;
            }
            else {
                if (in_register(in_base, in_base_size, 'a', 'z')) {
                    (*input) = from_base_to_int(in_base, in_base_size, base, 0);
                }
                else (*input) = 0; 
            }

        }
        else  {
            void * new_arg = va_arg(args, void *);
            accum += fscanf(stream, flag, new_arg);
        }
        tmp_flag = flag;
        while (format[format_index++] != '%' && (format_index < format_size));
    }
    if (tmp_flag) free(tmp_flag);
    va_end(args);
    return accum;
}

int oversscanf(char * buf, const char * format, ...) {
    if (!buf) return -1;
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
    size_t buf_index = 0;

    while (format[format_index++] != '%' && (format_index < format_size));
    char * flag = NULL;
    char * tmp_flag = NULL;
    
    for (int i = 0; i < arg_quantity; i++) {
        size_t flag_size = 1, flag_capacity = 2;
        flag = (char *)malloc(sizeof(char) * flag_capacity);
        flag[0] = '%';
        if (tmp_flag) free(tmp_flag);

        while ((isdigit(format[format_index])) || (isalpha(format[format_index]) || (format[format_index] == '%' && format[format_index - 1] == '%') || format[format_index] == '[' || format[format_index] == ']') && (format_index < format_size)) {
            flag[flag_size] = format[format_index];
            flag_size++;
            if (flag_size >= flag_capacity) {
                flag_capacity *= 2;
                char * tmp = (char *)realloc(flag, sizeof(char) * flag_capacity);
                if (!tmp) {
                    free(flag);
                    //printf("No memory\n");
                    return -1;
                }
                flag = tmp;
            }
            format_index++;
        }
        flag[flag_size] = '\0';
        if (strcmp(flag, "%Ro") == 0) {
            int * input = va_arg(args, int *);
            char roman[BUFSIZ];
            accum += sscanf(buf + buf_index, "%s", roman);
            buf_index += _strlen(roman);
            (*input) = from_roman(roman);
        }
        else if (strcmp(flag, "%Zr") == 0) {
            unsigned int * input = va_arg(args, unsigned int *);
            char zeckendorf[BUFSIZ];
            accum += sscanf(buf + buf_index, "%s", zeckendorf);
            buf_index += _strlen(zeckendorf);
            (*input) = from_zeckendorf(zeckendorf);
        }
        else if (strcmp(flag, "%Cv") == 0 || strcmp(flag, "%CV") == 0) {
            int * input = va_arg(args, int *);
            int base = va_arg(args, int);
            arg_quantity++;
            char in_base[BUFSIZ];
            accum += sscanf(buf + buf_index, "%s", in_base);
            size_t in_base_size = _strlen(in_base);
            buf_index += in_base_size;
            if (flag[2] == 'V') {
                if (in_register(in_base, in_base_size, 'A', 'Z')) {
                    (*input) = from_base_to_int(in_base, in_base_size, base, 1);
                }
                else (*input) = 0;
            }
            else {
                if (in_register(in_base, in_base_size, 'a', 'z')) {
                    (*input) = from_base_to_int(in_base, in_base_size, base, 0);
                }
                else (*input) = 0; 
            }

        }
        else  {
            void * new_arg = va_arg(args, void *);
            accum += sscanf(buf + buf_index, flag, new_arg);
            // buf_index += _strlen(new_arg) * sizeof(void *) + 1;
            // printf("%d\n", buf_index);
            // accum += vsscanf(buf + buf_index, flag, args);
            int splitter = 0;
            while (buf[buf_index] != 0) {
                if ((isalpha(buf[buf_index]) || isdigit(buf[buf_index])) && (splitter)) {
                    break;
                }
                if (buf[buf_index] == ' ' || buf[buf_index] == '\n') splitter++;
                buf_index++;
            }
        }
        tmp_flag = flag;
        while (format[format_index++] != '%' && (format_index < format_size));
    }
    if (tmp_flag) free(tmp_flag);
    va_end(args);
    return accum;
}

int from_roman(const char * roman) {
    size_t roman_size = _strlen(roman);
    if (!roman_size) return 0;
    int sign = 1, sign_flag = 0;
    if (roman[0] == '-') {
        sign = -1;
        sign_flag = 1;
    }
    
    int result = 0;
    for (int i = roman_size - 1; i >= sign_flag; i--) {
        if (roman[i] == 'I') {
            if (roman[i + 1] == 'X' || roman[i + 1] == 'V') result--;
            else result++;
        }
        else if (roman[i] == 'V') result += 5;
        else if (roman[i] == 'X') {
            if (roman[i + 1] == 'L' || roman[i + 1] == 'C') result -= 10;
            else result += 10;
        }
        else if (roman[i] == 'L') result += 50;
        else if (roman[i] == 'C') {
            if (roman[i + 1] == 'D' || roman[i + 1] == 'M') result -= 100;
            else result += 100;
        }
        else if (roman[i] == 'M') result += 1000;
        //else return -1;

    }
    return result * sign;
}

unsigned int from_zeckendorf(const char * zeckendorf) {
    size_t zeckendorf_size = _strlen(zeckendorf);
    if (!zeckendorf_size) return 0;
    unsigned int prev1 = 0, prev2 = 1;
    unsigned int curr = 1;
    
    unsigned int result = 0;

    for (int i = 0; i < zeckendorf_size - 1; i++) {
        if (zeckendorf[i] == '1') result += curr;
        prev1 = prev2;
        prev2 = curr;
        curr = prev1 + prev2;
    }
    return result;
}

int in_register(const char * number, const size_t number_size, const char left, const char right) {
    for (int i = 0; i < number_size; i++) {
        if (!((number[i] >= left) && (number[i] <= right)) && isalpha(number[i])) return 0;
    }
    return 1;
}

int from_base_to_int(const char * number, const size_t number_size, int base, const int upper_flag) {
    if (!number) return 0;
    if (!number_size) return 0;
    if ((base < 2) || (base > 36)) base = 10;
    
    int power = 1, sign = 1, sign_flag = 0;
    int decimal = 0;
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
