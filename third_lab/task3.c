#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct {
    int id;
    char name[BUFSIZ];
    char surname[BUFSIZ];
    double salary;
} Employee;

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

int name_validation(const char * name) {
    const size_t size = strlen(name);
    for (int i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i])) return 0;
    }
    return 1;
}

int double_validation(const char * number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    size_t index = 0;
    int e_count = 0, _count = 0, dot_count = 0;
    int fl = 0;
    while (index < size) {
        if (number[index] == 'e') e_count++;
        if (number[index] == '-') _count++;
        if (number[index] == '.') dot_count++;
        if (e_count > 1 || _count > 1 || dot_count > 1 || (!isdigit(number[index]) && (number[index] != 'e' || number[index] != '.' || (index != 0 && number[index] != '-')))) return 0;
        if (isdigit(number[index])) fl = 1;
        index++;
    }
    return (1 && fl);
}

int int_validation(const char * number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(number[i]) || (i != 0 && number[i] == '-')) return 0;
    }
    return 1;
}

int comp(const void * first, const void * second) {
    const double eps = 1e-10;
    double salary_difference = ((Employee *)first)->salary - ((Employee *)second)->salary;
    if (fabs(salary_difference) > eps) {
        if (salary_difference > eps) return -1;
        else return 1;
    }
    if (strcmp(((Employee *)second)->surname, ((Employee *)first)->surname)) return strcmp(((Employee *)second)->surname, ((Employee *)first)->surname);
    if (strcmp(((Employee *)second)->name, ((Employee *)first)->name)) return strcmp(((Employee *)second)->name, ((Employee *)first)->name);
    return ((Employee *)first)->id - ((Employee *)second)->id;
}

status_codes get_database(FILE * file, Employee ** database, size_t * size, size_t * capacity);

void put_database(FILE * file, Employee * database, const size_t size, const int sort_flag);

int main(int argc, char * argv[]) {
    if (argc != 4) {
        printf("Wrong flag!\n");
        return -1;
    }
    FILE * in = fopen(argv[1], "r");
    if (!in) {
        printf("No file\n");
        return -1;
    }
    int sort_flag;
    if (argv[2][0] == '-' || argv[2][0] == '/') {
        switch (argv[2][1]) {
            case 'a':
                sort_flag = 0;
                break;
            case 'd':
                sort_flag = 1;
                break;
            default:
                printf("Wrong flag\n");
                fclose(in);
                return -1;
        }
    }
    else {
        printf("Wrong flag\n");
        fclose(in);
        return -1;
    }
    FILE * out = fopen(argv[3], "w");
    if (!out) {
        printf("No file\n");
        fclose(in);
        return -1;
    }
    
    Employee * database = NULL;
    size_t size;
    size_t capacity;

    switch (get_database(in, &database, &size, &capacity)) {
        case OK:
            break;
        case NO_MEMORY:
            printf("No memory\n");
            fclose(in);
            fclose(out);
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter detected\n");
            free(database);
            fclose(in);
            fclose(out);
            return -1;
    }
    qsort(database, size, sizeof(database[0]), comp);
    put_database(out, database, size, sort_flag);
    fclose(in);
    fclose(out);
    free(database);
    return 0;
}

status_codes get_database(FILE * file, Employee ** database, size_t * size, size_t * capacity) {
    (*capacity) = 2;
    (*size) = 0;
    (*database) = (Employee *)malloc(sizeof(Employee) * (*capacity));
    if (!(*database)) return NO_MEMORY;
    char new_id_string[BUFSIZ]; 
    char new_salary_string[BUFSIZ];
    char new_surname[BUFSIZ];
    char new_name[BUFSIZ];
    while (fscanf(file, "%s %s %s %s", new_id_string, new_name, new_surname, new_salary_string) != EOF) {
        if (!strlen(new_surname) || !strlen(new_id_string) || !strlen(new_salary_string) || !strlen(new_name)) return INVALID_PARAMETER;
        Employee new;
        int new_id = atoi(new_id_string);
        if (new_id < 0 || !int_validation(new_id_string)) return INVALID_PARAMETER;
        new.id = new_id;
        if (!name_validation(new_name) || !name_validation(new_surname)) return INVALID_PARAMETER;
        strcpy(new.name, new_surname);
        strcpy(new.surname, new_surname);
        double new_salary = atof(new_salary_string);
        if (new_salary < 0 || !double_validation(new_salary_string)) return INVALID_PARAMETER;
        new.salary = new_salary;

        (*database)[(*size)] = new;
        (*size)++;
        if ((*size) == (*capacity) - 1) {
            (*capacity) *= 2;
            Employee * tmp = (Employee *)realloc((*database), sizeof(Employee) * (*capacity));
            if (tmp == NULL) {
                free(*database);
                return NO_MEMORY;
            }
            (*database) = tmp;
        }
    }
    return OK;
}

void put_database(FILE * file, Employee * database, const size_t size, const int sort_flag) {
    if (!sort_flag) {
        for (int i = 0; i < size; i++)
            fprintf(file, "%d %s %s %lf\n", database[i].id, database[i].name, database[i].surname, database[i].salary);
    }
    else {
        for (int i = size - 1; i >= 0; i--)
            fprintf(file, "%d %s %s %lf\n", database[i].id, database[i].name, database[i].surname, database[i].salary);
    }
}
