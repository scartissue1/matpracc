#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

bool is_splitter (char to_check) {
    return (to_check == ' ' || to_check == '\t' || to_check == '\n');
}

char * from_decimal_to_base (int number, int base) {
    int num_in_base_length = (log(number) / log(base)) + 2;
    char * num_in_base = (char *)malloc(sizeof(char) * num_in_base_length);
    if (!num_in_base) {
        return NULL;
    }
    num_in_base[num_in_base_length - 1] = '\0';

    for (int i = num_in_base_length - 2; i >= 0; i--) {
        num_in_base[i] = number % base + '0';
        number /= base;
    }

    return num_in_base;
}

void file_merge (FILE * in_1, FILE * in_2, FILE * out) {
    int flag = 0;
    char input1 = fgetc(in_1), input2 = fgetc(in_2);
    while (input1 != EOF || input2 != EOF) {
        if (flag == 0) {
            if (!is_splitter(input1) && input1 != EOF) {
                fputc(input1, out);
                input1 = fgetc(in_1);
            }
            else {
                fputc(' ', out);
                if (input2 != EOF) flag = 1;
                while (is_splitter(input1) && (input1 != EOF)) input1 = fgetc(in_1);
            }
        }
        else if (flag == 1) {
            if (!is_splitter(input2) && input2 != EOF) {
                fputc(input2, out);
                input2 = fgetc(in_2);
            }
            else {
                fputc(' ', out);
                if (input1 != EOF) flag = 0;
                while (is_splitter(input2) && (input2 != EOF)) input2 = fgetc(in_2);
            }
        }
    }
}

void file_copy (FILE * in, FILE * out) {
    char input = fgetc(in);
    int word_count = is_splitter(input) ? 0 : 1;
    while (input != EOF) {
        if (is_splitter(input)) {
            word_count++;
            fputc(' ', out);
            while (is_splitter(input)) input = getc(in);
        }
        else {
            if (word_count % 10 == 0) {
                int ascii = ('A' <= input && input <= 'Z') ? input + 32 : input;
                char * ascii_in_4 = from_decimal_to_base(ascii, 4);
                fprintf(out, "%s", ascii_in_4);
            }
            else if (word_count % 2 == 0) {
                int ascii = ('A' <= input && input <= 'Z' && !isdigit(input)) ? input + 32 : input;
                char output = ascii;
                if (isdigit(input)) output = input;
                fputc(output, out);
            }
            else if (word_count % 5 == 0) {
                int ascii = input;
                char * ascii_in_8 = from_decimal_to_base(ascii, 8);
                fprintf(out, "%s", ascii_in_8);
            }
            else fputc(input, out);
            input = fgetc(in);
        }

    }
}

int main (int argc, char * argv[]) {
    int val_flag = 1;
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'r') {
            if (argc == 5) {
                FILE * in_1 = fopen(argv[2], "r");
                FILE * in_2 = fopen(argv[3], "r");
                FILE * out = fopen(argv[5], "w");

                if (!in_1 || !in_2 || !out) {
                    fclose (in_1);
                    fclose (in_2);
                    fclose (out);
                    printf("Cannot open a file\n");
                    return 0;
                }

                file_merge(in_1, in_2, out);
                fclose(in_1);
                fclose(in_2);
                fclose(out);
            }
            else val_flag = 0;
        }
        else if (argv[1][1] == 'a') {
            if (argc == 4) {
                FILE * in = fopen(argv[2], "r");
                FILE * out = fopen(argv[3], "w");

                if (!in || !out) {
                    fclose(in);
                    fclose(out);
                    printf("Cannot open a file\n");
                    return 0;
                }

                file_copy(in, out);
                fclose(in);
                fclose(out);
            }
            else val_flag = 0;
        }
        else val_flag = 0;
    }
    else val_flag = 0;
    if (!val_flag) printf("Wrong flag\n");
    
    return 0;
}