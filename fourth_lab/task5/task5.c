#include <stdio.h>

#include "stack.h"
#include "compute.h"

void errorProcessing(const status_codes error, FILE *stream, const char *infix, const int line) {
    fprintf(stream, "%d %s ", line, infix);
    switch (error) {
        case INVALID_BRACKET:
            fprintf(stream, "Invalid amount of brackets\n");
            return;
        case ZERO_SIZE:
            fprintf(stream, "Zero sized string\n");
            return;
        case DIVIDE_BY_ZERO:
            fprintf(stream, "Dividing by zero\n");
            return;
        case NEGATIVE_MOD:
            fprintf(stream, "Negative number in division with remainder\n");
            return;
        case NEGATIVE_POWER:
            fprintf(stream, "Negative power\n");
            return;
        case NOT_DIGIT_OR_OPERATOR:
            fprintf(stream, "Expression should contain only digits and allowed operators (+, -, *, /, %%, ^)to be computed\n");
            return;
        case UNUSED_DIGITS_OR_OPERATORS:
            fprintf(stream, "Unused digits or operators\n");
            return;
    }
}

void printHead();

void printExpression(const char *filename, const char *infix, const char *postfix, const int result);

status_codes processFiles(const int files_quantity, char *filenames[]) {
    printHead();
    for (int i = 1; i < files_quantity; i++) {
        FILE *stream = fopen(filenames[i], "r");
        if (!stream) {
            printf("%s: no such file\n", filenames[i]);
            continue;
        }
        char filename_copy[128];
        strcpy(filename_copy, filenames[i]);
        char *token = strtok(filename_copy, ".");
        char name[128];
        strcpy(name, token);
        char *filename_error = strcat(name, "_errors.");
        token = strtok(NULL, ".");
        filename_error = strcat(filename_error, token);
        FILE *stream_error = NULL;
        char *infix = NULL;
        int line = 0;
        while (getline(&infix, &(size_t){0}, stream) != -1) {
            char infix_copy[128];
            strcpy(infix_copy, infix);
            char *token = strtok(infix_copy, "\n");
            char *postfix = NULL;
            status_codes status = to_postfix(&postfix, infix);
            if (status == INVALID_BRACKET) {
                if (!stream_error) {
                    stream_error = fopen(filename_error, "w");
                    if (!stream_error) {
                        free(infix);
                        free(postfix);
                        fclose(stream);
                        return NO_FILE;
                    }
                }
                errorProcessing(status, stream_error, token, line);
                free(infix);
                line++;
                continue;
            }
            if (status == NO_MEMORY) {
                free(infix);
                free(postfix);
                fclose(stream);
                return NO_MEMORY;
            }
            
            long long compute = 0;
            status = postfix_compute(&compute, postfix);
            if (status != OK) {
                if (status == NO_MEMORY) {
                    free(infix);
                    free(postfix);
                    fclose(stream);
                    return NO_MEMORY;
                }
                if (!stream_error) {
                    stream_error = fopen(filename_error, "w");
                    if (!stream_error) {
                        free(infix);
                        free(postfix);
                        fclose(stream);
                        return NO_FILE;
                    }
                }
                errorProcessing(status, stream_error, token, line);
                free(postfix);
                free(infix);
                line++;
                continue;
            }
            printExpression(filenames[i], token, postfix, compute);
            free(infix);
            free(postfix);
            infix = NULL;
            line++;
        }
        fclose(stream);
        if (stream_error) {
            fclose(stream_error);
        }
        free(infix);
    }
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong flag\n");
        return -1;
    }
    if (processFiles(argc, argv) == NO_MEMORY) {
        printf("No memory\n");
        return -1;
    }
    return 0;
}

void printHead() {
    printf("filename        | infix       | postfix        | result             \n");
}

void printExpression(const char *filename, const char *infix, const char *postfix, const int result) {
    printf("%s | %s | ", filename, infix);
    size_t size = strlen(postfix);
    for (int i = 0; i < size; i++) {
        if (postfix[i] == '~') {
            printf("-");
            continue;
        }
        printf("%c", postfix[i]);
    }
    printf(" | %d\n", result);
}