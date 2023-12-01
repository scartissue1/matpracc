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

void tabulation_change(char **substring, const size_t substring_size);

status_codes find_overlap(founds ** result, int * result_size, int * result_capacity, char * substring, size_t substring_size, const char * filename, FILE * file, const int tabulation_count);

status_codes get_file(founds ** result, int * result_size, char * substring, int quantity, ...);

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
            printf("Zero sized string detected\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            break;
    }
    free(result);
    return 0;
}

status_codes get_file(founds ** result, int * result_capacity, char * substring, int quantity, ...) {
    size_t substring_size = _strlen(substring);
    if (!substring_size) {
        return ZERO_SIZE;
    }

    if (quantity < 1 || !substring) return INVALID_PARAMETER;
    (*result_capacity) = 2;
    (*result) = (founds *)malloc(sizeof(founds) * (*result_capacity));
    if (!(*result)) {
        return NO_MEMORY;
    }
    int result_size = 0;
    int tabulation_count = 0;
    for (int i = 0; i < substring_size; i++) {
        if (substring[i] == '\t') {
            tabulation_count++;
        }
    }
    if (tabulation_count) {
        tabulation_change(&substring, substring_size);
        substring_size = _strlen(substring);
    }
    va_list files;
    va_start(files, quantity);

    for (int i = 0; i < quantity; i++) {
        const char * filename = va_arg(files, const char *);
        FILE * file = fopen(filename, "r");
        if (!file) {
            va_end(files);
            free(*result);
            return NO_FILE;
        }
        switch (find_overlap(result, &result_size, result_capacity, substring, substring_size, filename, file, tabulation_count)) {
            case OK:
                break;
            case NO_MEMORY:
                fclose(file);
                va_end(files);
                free(*result);
                return NO_MEMORY;
        }
        fclose(file);
        
    }
    va_end(files);
    return OK;
}

status_codes find_overlap(founds ** result, int * result_size, int * result_capacity, char * substring, size_t substring_size, const char * filename, FILE * file, const int tabulation_count) {
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
            if (substring_index == substring_size && coincedence) {
                founds new;
                new.row = row_coincedence_index;
                new.symbol = symbol_coincedence_index;
                new.filename = (char *)malloc(sizeof(char) * filename_size);
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
                    (*result) = tmp;
                }
                int delta;
                if (tabulation_count) {
                    delta = -substring_size + 4;
                }
                else {
                    delta = -substring_size + 1;
                }
                fseek(file, delta, SEEK_CUR);
                curr_symbol += delta;
                row_coincedence_index = -1;
                symbol_coincedence_index = -1;
                substring_index = 0;
                coincedence = 0;
            }
        }
        else {
            coincedence = 0;
            substring_index = 0;
            if (symbol == substring[substring_index]) {
                substring_index++;
            }
            row_coincedence_index = -1;
            symbol_coincedence_index = -1;
        }
        curr_symbol++;
        if (symbol == '\n') {
            curr_row++;
            curr_symbol = 0;
        }
        symbol = fgetc(file);
    } 
    return OK;
}

void tabulation_change(char **substring, const size_t substring_size) {
    char res[BUFSIZ];
    size_t res_current = 0;
    for (size_t i = 0; i < substring_size; i++) {
        if ((*substring)[i] != '\t') { 
            res[res_current] = (*substring)[i];
            res_current++;
        }
        else {
            strcat(res, "    ");
            res_current += 4;
        }
    }
    (*substring) = res;
}