#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

size_t _strlen(char * string) {
    int index = 0;
    while (string[index] != '\0') index++;
    return index;
}

enum status_codes {
    OK,
    NO_MEMORY,
    ZERO_SIZE
};

enum status_codes reverse(char * input_string, char ** output_string) {
    size_t input_string_size = _strlen(input_string);
    if (!input_string_size) return ZERO_SIZE;
    (*output_string) = (char *)malloc(sizeof(char) * (input_string_size + 1));
    if (!(*output_string)) return NO_MEMORY;
    (*output_string)[input_string_size] = '\0';
    for (int i = input_string_size - 1; i >= 0; i--)
        (*output_string)[abs(input_string_size - 1 - i)] = input_string[i];
    return OK;
}

enum status_codes even_uppercase(char * input_string, char ** output_string) {
    size_t input_string_size = _strlen(input_string);
    if (!input_string_size) return ZERO_SIZE;
    (*output_string) = (char *)malloc(sizeof(char) * (input_string_size + 1));
    if (!(*output_string)) return NO_MEMORY;
    (*output_string)[input_string_size] = '\0';
    for (int i = 0; i < input_string_size; i++) {
        (*output_string)[i] = (i % 2 != 0) ? input_string[i] : 
            (input_string[i] >= 'a' && input_string[i] <= 'z') ? (char)((int)input_string[i] - 32) : input_string[i];
    }
    return OK;
}

int comp(const char * sym1, const char * sym2) {
    if (isdigit(*sym1)) {
        if (isdigit(*sym2)) return 0;
        return -1;
    }
    else if (isalpha(*sym1)) {
        if (isdigit(*sym2)) return 1;
        else if (isalpha(*sym2)) return 0;
        return -1;
    }
    
    if (!isdigit(*sym2) && !isalpha(*sym2)) return 0;
    else return 1;
    
}

char * _2concat(char * string1, char * string2) {
    size_t s1_size = _strlen(string1);
    size_t s2_size = _strlen(string2);
    if (!s1_size || !s2_size) return NULL;
    size_t result_size = s1_size + s2_size + 1;
    char * result = (char *)malloc(sizeof(char) * result_size);
    if (!result) return NULL;
    size_t s2_index = 0;
    for (size_t i = 0; i < s1_size; i++) result[i] = string1[i];
    for (size_t i = s1_size; i < result_size - 1; i++) result[i] = string2[s2_index++];
    result[result_size - 1] = '\0';
    return result;
}

void index_array_init(int index_array[], const size_t size, const int seed) {
    srand(seed);
    int unique_flag = 1;
    for (int i = 0; i < size;) {
        unique_flag = 1;
        if (!i) {
            index_array[i] = rand() % size;
            i++;
            continue;
        }
        int new_value = rand() % size;
        for (int j = 0; j < i; j++) {
            if (index_array[j] == new_value) {
                unique_flag = 0;
                break;
            }
        }
        if (!unique_flag) continue;
        index_array[i] = new_value;
        i++;
    }
} 

enum status_codes random_concat(char ** result, char * input_string, char ** to_concat_array, const size_t to_concat_array_size, int index_array[]) {
    if (!_strlen(input_string)) return ZERO_SIZE;
    char * to_concat = to_concat_array[index_array[0]];
    if (!_strlen(to_concat)) return ZERO_SIZE;
    (*result) = _2concat(input_string, to_concat);
    char * tmp = NULL;
    for (int i = 1; i < to_concat_array_size; i++) {
        to_concat = to_concat_array[index_array[i]];
        if (!_strlen(to_concat)) return ZERO_SIZE;
        tmp = (*result);
        (*result) = _2concat((*result), to_concat);
        if (!result) return NO_MEMORY;
        free(tmp);
        tmp = NULL;
    }
    return OK;
}

int main(int argc, char * argv[]) {
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argc < 3) {
            printf("Wrong flag\n");
        }
        switch (argv[1][1]) {
            case 'l':
                if (argc != 3) {
                    printf("Wrong flag\n");
                    return -1;
                }
                printf("%ld\n", _strlen(argv[2]));
                break;
            case 'r':
                if (argc != 3) {
                    printf("Wrong flag\n");
                    return -1;
                }
                char * reversed = NULL;
                switch (reverse(argv[2], &reversed)) {
                    case OK:
                        printf("%s\n", reversed);
                        break;
                    case NO_MEMORY:
                        printf("No memory\n");
                        return -1;
                    case ZERO_SIZE:
                        printf("Unable to reverse string which has zero size\n");
                        return -1;
                }
                free(reversed);
                break;
            case 'u':
                if (argc != 3) {
                    printf("Wrong flag\n");
                    return -1;
                }
                char * uppercase = NULL;
                switch (even_uppercase(argv[2], &uppercase)) {
                    case OK:
                        printf("%s\n", uppercase);
                        break;
                    case NO_MEMORY:
                        printf("No memory\n");
                        return -1;
                    case ZERO_SIZE:
                        printf("Unable to uppercase string which has zero size\n");
                        return -1;
                }
                free(uppercase);
                break;
            case 'n':
                if (argc != 3) {
                    printf("Wrong flag\n");
                    return -1;
                }
                char * to_sort = argv[2];
                qsort(to_sort, _strlen(to_sort), sizeof(char), (int(*)(const void *, const void *))comp);
                printf("%s\n", to_sort);
                break;
            case 'c':
                char * start_string = argv[2];
                int seed = atoi(argv[3]);
                size_t to_concat_array_size = argc - 4;
                char ** to_concat_array = (char **)malloc(sizeof(char *) * (to_concat_array_size));
                for (int i = 0; i < to_concat_array_size; i++) {
                    to_concat_array[i] = argv[4 + i];
                }
                int index_array[to_concat_array_size];
                index_array_init(index_array, to_concat_array_size, seed);
                char * result = NULL;
                switch (random_concat(&result, start_string, to_concat_array, to_concat_array_size, index_array)) {
                    case OK:
                        printf("%s\n", result);
                        break;
                    case NO_MEMORY:
                        printf("No memory\n");
                        return -1;
                    case ZERO_SIZE:
                        printf("Unable to concat strings, because one of them have zero size\n");
                        return -1;
                }
                free(result);
                free(to_concat_array);

        }
    }
    return 0;
}