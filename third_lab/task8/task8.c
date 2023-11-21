#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Polynomial.h"

status_codes get_polynomial(Polynomial ** poly, const int start, const int end, const char * command) {
    int x_flag = 0;
    int coef_size = 0;
    int coef_capacity = 2;
    char * strcoef = (char *)malloc(sizeof(char) * coef_capacity);
    if (!strcoef) {
        return NO_MEMORY;
    }
    strcoef[0] = '0';
    strcoef[1] = '\0';
    int coef = 0;
    int power_size = 0;
    int power_capacity = 2;
    char * strpower = (char *)malloc(sizeof(char) * power_capacity);
    if (!strpower) {
        return NO_MEMORY;
    }
    strpower[0] = '0';
    strpower[1] = '\0';
    int power = 0;
    int done = 0;
    int max_power = -1;
    int digit_flag = 0;
    for (int i = start; i < end; i++) {
        //printf("%c", command[i]);
        if (command[i] == 'x') {
            x_flag = 1;
            strcoef[coef_size] = '\0';
            if (!strlen(strcoef)) {
                coef = 1;
            }
            else if (strcmp(strcoef, "-") == 0) {
                coef = -1;
            }
            else {
                coef = atoi(strcoef);
            }
            free(strcoef);
            coef_size = 0;
            coef_capacity = 2;
            strcoef = (char *)malloc(sizeof(char) * coef_capacity);
            if (!strcoef) {
                return NO_MEMORY;
            }
        }
        if (!x_flag && (isdigit(command[i]) || command[i] == '-')) {
            strcoef[coef_size] = command[i];
            coef_size++;
            if (coef_size >= coef_capacity) {
                coef_capacity *= 2;
                char * tmp = NULL;
                if (!(tmp = (char *)realloc(strcoef, coef_capacity))) {
                    free(strcoef);
                    freePolynomial(*poly);
                    return NO_MEMORY;
                }
                strcoef = tmp;
            }
            if (isdigit(command[i])) {
                digit_flag = 1;
            }
            if ((command[i + 1] != 'x') && (!isdigit(command[i + 1])) && digit_flag) {
                if (!(*poly) || max_power < 0) {
                    if (push_forwardPolynomial(poly, atoi(strcoef), 0) == NO_MEMORY) {
                        free(strcoef);
                        free(strpower);
                        freePolynomial(*poly);
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial(*poly, atoi(strcoef), 0) == NO_MEMORY) {
                        free(strcoef);
                        free(strpower);
                        freePolynomial(*poly);
                        return NO_MEMORY;
                    }
                }
                free(strcoef);
                coef_size = 0;
                coef_capacity = 2;
                strcoef = (char *)malloc(sizeof(char) * coef_capacity);
                if (!strcoef) {
                    return NO_MEMORY;
                }
                strcoef[0] = '0';
                strcoef[1] = '\0';
                digit_flag = 0;
                continue;
            }
        }
        if (x_flag) {
            if (command[i + 1] != '^') {
                power = 1;
                done = 1;
            }
            if (!done) {
                i++;
                i++;
                while (isdigit(command[i])) {
                    strpower[power_size] = command[i];
                    power_size++;
                    if (power_size >= power_capacity) {
                        power_capacity *= 2;
                        char * tmp = NULL;
                        if (!(tmp = (char *)realloc(strpower, power_capacity))) {
                            free(strpower);
                            free(strcoef);
                            freePolynomial(*poly);
                            return NO_MEMORY;
                        }
                        strpower = tmp;
                    }
                    i++;
                }
                i--;
                done = 1;
                strpower[power_size] = '\0';
                power = atoi(strpower);
                free(strpower);
                char * strpower = (char *)malloc(sizeof(char) * power_capacity);
                if (!strpower) {
                    return NO_MEMORY;
                }
                strpower[0] = '0';
                strpower[1] = '\0';
            }
        }
        if (done) {
            if (max_power < power || max_power == -1 || !(*poly)) {
                max_power = power;
                if (push_forwardPolynomial(poly, coef, power) == NO_MEMORY) {
                    free(strcoef);
                    free(strpower);
                    freePolynomial(*poly);
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial(*poly, coef, power) == NO_MEMORY) {
                    free(strcoef);
                    free(strpower);
                    freePolynomial(*poly);
                    return NO_MEMORY;
                }
            }
            done = 0;
            x_flag = 0;
            power = 0;
            coef = 0;
        }
    }
    free(strcoef);
    free(strpower);
    return OK;
}

