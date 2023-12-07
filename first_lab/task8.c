#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

long long int pow_int (const int base, const int power) {
    if (power == 0) {
        long long int result = 1;
        return result;
    }
    if (power == 1) return base;

    long long int result = base;
    for (int i = 1; i < power; i++) {
        result *= base;
    }
    return result;
}

int digits_count (long long int number) {
    int digits_count = 0; 
    while (number != 0) { 
        digits_count++;
        number /= 10;
    }
    return digits_count;
}

long long int from_base_to_int (char * number, int number_length, int base) {
    int power = 1;
    long long int decimal = 0;
    for (int i = number_length - 2; i >= 0; i--) {
        decimal += power * (isdigit(number[i]) ? number[i] - '0' : number[i] - 'a' + 10);
        power *= base;
    }
    return decimal;
}

void output (FILE * out, char * number, int number_length, int base) {
    fprintf(out, "%s %d %lld\n", number, base, from_base_to_int(number, number_length, base));
}

int base_of_number (char * number, int number_length) {
    int max_value = 0;
    for (int i = 0; i < number_length - 1; i++) {
        int ascii = ('A' <= number[i] && number[i] <= 'Z') ? number[i] + 32 : number[i];
        if (ascii > max_value) max_value = ascii;
    }
    
    return (48 <= max_value && max_value <= 57) ? max_value - 47 : 
        (97 <= max_value && max_value <= 122) ? max_value - 86 : 0;
}

enum file_splitter_status_codes {
    fssc_ok,
    fssc_memory_overflow,
    fssc_undefined_symbol
};

enum file_splitter_status_codes got_a_number (char * number, int * number_length, int * index, FILE * out) {
    if (*index > 0) {
        number[*index] = '\0';
        int base = base_of_number(number, *number_length);
        if (!base) return fssc_undefined_symbol;
        output(out, number, *number_length, base);
        free(number);
        *number_length = 1, *index = 0;
    }
    else output(out, "0", 2, 1);
    return fssc_ok;
}

enum file_splitter_status_codes file_splitter (FILE * in, FILE * out) {
    char input = getc(in);
    int undefined_number_length = 1, index = 0;
    char * undefined_number;
    while (input != EOF) {
        if (input == ' ' || input == '\n' || input == '\t') {
            while (input == '\t' || input == '\n' || input == ' ') {
                input = getc(in);
            }
            switch (got_a_number(undefined_number, &undefined_number_length, &index, out)) {
                case fssc_ok:
                    break;
                case fssc_undefined_symbol:
                    return fssc_undefined_symbol;
                    break;
            }
        }
        else {
            if (undefined_number_length == 1) {
                undefined_number = (char *)malloc(sizeof(char) * undefined_number_length);
                if (!undefined_number) return fssc_memory_overflow;
            }
            if (!((undefined_number_length == 1) && (input == '0'))) {
                if (input != '\t' && input != '\n' && input != ' ') {
                    undefined_number[index] = input;
                    undefined_number_length++;
                    index++;
                    char * tmp = (char *)realloc(undefined_number, undefined_number_length);
                    if (tmp) undefined_number = tmp;
                    else return fssc_memory_overflow;
                }
            }
            input = getc(in);
        }
    }
    if (index > 0) {
        switch (got_a_number(undefined_number, &undefined_number_length, &index, out)) {
            case fssc_ok:
                break;
            case fssc_undefined_symbol:
                return fssc_undefined_symbol;
                break;
        }
    }
    return fssc_ok;
}

int main (int argc, char * argv[]) {
    if (argc == 3) {
        FILE * in = fopen(argv[1], "r");
        FILE * out = fopen(argv[2], "w");
        if (out == NULL || in == NULL) {
            fclose(in);
            fclose(out);
            printf("Unable to open file\n");
            return 0;
        }
        switch (file_splitter(in, out)) {
            case fssc_memory_overflow:
                printf("No memory!\n");
                break;
            case fssc_undefined_symbol:
                printf("Undefined symbol in input file\n");
                break;
            case fssc_ok:
                printf("Done!\n");
                break;
        }
        fclose(in);
        fclose(out);
    }
    else {
        printf("wrong flag\n");
    }
    return 0;
}
