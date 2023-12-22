#include <stdlib.h>
#include <string.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

typedef struct {
    char * string;
    size_t size;
} String;

status_codes init_string(String ** str, const char * new_string) {
    size_t new_string_size = strlen(new_string);
    if (!new_string) return INVALID_PARAMETER;
    (*str) = (String *)malloc(sizeof(String));
    if (!(*str)) return NO_MEMORY;
    (*str)->size = new_string_size;
    (*str)->string = (char *)malloc(sizeof(char) * (*str)->size);
    if (!(*str)->string) {
        free(*str);
        return NO_MEMORY;
    }
    memcpy((*str)->string, new_string, new_string_size);
    return OK;
}

status_codes free_string(String ** str) {
    if (!(*str) || !(*str)->string) return INVALID_PARAMETER;
    free((*str)->string);
    (*str)->string = NULL;
    (*str)->size = 0;
    free(*str);
    *str = NULL;
    return OK;
}

status_codes compare_string(int * result, const String * str1, const String * str2) {
    if (!str1 || !str2 || !str1->string || !str2->string) return INVALID_PARAMETER;
    if (str1->size > str2->size) {
        (*result) = 1;
        return OK;
    }
    if (str2->size > str1->size) {
        (*result) = -1;
        return OK;
    }
    for (int i = 0; i < str1->size; i++) {
        if (str1->string[i] - str2->string[i] > 0) {
            (*result) = 1;
            return OK;
        }
        if (str1->string[i] - str2->string[i] < 0) {
            (*result) = -1;
            return OK;
        }
    }
    (*result) = 0;
    return OK;
}

status_codes copy_string(String ** destination, const String * source) {
    if (!source || !source->string) return INVALID_PARAMETER;
    if (free_string(destination) == INVALID_PARAMETER) return INVALID_PARAMETER;
    return init_string(destination, source->string);
}

status_codes copy_string_new(String ** destination, const String * source) {
    if (*destination || !source || !source->string) return INVALID_PARAMETER;
    return init_string(destination, source->string);
}

status_codes concat_strings(String ** str1, String * str2) {
    if (!(*str1) || !str2 || !(*str1)->string || !str2->string) return INVALID_PARAMETER;
    size_t new_size = (*str1)->size + str2->size + 1;
    char * tmp = (char *)realloc((*str1)->string, sizeof(char) * new_size);
    if (!tmp) {
        free_string(str1);
        free_string(&str2);
        return NO_MEMORY;
    }
    (*str1)->string = tmp;
    for (int i = 0; i < str2->size; i++) (*str1)->string[i + (*str1)->size] = str2->string[i];
    (*str1)->string[new_size] = '\0';
    (*str1)->size = new_size;
    return OK;
}
