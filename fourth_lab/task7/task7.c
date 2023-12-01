#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "MemoryCell.h"
#include "compute.h"
#include "task.h"

void errorProcessing(const status_codes error, const int line) {
    switch (error) {
        case NO_MEMORY:
            printf("No memory after %d\n line", line);
            return;
        case INVALID_PARAMETER:
            printf("Invalid parameter detected in %d line\n", line);
            return;
        case ZERO_SIZE:
            printf("Zero sized rvalue detected in line %d\n", line);
            return;
        case NEGATIVE_MOD:
            printf("Attempt to module negative values in %d line\n", line);
            return;
        case INVALID_SYMBOL:
            printf("Invalid symbol detected. %d line\n", line);
            return;
        case INVALID_NAME:
            printf("Invalid variable name: it should contain only letters. %d line\n", line);
            return;
        case DIVIDE_BY_ZERO:
            printf("Attempt to divide by zero in %d line\n", line);
            return;
        case UNEQUAL_BRACKET:
            printf("Unequal amount of opening and closing brackets detected, %d line\n", line);
            return;
        case UNINITIALIZED_VALUE:
            printf("Using of uninitialized value detected in %d line\n", line);
            return;
        case UNUSED_VARS_OR_OPERATORS:
            printf("Unused vars or operators in rvalue in %d line\n", line);
            return;
        
    }
}

status_codes deleteString(char **string, size_t *size, size_t *capacity) {
    (*size) = 0;
    (*capacity) = 2;
    char *tmp = NULL;
    if (!(tmp = (char *)realloc((*string), sizeof(char) * (*capacity)))) {
        free(*string);
        return NO_MEMORY;
    }
    (*string) = tmp;
    return OK;
}

status_codes addString(char **string, size_t *size, size_t *capacity, const char symbol) {
    (*string)[(*size)] = symbol;
    (*size)++;
    if ((*size) >= (*capacity)) {
        (*capacity) *= 2;
        char *tmp = NULL;
        if (!(tmp = (char *)realloc((*string), sizeof(char) * (*capacity)))) {
            free(*string);
            return NO_MEMORY;
        }
        (*string) = tmp;
    }
    return OK;
}

void printAll(ArrayCell array, size_t size) {
    for (int i = 0; i < size; i++) {
        printf("Name: %s, value: %lld\n", array[i]->name, array[i]->value);
    }
}

status_codes processPrint(const char *command, ArrayCell array, size_t size) {
    char copy[strlen(command)];
    strcpy(copy, command);
    const char *separators = " ";
    char *token = strtok(copy, separators);
    if (!token || strcmp(token, "print")) {
        freeArrayCell(array, size);
        return INVALID_PARAMETER;
    }
    token = strtok(NULL, separators);
    if (!token) {
        printAll(array, size);
        return OK;
    }
    int index = binarySearch(array, size, token);
    if (index == -1) {
        freeArrayCell(array, size);
        return UNINITIALIZED_VALUE;
    }
    printf("%lld\n", array[index]->value);
    return OK;
}

status_codes processCommand(const char *command, ArrayCell *array, size_t *size, size_t *capacity) {
    char copy[strlen(command)];
    strcpy(copy, command);
    const char *separators = "=";
    char *token = strtok(copy, separators);
    if (!token) {
        freeArrayCell(*array, *size);
        return INVALID_PARAMETER;
    }

    char lvalue[strlen(token)];
    strcpy(lvalue, token);
    if (!name_validation(lvalue)) {
        freeArrayCell(*array, *size);
        return INVALID_NAME;
    }
    token = strtok(NULL, separators);
    if (!token) {
        freeArrayCell(*array, *size);
        return INVALID_PARAMETER;
    }
    char *postfix = NULL;
    status_codes status = getPostfix(&postfix, token);
    if (status != OK) {
        freeArrayCell(*array, *size);
        return status;
    }
    long long res = 0;
    status = computePostfix(&res, postfix, *array, *size);
    if (status != OK) {
        freeArrayCell(*array, *size);
        return status;
    }
    free(postfix);
    int index = binarySearch(*array, *size, lvalue);
    if (index == -1) {
        if (addCell(array, size, capacity, lvalue, res) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    else {
        (*array)[index]->value = res; 
    }
    return OK;
}

status_codes getCommand(FILE *stream) {
    ArrayCell array = NULL;
    size_t array_size = 0;
    size_t array_capacity = 2;
    if (initializeArray(&array, &array_size, &array_capacity) == NO_MEMORY) {
        fclose(stream);
        return NO_MEMORY;
    }
    size_t command_size = 0;
    size_t command_capacity = 2;
    char *command = (char *)malloc(sizeof(char) * command_capacity);
    if (!command) {
        freeArrayCell(array, array_size);
        fclose(stream);
        return NO_MEMORY;
    }
    char symbol = fgetc(stream);
    int print_flag = 0;
    status_codes status = OK;
    int line = 0;
    while (symbol != EOF) {
        if (symbol == ' ') {
            command[command_size] = '\0';
            if (strcmp(command, "print") == 0) {
                print_flag = 1;
            }
            else {
                symbol = fgetc(stream);
                continue;
            }
        }
        if (symbol == '\n') {
            line++;
            symbol = fgetc(stream);
            continue;
        }
        if (symbol == ';') {
            command[command_size] = '\0';
            if (strcmp(command, "print") == 0) {
                print_flag = 1;
            }
            if (!print_flag) {
                status = processCommand(command, &array, &array_size, &array_capacity);
            }
            else {
                status = processPrint(command, array, array_size);
                print_flag = 0;
            }
            if (status != OK) {
                free(command);
                fclose(stream);
                errorProcessing(status, line);
                return status;
            }
            if (deleteString(&command, &command_size, &command_capacity) == NO_MEMORY) {
                free(command);
                fclose(stream);
                freeArrayCell(array, array_size);
                errorProcessing(NO_MEMORY, line);
                return NO_MEMORY;
            }
            symbol = fgetc(stream);
            continue;
        }
        if (addString(&command, &command_size, &command_capacity, symbol) == NO_MEMORY) {
            free(command);
            fclose(stream);
            freeArrayCell(array, array_size);
            errorProcessing(NO_MEMORY, line);
            return NO_MEMORY;
        }
        symbol = fgetc(stream);
    }
    freeArrayCell(array, array_capacity);
    free(command);
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    FILE *stream = fopen(argv[1], "r");
    if (!stream) {
        printf("No such file\n");
        return -1;
    }
    if (getCommand(stream) != OK) {
        return -1;
    }
    fclose(stream);
    return 0;
}