status_codes get_command(FILE * stream) {
    char * command = NULL;
    Polynomial * summator = NULL;
    int long_comment = 0;
    while (getline(&command, &(size_t){0}, stream) != -1) {
        int argument_count = 1;
        size_t size = strlen(command);
        int start_first_polynomial = 0, end_first_polynomial = 0;
        int start_second_polynomial = 0, end_second_polynomial = 0;
        int got_command = 0;
        int comment_end = 0, comment_start = 0, end_command = 0;
        int short_comment_start = 0;
        for (int i = 0; i < size; i++) {
            if (command[i] == '(') {
                if (command[i + 1] == ')') {
                    argument_count = 0;
                    break;
                }
                start_first_polynomial = i + 1;
            }
            if (command[i] == ',') {
                argument_count = 2;
                end_first_polynomial = i;
                start_second_polynomial = i + 1;
            }
            if (command[i] == ')') {
                if (argument_count == 2) {
                    end_second_polynomial = i;
                }
                else {
                    end_first_polynomial = i;
                }
            }
            if (command[i] == ';') {
                end_command = i;
            }
            if (command[i] == '%') {
                short_comment_start = i;
            }
            if (command[i] == '[') {
                if (long_comment) {
                    free(command);
                    return INVALID_PARAMETER;
                }
                long_comment++;
                comment_start = i;

            }
            if (command[i] == ']') {
                long_comment--;
                comment_end = i;
            }
        }
        if ((end_command > comment_start && end_command < comment_end) || (end_command > short_comment_start && (short_comment_start != 0))) {
            free(command);
            command = NULL;
            continue;
        }
        Polynomial * first = NULL;
        Polynomial * second = NULL;
        if (get_polynomial(&first, start_first_polynomial, end_first_polynomial, command) == NO_MEMORY) {
            freePolynomial(second);
            free(command);
            freePolynomial(summator);
            return NO_MEMORY;
        }
        if (argument_count == 2) {
            if (get_polynomial(&second, start_second_polynomial, end_second_polynomial, command) == NO_MEMORY) {
                freePolynomial(first);
                free(command);
                return NO_MEMORY;
            }
        }
        if (command[0] == 'A' && command[1] == 'd' && command[2] == 'd') {
            if (argument_count == 1) {
                Polynomial * tmp = NULL;
                if (copyPolynomial(&tmp, summator) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(summator);
                summator = NULL;
                if (add(&summator, tmp, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(tmp);
                printPolynomial(summator, 1);
            }
            else if (argument_count == 2) {
                freePolynomial(summator);
                summator = NULL;
                if (add(&summator, first, second) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                printPolynomial(summator, 1);
            }
        }
        else if (command[0] == 'S' && command[1] == 'u' && command[2] == 'b') {
            if (argument_count == 1) {
                Polynomial * tmp = NULL;
                if (copyPolynomial(&tmp, summator) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                //printPolynomial(tmp, 1);
                //printPolynomial(first, 1);
                freePolynomial(summator);
                summator = NULL;
                if (sub(&summator, tmp, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(tmp);
                printPolynomial(summator, 1);
            }
            else if (argument_count == 2) {
                freePolynomial(summator);
                summator = NULL;
                if (sub(&summator, first, second) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                printPolynomial(summator, 1);
            }
        }
        else if (command[0] == 'M' && command[1] == 'u' && command[2] == 'l' && command[3] == 't') {
            if (argument_count == 1) {
                Polynomial * tmp = NULL;
                if (copyPolynomial(&tmp, summator) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(summator);
                summator = NULL;
                if (mult(&summator, tmp, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(tmp);
                printPolynomial(summator, 1);
            }
            else if (argument_count == 2) {
                freePolynomial(summator);
                summator = NULL;
                if (mult(&summator, first, second) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                printPolynomial(summator, 1);
            }
        }
        else if (command[0] == 'D' && command[1] == 'i' && command[2] == 'v') {
            if (argument_count == 1) {
                Polynomial * tmp = NULL;
                if (copyPolynomial(&tmp, summator) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(summator);
                summator = NULL;
                Polynomial * no_use = NULL;
                if (!tmp) {
                    printPolynomial(summator, 1);
                    continue;
                }
                if (copyPolynomial(&no_use, second) == NO_MEMORY) {
                    return NO_MEMORY;
                }
                int val_flag = 0;
                while (no_use) {
                    if (no_use->coefficient != 0) {
                        val_flag = 1;
                        break;
                    }
                    no_use = no_use->next; 
                }
                if (!val_flag) {
                    printf("can't divide by zero\n");
                    continue;
                }
                if (divmod(&summator, &no_use, tmp, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(tmp);
                freePolynomial(no_use);
                printPolynomial(summator, 1);
            }
            else if (argument_count == 2) {
                freePolynomial(summator);
                summator = NULL;
                Polynomial * no_use = NULL;
                if (copyPolynomial(&no_use, second) == NO_MEMORY) {
                    return NO_MEMORY;
                }
                int val_flag = 0;
                while (no_use) {
                    if (no_use->coefficient != 0) {
                        val_flag = 1;
                        break;
                    }
                    no_use = no_use->next; 
                }
                if (!val_flag) {
                    printf("can't divide by zero\n");
                    continue;
                }
                if (divmod(&summator, &no_use, first, second) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(no_use);
                printPolynomial(summator, 1);
            }
        }
        else if (command[0] == 'M' && command[1] == 'o' && command[2] == 'd') {
            if (argument_count == 1) {
                Polynomial * tmp = NULL;
                if (copyPolynomial(&tmp, summator) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(summator);
                summator = NULL;
                Polynomial * no_use = NULL;
                if (!tmp) {
                    printPolynomial(summator, 1);
                    continue;
                }
                if (copyPolynomial(&no_use, first) == NO_MEMORY) {
                    return NO_MEMORY;
                }
                int val_flag = 0;
                while (no_use) {
                    if (no_use->coefficient != 0) {
                        val_flag = 1;
                        break;
                    }
                    no_use = no_use->next; 
                }
                if (!val_flag) {
                    printf("can't divide by zero\n");
                    continue;
                }
                if (divmod(&no_use, &summator, tmp, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(tmp);
                freePolynomial(no_use);
                printPolynomial(summator, 1);
            }
            else if (argument_count == 2) {
                freePolynomial(summator);
                summator = NULL;
                Polynomial * no_use = NULL;
                if (copyPolynomial(&no_use, second) == NO_MEMORY) {
                    return NO_MEMORY;
                }
                int val_flag = 0;
                while (no_use) {
                    if (no_use->coefficient != 0) {
                        val_flag = 1;
                        break;
                    }
                    no_use = no_use->next; 
                }
                if (!val_flag) {
                    printf("can't divide by zero\n");
                    continue;
                }
                if (divmod(&no_use, &summator, first, second) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(no_use);
                printPolynomial(summator, 1);
            }
        }
        else if ((command[0] == 'E' && command[1] == 'v' && command[2] == 'a' && command[3] == 'l') ||
        (command[0] == 'C' && command[1] == 'm' && command[2] == 'p' && command[3] == 's')) {
            if (argument_count == 1) {
                Polynomial * tmp = NULL;
                if (copyPolynomial(&tmp, summator) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(summator);
                summator = NULL;
                if (cmps(&summator, tmp, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(tmp);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                freePolynomial(tmp);
                printPolynomial(summator, 1);
            }
            else if (argument_count == 2) {
                freePolynomial(summator);
                summator = NULL;
                if (cmps(&summator, first, second) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
                printPolynomial(summator, 1);
            }
        }
        else if (command[0] == 'D' && command[1] == 'i' && command[2] == 'f' && command[3] == 'f') {
            if (argument_count == 0) {
                diff(summator);
                printPolynomial(summator, 1);
            }
            else {
                freePolynomial(summator);
                summator = NULL;
                diff(first);
                printPolynomial(first, 1);
                if (copyPolynomial(&summator, first) == NO_MEMORY) {
                    freePolynomial(summator);
                    freePolynomial(first);
                    freePolynomial(second);
                    free(command);
                    return NO_MEMORY;
                }
            }
        }
        freePolynomial(first);
        freePolynomial(second);
        free(command);
    }
    freePolynomial(summator);
    if (command) {
        free(command);
    }
    return OK;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    FILE * file = fopen(argv[1], "r");
    switch (get_command(file)) {
        case OK: break;
        case NO_MEMORY:
            fclose(file);
            return -1;
    }
    fclose(file);
    return 0;
}
