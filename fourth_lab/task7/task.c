#include "task.h"

int int_validation(const char *number) {
    const size_t size = strlen(number);
    if (!size) {
        return 0;
    }
    if (!isdigit(number[0]) && number[0] != '-' && number[0] != '+') {
        return 0;
    }
    int digit_flag = isdigit(number[0]);
    for (size_t i = 1; i < size; i++) {
        if (!isdigit(number[i])) {
            return 0;
        }
        else {
            digit_flag = 1;
        }
    }
    return (1 && digit_flag);
}

int name_validation(const char *name) {
    size_t name_size = strlen(name);
    for (int i = 0; i < name_size; i++) {
        if (!isalpha(name[i])) {
            return 0;
        }
    }
    return 1;
}