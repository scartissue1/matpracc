#include <stdio.h>

#include "String.h"

int main(int argc, char * argv[]) {
    String * _1 = NULL;
    String * _2 = NULL;
    switch (init_string(&_1, "abcabc")) {
        case OK:
            printf("first string %s\n", _1->string);
            break;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            return -1;
    }
    switch (init_string(&_2, "123123")) {
        case OK:
            printf("second string %s\n", _2->string);
            break;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            return -1;
    }
    int compare_result = 0;
    if (compare_string(&compare_result, _1, _2) == INVALID_PARAMETER) {
        printf("Invalid parameter\n");
        return -1;
    }
    if (compare_result == 0) printf("%s == %s\n", _1->string, _2->string);
    else if (compare_result == 1) printf("%s > %s\n", _1->string, _2->string);
    else printf("%s < %s\n", _1->string, _2->string);
    switch (copy_string(&_1, _2)) {
        case OK:
            printf("first string after copying from second string is %s\n", _1->string);
            break;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            return -1;

    }
    String * _3 = NULL;
    switch (copy_string_new(&_3, _1)) {
        case OK:
            printf("new third string after copying from first string is %s\n", _3->string);
            break;
        case NO_MEMORY:
            printf("No memory\n");
            break;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            break;
    }
    switch (concat_strings(&_2, _3)) {
        case OK:
            printf("second string after concatenated third is %s\n", _2->string);
            break;
        case NO_MEMORY:
            printf("No memory\n");
            break;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            break;
    }
<<<<<<< HEAD
    free_string(&_1);
    free_string(&_2);
    free_string(&_3);
=======
    free_string(_1);
    free_string(_2);
    free_string(_3);
>>>>>>> 16ef40e (edit)
    return 0;
}