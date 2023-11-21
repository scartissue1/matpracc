#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "stack.h"
#include "compute.h"

void printHead(FILE *stream, const char *vars, const int vars_count, const char *infix);

void printTable(FILE *stream, const int *values, const int values_count, const int result);

void errorProcessing(const status_codes error);

int comp(const char *first, const char *second);

void getFilename(char *filename);

status_codes getVariables(char **variables_array, const char *infix) {
    size_t array_size = 0;
    size_t array_capacity = 2;
    (*variables_array) = (char *)malloc(sizeof(char) * array_capacity);
    if (!(*variables_array)) {
        return NO_MEMORY;
    }
    size_t infix_size = strlen(infix);
    for (int i = 0; i < infix_size; i++) {
        if (!isalpha(infix[i])) {
            continue;
        }
        for (int j = 0; j < array_size; j++) {
            if (infix[i] == (*variables_array)[j]) {
                continue;
            }
        }
        (*variables_array)[array_size] = infix[i];
        array_size++;
        if (array_size >= array_capacity) {
            array_capacity *= 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc((*variables_array), sizeof(char) * array_capacity))) {
                free(*variables_array);
                return NO_MEMORY;
            }
            (*variables_array) = tmp;
        }
    }
    (*variables_array)[array_size] = '\0';
    qsort((*variables_array), array_size, sizeof(char), (int(*)(const void *, const void *))comp);
    return OK;
}

status_codes truth_table(const char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        return NO_FILE;
    }
    char *infix = NULL;
    getline(&infix, &(size_t){0}, input);
    size_t string_size = strlen(infix);
    if (!string_size || !infix || infix[string_size - 1] == '\n') {
        errorProcessing(INVALID_FILE);
        fclose(input);
        return INVALID_PARAMETER;
    }
    fclose(input);
    Node *root = NULL;
    status_codes status = getTree(&root, infix);
    if (status == NO_MEMORY) {
        freeTree(root);
        free(infix);
        return NO_MEMORY;
    }
    if (status != OK) {
        errorProcessing(status);
        freeTree(root);
        free(infix);
        return INVALID_PARAMETER;
    }
    char out_filename[128];
    getFilename(out_filename);
    FILE *out = fopen(out_filename, "w");
    if (!out) {
        freeTree(root);
        free(infix);
        return NO_FILE;
    }
    char *vars = NULL;
    if (getVariables(&vars, infix) == NO_MEMORY) {
        free(infix);
        freeTree(root);
        return NO_MEMORY;
    };
    int vars_count = strlen(vars);
    int combinations_count = 1 << vars_count;
    int vars_values[vars_count];
    printHead(out, vars, vars_count, infix);
    for (int i = 0; i < combinations_count; i++) {
        for (int j = 0; j < vars_count; j++) {
            vars_values[j] = (i >> j) & 1;
        }
        printTable(out, vars_values, vars_count, computeTree(root, vars, vars_values, vars_count));
    }
    free(infix);
    freeTree(root);
    fclose(out);
    free(vars);
    return OK;

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    switch (truth_table(argv[1])) {
        case OK:
            break;
        case INVALID_PARAMETER:
            return -1;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case NO_FILE:
            printf("No such file\n");
            return -1;
    }
    return 0;
}

void printHead(FILE *stream, const char *vars, const int vars_count, const char *infix) {
    for (int i = 0; i < vars_count; i++) {
        fprintf(stream, " %c |", vars[i]);
    }
    fprintf(stream, " %s\n", infix);
    fprintf(stream, "________________________________");
}

void printTable(FILE *stream, const int *values, const int values_count, const int result) {
    fprintf(stream, "\n");
    for (int i = 0; i < values_count; i++) {
        fprintf(stream, " %d |", values[i]);
    }
    fprintf(stream, " %d\n", result);
    fprintf(stream, "________________________________");
}

void errorProcessing(const status_codes error) {
    switch (error) {
        case INVALID_BRACKET:
            printf("Invalid amount of brackets\n");
            return;
        case INVALID_PARAMETER:
            printf("Invalid operand or operator\n");
            return;
        case UNUSED_OPERATORS_OR_OPERANDS:
            printf("Invalid amount of operators or operands\n");
            return;
        case INVALID_FILE:
            printf("Empty expression detected\n");
            return;
    }
}

int comp(const char *first, const char *second) {
    return *first - *second;
}

void getFilename(char *filename) {
    int size = rand() % 6 + 1;
    char result[size];
    for (int i = 0; i < size;) {
        result[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(result[i]) || isdigit(result[i])) {
            i++;
        }
    }
    strcat(result, ".txt\0");
    strcpy(filename, result);
}