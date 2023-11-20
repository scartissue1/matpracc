#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "task.h"


status_codes push_backString(char **string, size_t *size, size_t *capacity, char symbol) {
    (*string)[*size] = symbol;
    (*size)++;
    if ((*size) >= (*capacity)) {
        (*capacity) *= 2;
        char *tmp = NULL;
        if (!(tmp = (char *)realloc((*string), sizeof(char) * (*capacity)))) {
            return NO_MEMORY;
        }
        (*string) = tmp;
    }
    return OK;
}

status_codes deleteString(char **string, size_t *size, size_t *capacity) {
    (*size) = 0;
    (*capacity) = 2;
    char *tmp = NULL;
    if (!(tmp = (char *)realloc((*string), sizeof(char) * (*capacity)))) {
        return NO_MEMORY;
    }
    (*string) = tmp;
    return OK;
}

status_codes changeFile(const char *filename) {
    FILE *stream = fopen(filename, "r");
    if (!stream) {
        return NO_FILE;
    }
    char tmp_name[5];
    FILE *tmp;
    if (strcmp(filename, "tmp") != 0) {
        strcpy(tmp_name, "tmp");
        tmp = fopen(tmp_name, "w");
    }
    else {
        strcpy(tmp_name, "tmp1");
        tmp = fopen(tmp_name, "w");
    }
    HashTable *hash = NULL;
    if (createTable(&hash, 128, hashFunction) == NO_MEMORY) {
        rename(tmp_name, filename);
        fclose(tmp);
        fclose(stream);
        return NO_MEMORY;
    }
    char symbol = fgetc(stream);
    size_t word_size = 0;
    size_t word_capacity = 2;
    char *word = (char *)malloc(sizeof(char) * word_capacity);
    if (!word) {
        rename(tmp_name, filename);
        fclose(tmp);
        fclose(stream);
        freeTable(hash);
        return NO_MEMORY;
    }
    char key[BUFSIZ];
    char value[BUFSIZ];
    int define_flag = 0;
    while (symbol != EOF) {
        if (define_flag == 2) {
            while (symbol != EOF && symbol != '\n') {
                if (push_backString(&word, &word_size, &word_capacity, symbol) == NO_MEMORY) {
                    free(word);
                    rename(tmp_name, filename);
                    fclose(tmp);
                    fclose(stream);
                    freeTable(hash);
                    return NO_MEMORY;
                }
                symbol = fgetc(stream);
            }
            word[word_size] = '\0';
            strcpy(value, word);
            if (!value) {
                free(word);
                rename(tmp_name, filename);
                fclose(tmp);
                fclose(stream);
                freeTable(hash);
                return NO_MEMORY;
            }
            
            if (insertTable(hash, key, value, 0, -1) == NO_MEMORY) {
                free(word);
                rename(tmp_name, filename);
                fclose(tmp);
                fclose(stream);
                freeTable(hash);
                return NO_MEMORY;
            }
            define_flag = 0;
            fprintf(tmp, "%s", value);
            if (symbol != EOF) {
                fputc(symbol, tmp);
            }
            if (deleteString(&word, &word_size, &word_capacity) == NO_MEMORY) {
                free(word);
                rename(tmp_name, filename);
                fclose(tmp);
                fclose(stream);
                freeTable(hash);
                return NO_MEMORY;
            }
            symbol = fgetc(stream);
            continue;
        }
        if (symbol == ' ' || symbol == '\t' || symbol == '\n') {
            word[word_size] = '\0';
            if (strcmp(word, "#define") == 0 && define_flag == 0) {
                if (!define_flag) {
                    define_flag = 1;
                }
                else {
                    return INVALID_PARAMETER;
                }
                fprintf(tmp, "%s", word);
                fputc(symbol, tmp);
            }
            else if (define_flag == 1) {
                strcpy(key, word);
                define_flag = 2;
                fprintf(tmp, "%s", word);
                fputc(symbol, tmp);
            }
            else {
                char *found = findTable(hash, word);
                if (!found) {
                    fprintf(tmp, "%s", word);
                }
                else {
                    fprintf(tmp, "%s", found);
                }
                fputc(symbol, tmp);
            }
            if (deleteString(&word, &word_size, &word_capacity) == NO_MEMORY) {
                free(word);
                rename(tmp_name, filename);
                fclose(tmp);
                fclose(stream);
                freeTable(hash);
                return NO_MEMORY;
            }
        }
        else {
            if (push_backString(&word, &word_size, &word_capacity, symbol) == NO_MEMORY) {
                free(word);
                rename(tmp_name, filename);
                fclose(tmp);
                fclose(stream);
                freeTable(hash);
                return NO_MEMORY;
            }
        }
        symbol = fgetc(stream);

    }
    if (word) {
        word[word_size] = '\0';

        char *found = findTable(hash, word);
        if (!found) {
            fprintf(tmp, "%s", word);
        }
        else {
            fprintf(tmp, "%s", found);
        }
    }
    freeTable(hash);
    free(word);
    rename(tmp_name, filename);
    fclose(stream);
    fclose(tmp);
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    switch(changeFile(argv[1])) {
        case OK:
            break;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter detected\n");
            return -1;
        case NO_FILE:
            printf("No such file\n");
            return -1;
    }
    return 0;
}