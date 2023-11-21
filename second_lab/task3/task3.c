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

status_codes zero_size_exeption(founds ** result, int * result_size, int * result_capacity, const char * filename, FILE * file);

status_codes find_overlap(founds ** result, int * result_size, int * result_capacity, const char * substring, size_t substring_size, const char * filename, FILE * file);

status_codes get_file(founds ** result, int * result_size, const char * substring, int quantity, ...);

int main(int argc, char * argv[]) {
    founds * result = NULL;
    int result_capacity;
    switch (get_file(&result, &result_capacity, "\n", 2, "input1.txt", "input2.txt")) {
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
            printf("Zero sized string contains after: \n");
            for (int i = 0; i < result_capacity; i++) {
                if (result[i].row != 0) {
                    printf("row: %d, index: %d, file: %s\n", result[i].row, result[i].symbol, result[i].filename);
                }
                free(result[i].filename);
            }
            break;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
    }
    free(result);
    return 0;
}

status_codes get_file(founds ** result, int * result_capacity, const char * substring, int quantity, ...) {
    if (quantity < 1 || !substring) return INVALID_PARAMETER;
    (*result_capacity) = 32;
    (*result) = (founds *)malloc(sizeof(founds) * (*result_capacity));
    if (!(*result)) return NO_MEMORY;
    int result_size = 0;

    size_t substring_size = _strlen(substring);

    va_list files;
    va_start(files, quantity);
    int zero_size_flag = 0;

    for (int i = 0; i < quantity; i++) {
        const char * filename = va_arg(files, const char *);
        FILE * file = fopen(filename, "r");
        if (!file) {
            va_end(files);
            return NO_FILE;
        }
        if (!substring_size) {
            switch (zero_size_exeption(result, &result_size, result_capacity, filename, file)) {
                case OK:
                    zero_size_flag = 1;
                    break;
                case NO_MEMORY:
                    return NO_MEMORY;
            }
        }
        switch (find_overlap(result, &result_size, result_capacity, substring, substring_size, filename, file)) {
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
    if (zero_size_flag) return ZERO_SIZE;
    return OK;
}

status_codes find_overlap(founds ** result, int * result_size, int * result_capacity, const char * substring, size_t substring_size, const char * filename, FILE * file) {
    size_t filename_size = _strlen(filename);
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
                founds new;
                new.row = row_coincedence_index;
                new.symbol = symbol_coincedence_index;
                new.filename = (char *)malloc(sizeof(char) * filename_size);
                memcpy(new.filename, filename, filename_size);
                (*result)[(*result_size)] = new;
                (*result_size)++;
                if (*result_size >= *result_capacity) {
                    *result_capacity *= 2;
                    founds * tmp = (founds *)realloc((*result), (*result_capacity));
                    if (!tmp) {
                        free(*result);
                        return NO_MEMORY;
                    }
                    *result = tmp;
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
            curr_symbol = -1;
        }
    } 
    return OK;
}

status_codes zero_size_exeption(founds ** result, int * result_size, int * result_capacity, const char * filename, FILE * file) {
    char symbol = fgetc(file);
    int curr_row = 1;
    int curr_symbol = 0;
    size_t filename_size = _strlen(filename);

    while (symbol != EOF) {
        if ((symbol = fgetc(file)) != EOF) {
            founds new;
            new.row = curr_row;
            new.symbol = curr_symbol;
            new.filename = (char *)malloc(sizeof(char) * filename_size);
            if (!new.filename) return NO_MEMORY;
            memcpy(new.filename, filename, filename_size);
            (*result)[(*result_size)] = new;
            (*result_size)++;
            if (*result_size >= *result_capacity) {
                *result_capacity *= 2;
                founds * tmp = (founds *)realloc((*result), sizeof(founds) * (*result_capacity));
                if (!tmp) {
                    free(*result);
                    return NO_MEMORY;
                }  
                *result = tmp;      
            }
            fseek(file, -1, SEEK_CUR);
        }
        symbol = fgetc(file);
        curr_symbol++;
        if (symbol == '\n') {
            curr_row++;
            curr_symbol = 0;
        }
    }
    return OK;
}