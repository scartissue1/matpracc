#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "task.h"
#include "Array.h"
#include "BST.h"
#include "command.h"

void errorProcessing(status_codes error) {
    if (error == NO_MEMORY) {
        printf("No memory\n");
    }
    else if (error == INVALID_INPUT) {
        printf("Invalid input\n");
    }
    else if (error == NO_FILE) {
        printf("No such file\n");
    }
    else if (error == OUT_OF_BOUNDS) {
        printf("Out of bounds of array\n");
    }
}

void to_lower(char *string) {
    size_t string_size = strlen(string);
    for (int i = 0; i < string_size; i++) {
        string[i] = tolower(string[i]);
    }
}

status_codes makeCommand(char *command, Node **root) {
    const char *separators = " ,()\n\t;";
    char *token = strtok(command, separators);
    to_lower(token);
    if (strcmp(token, "load") == 0 || strcmp(token, "save") == 0) {
        int command_type = 1;
        if (strcmp(token, "load") == 0) {
            command_type = 0;
        }
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (!token) {
            return INVALID_INPUT;
        }
        char *filename = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        if (!filename) {
            return NO_MEMORY;
        }
        strcpy(filename, token);
        status_codes status;
        if (!command_type) {
            status = loadProcessing(root, filename, array_name);
        }
        else {
            status = saveProcessing((*root), filename, array_name);
        }
        free(filename);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return status;
    }
    else if (strcmp(token, "rand") == 0) {
        char array_name;
        int count = 0, left_bound = 0, right_bound = 0;
        for (int i = 0; i < 4; i++) {
            token = strtok(NULL, separators);
            if (!token) {
                return INVALID_INPUT;
            }
            switch (i) {
                case 0:
                    if (strlen(token) != 1 || !isalpha(token[0])) {
                        return INVALID_INPUT;
                    }
                    array_name = toupper(token[0]);
                    break;
                case 1:
                    count = atoi(token);
                    if (!int_validation(token) || count < 0) {
                        return INVALID_INPUT;
                    }
                    break;
                case 2:
                    left_bound = atoi(token);
                    if (!int_validation(token) || left_bound < 0) {
                        return INVALID_INPUT;
                    }
                    break;
                case 3:
                    right_bound = atoi(token);
                    if (!int_validation(token) || right_bound < 0) {
                        return INVALID_INPUT;
                    }
                    break;
            }
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return randProcessing(root, array_name, count, left_bound, right_bound);
    }
    else if (strcmp(token, "concat") == 0) {
        char src_name, dest_name;
        for (int i = 0; i < 2; i++) {
            token = strtok(NULL, separators);
            if (!token || strlen(token) != 1 || !isalpha(token[0])) {
                return INVALID_INPUT;
            }
            if (i == 0) {
                dest_name = toupper(token[0]);
            }
            else {
                src_name = toupper(token[0]);
            }

        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return concatProcessing(root, dest_name, src_name);
    }
    else if (strcmp(token, "free") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) > 1) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return freeProcessing((*root), array_name);
    }
    else if (strcmp(token, "remove") == 0) {
        char array_name;
        int start = 0, count = 0;
        for (int i = 0; i < 3; i++) {
            token = strtok(NULL, separators);
            if (!token) {
                return INVALID_INPUT;
            }
            if (i == 0) {
                if (strlen(token) != 1 || !isalpha(token[0])) {
                    return INVALID_INPUT;
                }
                array_name = toupper(token[0]);
            }
            else if (i == 1) {
                start = atoi(token);
                if (!int_validation(token) || start < 0) {
                    return INVALID_INPUT;
                }
            }
            else {
                count = atoi(token);
                if (!int_validation(token) || count < 0) {
                    return INVALID_INPUT;
                }
            }
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        removeProcessing((*root), array_name, start, count);
    }
    else if (strcmp(token, "copy") == 0) {
        int start = 0, end = 0;
        char src_name, dest_name;
        for (int i = 0; i < 4; i++) {
            token = strtok(NULL, separators);
            if (!token) {
                return INVALID_INPUT;
            }
            if (i == 0) {
                if (strlen(token) > 1) {
                    return INVALID_INPUT;
                }
                src_name = toupper(token[0]);
            }
            else if (i == 1) {
                start = atoi(token);
                if (!int_validation(token) || start < 0) {
                    return INVALID_INPUT;
                }
            }
            else if (i == 2) {
                end = atoi(token);
                if (!int_validation(token) || end < 0) {
                    return INVALID_INPUT;
                }
            }
            else {
                if (strlen(token) > 1) {
                    return INVALID_INPUT;
                }
                dest_name = toupper(token[0]);
            }
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return copyProcessing(root, src_name, dest_name, start, end);
    }
    else if (strcmp(token, "sort") == 0) {
        token = strtok(NULL, separators);
        if (!token) {
            return INVALID_INPUT;
        }
        if (strlen(token) != 2 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        int sort_flag = 0;
        if (token[1] == '+') {
            sort_flag = 0;
        }
        else if (token[1] == '-'){
            sort_flag = 1;
        }
        else {
            return INVALID_INPUT;
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return sortProcessing((*root), array_name, sort_flag);
    }
    else if (strcmp(token, "shuffle") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return sortProcessing((*root), array_name, 2);
    }
    else if (strcmp(token, "stats") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return statsProcessing((*root), array_name);
    }
    else if (strcmp(token, "print") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (strcmp(token, "all") == 0) {
            token = strtok(NULL, separators);
            if (token) {
                return INVALID_INPUT;
            }
            return printProcessing((*root), array_name, -1, -1);
        }
        int left_bound = atoi(token);
        if (!int_validation(token) && left_bound < 0) {
            return INVALID_INPUT;
        }
        token = strtok(NULL, separators);
        if (!token) {
            return printProcessing((*root), array_name, left_bound, left_bound);
        }
        int right_bound = atoi(token);
        if (!int_validation(token) && right_bound < 0) {
            return INVALID_INPUT;
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return printProcessing((*root), array_name, left_bound, right_bound);
    }
    else {
        return INVALID_INPUT;
    }
}


status_codes getCommand(FILE *programm) {
    Node *root = NULL;
    char symbol = fgetc(programm);
    while (symbol == ' ' || symbol == '\t' || symbol == '\n') {
        symbol = fgetc(programm);
    }
    size_t command_size = 0;
    size_t command_capacity = 2;
    char *command = (char *)malloc(sizeof(char) * command_capacity);
    if (!command) {
        return NO_MEMORY;
    }
    int command_flag = 0;
    while (symbol != EOF) {
        command_flag = 0;
        command[command_size] = symbol;
        command_size++;
        if (command_size >= command_capacity) {
            command_capacity *= 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc(command, (sizeof(char) * command_capacity)))) {
                free(command);
                freeNode(root);
                return NO_MEMORY;
            }
            command = tmp;
        }
        if (symbol == ';') {
            command[command_size] = '\0';
            status_codes status = makeCommand(command, &root);
            if (status != OK) {
                errorProcessing(status);
                freeNode(root);
                free(command);
                return status;
            }
            char *tmp = NULL;
            command_size = 0;
            command_capacity = 2;
            if (!(tmp = (char *)realloc(command, sizeof(char) * command_capacity))) {
                free(command);
                return NO_MEMORY;
            }
            command = tmp;
            symbol = fgetc(programm);
            while (symbol == ' ' || symbol == '\n' || symbol == '\t') {
                symbol = fgetc(programm);
            }
            command_flag = 1;
        }
        if (!command_flag) {
            symbol = fgetc(programm);
        }
    }
    free(command);
    freeNode(root);
    return OK;
}


int main(int argc, char *argv[]) {
    FILE *programm = fopen(argv[1], "r");
    if (!programm) {
        printf("No such file\n");
        return -1;
    }
    if (getCommand(programm) == NO_MEMORY) {
        printf("No memory\n");
        return -1;
    }
    fclose(programm);
    return 0;
}
