#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int id;
    char surname[BUFSIZ];
    unsigned int salary;
} Employee;

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

int comp(const Employee * first, const Employee * second) {
    if ((*first).salary != (*second).salary) return (*first).salary - (*second).salary;
    if (strcmp((*first).surname, (*second).surname)) return strcmp((*first).surname, (*second).surname);
    return (*first).id - (*second).id;
}

status_codes get_database(FILE * file, Employee ** database, size_t * size, size_t * capacity);

status_codes put_database(FILE * file, Employee * database, const size_t size);

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
    int sort_flag = 0;
    if (argv[2][0] == '-' || argv[2][0] == '/') {
        switch (argv[2][1]) {
            case 'a':
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
    size_t size = 0;
    size_t capacity = 2;

    switch (get_database(in, &database, &size, &capacity)) {
        case OK:
            break;
        case NO_MEMORY:
            printf("No memory\n");
            fclose(out);
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter detected\n");
            fclose(out);
            return -1;
    }
    qsort(database, size, sizeof(Employee), (int(*)(const void *, const void *))comp);
    for (int i = 0; i < size; i++) {
        printf("%u %s %u\n", database[i].id, database[i].surname, database[i].salary);
    }
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
    unsigned int new_id, new_salary;
    char new_surname[BUFSIZ];
    while (fscanf(file, "%u %s %u", &new_id, new_surname, &new_salary) != EOF) {
        if (!strlen(new_surname)) return INVALID_PARAMETER;
        Employee new;
        new.id = new_id;
        strcpy(new.surname, new_surname);
        new.salary = new_salary;

        (*database)[(*size)] = new;
        (*size)++;
        if ((*size) >= (*capacity)) {
            (*capacity) *= 2;
            Employee * tmp;
            if (!(tmp = (Employee *)realloc((*database), (*capacity)))) {
                fclose(file);
                free(*database);
                return NO_MEMORY;
            }
            (*database) = tmp;
        }
    }
    return OK;
}

status_codes put_database(FILE * file, Employee * database, const size_t size) {
    for (int i = 0; i < size; i++)
        fprintf(file, "%u %s %u\n", database[i].id, database[i].surname, database[i].salary);
    return OK;
}