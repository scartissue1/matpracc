#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

void arabic_number_excluder (FILE * in, FILE * out) {
    char to_check;
    while ((to_check = fgetc(in)) != EOF) {
        if (!isdigit(to_check)) {
            fputc(to_check, out);
        }
    }
}

void latin_letters_counter (FILE * in, FILE * out) {
    char to_check;
    int count = 0;
    while ((to_check = fgetc(in)) != EOF) {
        if (to_check != '\n') {
            if ((65 <= to_check && to_check <= 90) || (97 <= to_check && to_check <= 122)) {
                count++;
            }
        }
        else {
            fprintf(out, "%d\n", count);
            count = 0;
        }
    }
}

void not_latin_digit_space_counter (FILE * in, FILE * out) {
    char to_check;
    int count = 0;
    while ((to_check = fgetc(in)) != EOF) {
        if (to_check != '\n') {
            if (!isdigit(to_check) && !isalpha(to_check) && to_check != ' ') {
                count++;
            }
        }
        else {
            fprintf(out, "%d\n", count);
            count = 0;
        }
    }
}

void char_to_hex (FILE * in, FILE * out) {
    char to_check;
    while ((to_check = fgetc(in)) != EOF) {
        if (!isdigit(to_check)) {
            fprintf(out, "%x", to_check);
        }
        else fputc(to_check, out);
    }
}

int main (int argc, char * argv[]) {
    if (argc >= 3) {
        FILE * in = fopen(argv[2], "r");
        if (in == NULL) printf("Unable to open the file\n");
        FILE * out;
        if (argv[1][0] == '-' || argv[1][0] == '/') {
            if (argv[1][1] == 'n') {
                if (argc == 4) {
                    out = fopen(argv[3], "w");
                    if (out == NULL) printf("Unable to create the file\n");
                }
                else printf("No filename to create\n");
            }
            else {
                if (argc == 3) {
                    char * file_name = (char*)malloc(sizeof(char) * (5 + strlen(argv[2])));
                    if (file_name == NULL) printf("No memory\n");
                    file_name[0] = 'o', file_name[1] = 'u', file_name[2] = 't', file_name[3] = '_', file_name[4 + strlen(argv[2])] = '\0';
                    for (int i = 4; i < 4 + strlen(argv[2]); i++) {
                        file_name[i] = argv[2][i - 4];
                    }
                    
                    out = fopen(file_name, "w");
                    if (out == NULL) printf("Unable to create the file\n");
                    free(file_name);
                }
                else printf("No filename\n");
            }
            if (argv[1][1] == 'd' || argv[1][2] == 'd') {
                arabic_number_excluder(in, out);
            }
            else if (argv[1][1] == 'i' || argv[1][2] == 'i') {
                latin_letters_counter(in, out);
            }
            else if (argv[1][1] == 's' || argv[1][2] == 's') {
                not_latin_digit_space_counter(in, out);
            }
            else if (argv[1][1] == 'a' || argv[1][2] == 'a') {
                char_to_hex(in, out);
            }
            else printf("Wrong flag\n");
        }
        fclose(in);
        fclose(out);
    }
    else printf("Wrong flag!\n");
    return 0;
    
}
