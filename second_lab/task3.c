#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

size_t _strlen(const char * string) {
    size_t size = 0;
    while (string[size++] != '\0');
    return size - 1;
}

typedef enum {
    OK,
    NO_FILE,
    NO_MEMORY,
    INVALID_PARAMETER,
    ZERO_SIZE
} status_codes;

typedef struct {
    int row;
    int symbol;
    char * filename;
} founds;

status_codes find_overlap(founds ** result, int * result_size, int * result_capacity, const char * substring, size_t substring_size, const char * filename, FILE * file);

status_codes get_file(founds ** result, int * result_size, const char * substring, int quantity, ...);

int main(int argc, char * argv[]) {
    founds * result = NULL;
    int result_capacity;
    switch (get_file(&result, &result_capacity, "", 2, "input1.txt", "input2.txt")) {
        case OK:
            for (int i = 0; i < result_capacity; i++) {
                if (result[i].row != 0) {
                    printf("row: %d, index: %d, file: %s\n", result[i].row, result[i].symbol, result[i].filename);
                }
                free(result[i].filename);
            }
            break;
        case NO_FILE:
            printf("No such file or directory\n");
            return -1;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case ZERO_SIZE:
            printf("Zero sized string\n");
            break;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
    }
    free(result);
    return 0;
}

status_codes get_file(founds ** result, int * result_capacity, const char * substring, int quantity, ...) {
    if (quantity < 1 || !substring) return INVALID_PARAMETER;
    size_t substring_size = _strlen(substring);
    if (!substring_size) return ZERO_SIZE;
    (*result_capacity) = 32;
    (*result) = (founds *)malloc(sizeof(founds) * (*result_capacity));
    if (!(*result)) return NO_MEMORY;
    int result_index = 0;

    va_list files;
    va_start(files, quantity);

    for (int i = 0; i < quantity; i++) {
        const char * filename = va_arg(files, const char *);
        FILE * file = fopen(filename, "r");
        if (!file) {
            va_end(files);
            return NO_FILE;
        }
        switch (find_overlap(result, &result_index, result_capacity, substring, substring_size, filename, file)) {
            case OK:
                break;
            case NO_MEMORY:
                fclose(file);
                va_end(files);
                break;
        }
        fclose(file);
        
    }
    va_end(files);
    return OK;
}

status_codes find_overlap(founds ** result, int * result_size, int * result_capacity, const char * substring, size_t substring_size, const char * filename, FILE * file) {
    int substring_index = 0;
    char symbol = fgetc(file);
    int coincedence = 0;
    int row_coincedence_index = -1;
    int symbol_coincedence_index = -1;

    int curr_row = 1;
    int curr_symbol = 0;

    while (symbol != EOF) {
        if (symbol == substring[substring_index]) {
            if (!coincedence) {
                coincedence = 1;
                row_coincedence_index = curr_row;
                symbol_coincedence_index = curr_symbol;
            }
            substring_index++;
            if (substring_index == substring_size) {
                founds t;
                t.row = row_coincedence_index;
                t.symbol = symbol_coincedence_index;
                t.filename = strdup(filename);
                (*result)[(*result_size)] = t;
                (*result_size)++;
                if (*result_size >= *result_capacity) {
                    *result_capacity *= 2;
                    founds * tmp = (founds *)realloc((*result), (*result_capacity));
                    if (!tmp) {
                        free(*result);
                        return NO_MEMORY;
                    }
                }
                fseek(file, -substring_size + 1, SEEK_CUR);
                curr_symbol += -substring_size + 1;
                row_coincedence_index = -1;
                symbol_coincedence_index = -1;
                substring_index = 0;
                coincedence = 0;
            }
        }
        else {
            coincedence = 0;
            substring_index = 0;
            row_coincedence_index = -1;
            symbol_coincedence_index = -1;
        }
        curr_symbol++;
        symbol = fgetc(file);
        if (symbol == '\n') {
            curr_row++;
            curr_symbol = 0;
        }
    } 
    return OK;
}