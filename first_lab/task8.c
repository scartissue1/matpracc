#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>

long long int pow_int (const int base, const int power) {
    if (power == 0) {
        int result = 1;
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
    int power = 0;
    long long int decimal = 0;
    for (int i = number_length - 2; i >= 0; i--) {
        int num = 0;
        if (isdigit(number[i])) {
            num = number[i] - '0';
        }
        else if (isalpha(number[i])) {
            num = number[i] - 87;
        }
        decimal += pow_int(base, power) * num;
        power++;
    }
    return decimal;
}

void output (FILE * out, char * number, int number_length, int base) {
    fprintf(out, "%s %d %lld\n", number, base, from_base_to_int(number, number_length, base));
}

int base_of_number (char * number, int number_length) {
    int max_value = 0, base;
    for (int i = 0; i < number_length - 1; i++) {
        int ascii = number[i];
        if (65 <= ascii && ascii <= 90) {
            ascii += 32;
            number[i] = ascii;
        }
        if (ascii > max_value) max_value = ascii;
    }
    if (48 <= max_value && max_value <= 57) base = max_value - 47;
    else if (97 <= max_value && max_value <= 122) base = max_value - 86;
    else if (max_value == 32 || max_value == 9 || max_value == 10) base = 1;
    else base = 0;
    return base;
}

enum file_splitter_status_codes {
    fssc_ok,
    fssc_memory_overflow,
    fssc_undefined_symbol
};

enum file_splitter_status_codes file_splitter (FILE * in, FILE * out) {
    char input;
    int undefined_number_length = 1, index = 0;
    char * undefined_number;
    while ((input = getc(in)) != EOF) {
        if (input == ' ' || input == '\n' || input == '\t') {
            undefined_number[index] = '\0';
            int base = base_of_number(undefined_number, undefined_number_length);
            if (!base) return fssc_undefined_symbol;
            output(out, undefined_number, undefined_number_length, base);
            free(undefined_number);
            undefined_number_length = 1, index = 0;
        }
        else {
            if (undefined_number_length == 1) undefined_number = (char *)malloc(sizeof(char) * undefined_number_length);
            if (!((undefined_number_length == 1) && (input == '0'))) {
                if (input != '\t' && input != '\n' && input != ' ') {
                    undefined_number[index] = input;
                    undefined_number_length++;
                    index++;
                    undefined_number = (char *)realloc(undefined_number, undefined_number_length);
                }
            }
        }
    }
    free(undefined_number);
    return fssc_ok;
}

int main (int argc, char * argv[]) {
    if (argc == 3) {
        FILE * in = fopen(argv[1], "r");
        FILE * out = fopen(argv[2], "w");
        if (in == NULL) {
            printf("Unable to open input file\n");
            return 0;
        }
        if (out == NULL) {
            printf("Unable to create output file\n");
            return 0;
        }
        switch (file_splitter(in, out)) {
            case fssc_memory_overflow:
                printf("No memory!\n");
                break;
            case fssc_undefined_symbol:
                printf("Undefined symbol in input file or two+ splitters char in a row\n");
                break;
            case fssc_ok:
                printf("Done!\n");
                break;
        }
        fclose(in);
        fclose(out);
    }
    return 0;
}
