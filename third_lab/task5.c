#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char name[BUFSIZ];
    char surname[BUFSIZ];
    char group[BUFSIZ];
    unsigned char * marks;
} Student;

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

int int_validation(const char * number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(number[i]) || (i != 0 && number[i] == '-')) return 0;
    }
    return 1;
}

int id_comp(const void * first, const void * second);

int surname_comp(const void * first, const void * second);

int name_comp(const void * first, const void * second);

int group_comp(const void * first, const void * second);

void id_finder(Student * database, size_t size, int to_find, int * result, int sort_flag);

status_codes other_finder(Student * database, size_t size, char * to_find, int ** result, size_t * result_size, int find_flag);

status_codes get_database(FILE * file, Student ** database, size_t * size, size_t * capacity, double * summ, double * quantity);

void print_menu();

status_codes communicate(FILE * file, Student * database, size_t size, double ovr_avg);

int main(int argc, char * argv[]) {
    if (argc != 3) {
        printf("Wrong flag\n");
        return -1;
    }
    FILE * in = fopen(argv[1], "r");
    if (!in) {
        printf("No file\n");
        return -1;
    }
    FILE * out = fopen(argv[2], "w");
    if (!out) {
        printf("No file\n");
        fclose(in);
        return -1;
    }
    Student * database = NULL;
    size_t size, capacity;
    double ovr_summ = 0;
    double ovr_quantity = 0;
    switch (get_database(in, &database, &size, &capacity, &ovr_summ, &ovr_quantity)) {
        case OK:
            break;
        case NO_MEMORY:
            fclose(in);
            fclose(out);
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            fclose(in);
            fclose(out);
            free(database);
            printf("Invalid parameter detected\n");
            return -1;

    }
    double ovr_avg = ovr_summ / ovr_quantity;
    if (communicate(out, database, size, ovr_avg) == NO_MEMORY) {
        free(database);
        fclose(in);
        printf("No memory\n");
        return -1;
    }
    free(database);
    fclose(in);
    fclose(out);
    return 0;
}

status_codes communicate(FILE * file, Student * database, size_t size, double ovr_avg) {
    print_menu();
    int sort_flag = 0;
    while (1) {
        char command[BUFSIZ];
        scanf("%s", command);
        if (strcmp(command, "find") == 0) {
            char flag[BUFSIZ];
            scanf("%s", flag);
            if (strcmp(flag, "id") == 0) {
                int to_find;
                int result_id = -1;
                scanf("%d", &to_find);
                id_finder(database, size, to_find, &result_id, sort_flag);
                if (result_id != -1) {
                    fprintf(file, "%d %s %s %s ", database[result_id].id, database[result_id].name, database[result_id].surname, database[result_id].group);
                    for (int i = 0; i < 5; i++) fprintf(file, "%c", database[result_id].marks[i]);
                    fprintf(file, "\n");
                }
                fprintf(file, "\n");
            }
            else if (strcmp(flag, "surname") == 0 || strcmp(flag, "name") == 0 || strcmp(flag, "group") == 0) {
                char to_find[BUFSIZ];
                scanf("%s", to_find);
                int * result = NULL;
                size_t result_size = 0;
                if (other_finder(database, size, to_find, &result, &result_size, (strcmp(flag, "surname") == 0) ? 0 : (strcmp(flag, "name") == 0) ? 1 : 2) == NO_MEMORY) {
                    fclose(file);
                    return NO_MEMORY;
                }
                for (int i = 0; i < result_size; i++) {
                    fprintf(file, "%d %s %s %s ", database[result[i]].id, database[result[i]].name, database[result[i]].surname, database[result[i]].group);
                    for (int j = 0; j < 5; j++) fprintf(file, "%c", database[result[i]].marks[j]);
                    fprintf(file, "\n");
                }
                fprintf(file, "\n");
                free(result);
                result = NULL;
            }
            
        }
        else if (strcmp(command, "sort") == 0) {
            char flag[BUFSIZ];
            scanf("%s", flag);
            if (strcmp(flag, "id") == 0) {
                qsort(database, size, sizeof(database[0]), id_comp);
                sort_flag = 1;
            }
            else {
                if (strcmp(flag, "surname") == 0) {
                    qsort(database, size, sizeof(database[0]), surname_comp);
                }
                else if (strcmp(flag, "name") == 0) {
                    qsort(database, size, sizeof(database[0]), name_comp);
                }
                else if (strcmp(flag, "grop") == 0) {
                    qsort(database, size, sizeof(database[0]), group_comp);
                }
                sort_flag = 0;
            }
        }
        else if (strcmp(command, "get_stat") == 0) {
            int to_find;
            int result_id = -1;
            scanf("%d", &to_find);
            id_finder(database, size, to_find, &result_id, sort_flag);
            if (result_id != -1) {
                fprintf(file, "%d %s %s %s ", database[result_id].id, database[result_id].name, database[result_id].surname, database[result_id].group);
                double summ = 0;
                double quantity = 0;
                for (int i = 0; i < 5; i++) {
                    summ += database[result_id].marks[i] - '0';
                    quantity++;
                }
                fprintf(file, "%f\n\n", summ / quantity);
            }
        }
        else if (strcmp(command, "find_best") == 0) {
            for (int i = 0; i < size; i++) {
                double summ = 0;
                double quantity = 0;
                for (int j = 0; j < 5; j++) {
                    summ += database[i].marks[j] - '0';
                    quantity++;
                }
                double avg = summ / quantity;
                if (avg > ovr_avg) fprintf(file, "%d %s %s %s %f\n", database[i].id, database[i].name, database[i].surname, database[i].group, avg);
            }
        }
        else if (strcmp(command, "print") == 0) {
            for (int i = 0; i < size; i++) {
                fprintf(file, "%d %s %s %s ", database[i].id, database[i].name, database[i].surname, database[i].group);
                for (int j = 0; j < 5; j++) fprintf(file, "%c", database[i].marks[j]);
                fprintf(file, "\n\n");
            }
        }
        else if (strcmp(command, "exit") == 0) break;

    }
    return OK;
}

status_codes get_database(FILE * file, Student ** database, size_t * size, size_t * capacity, double * summ, double * quantity) {
    (*size) = 0;
    (*capacity) = 2;
    (*database) = (Student *)malloc(sizeof(Student) * (*capacity));
    if (!(*database)) return NO_MEMORY;
    char new_id_string[BUFSIZ];
    char new_name[BUFSIZ];
    char new_surname[BUFSIZ];
    char new_group[BUFSIZ];
    unsigned char * new_marks = (unsigned char *)malloc(sizeof(unsigned char) * 5);
    if (!new_marks) return NO_MEMORY;
    while (fscanf(file, "%s %s %s %s %c %c %c %c %c", new_id_string, new_name, new_surname, new_group, &new_marks[0], &new_marks[1], &new_marks[2], &new_marks[3], &new_marks[4]) != EOF) {
        if (!strlen(new_id_string) || !strlen(new_name) || !strlen(new_surname) || !strlen(new_group)) return INVALID_PARAMETER;
        int new_id = atoi(new_id_string);
        if (new_id < 0 || !int_validation(new_id_string) || !name_validation(new_name) || !name_validation(new_surname)) return INVALID_PARAMETER;
        int double_id_check = -1;
        id_finder((*database), (*size) + 1, new_id, &double_id_check, 0);
        if (double_id_check != -1) return INVALID_PARAMETER;
        for (int i = 0; i < 5; i++) {
            if (!('2' <= new_marks[i] && new_marks[i] <= '5')) return INVALID_PARAMETER;
            (*summ) += new_marks[i] - '0';
            (*quantity)++;
        }
        Student new;
        new.id = new_id;
        strcpy(new.name, new_name);
        strcpy(new.surname, new_surname);
        strcpy(new.group, new_group);
        new.marks = (unsigned char *)malloc(sizeof(unsigned char) * 5);
        memcpy(new.marks, new_marks, 5);
        free(new_marks);
        new_marks = (unsigned char *)malloc(sizeof(unsigned char) * 5);
        (*database)[(*size)] = new;
        (*size)++;
        if ((*capacity) - 1 == (*size)) {
            (*capacity) *= 2;
            Student * tmp = (Student *)realloc((*database), sizeof(Student) * (*capacity));
            if (!tmp) {
                free(*database);
                return NO_MEMORY;
            }
            (*database) = tmp;
        }
    }
    if (new_marks) free(new_marks);
    return OK;
}

void id_finder(Student * database, size_t size, int to_find, int * result, int sort_flag) {
    if (sort_flag) {
        int left = 0, right = size - 1;
        while (left < right) {
            int median = (left + right) / 2;
            if (database[median].id > to_find) left = median + 1;
            else if (database[median].id < to_find) right = median;
            else {
                (*result) = median;
                break;
            }
        }
        
    }
    else {
        for (int i = 0; i < size; i++) {
            if (database[i].id == to_find) {
                (*result) = i;
                break;
            }
        }
    }
    
}

status_codes other_finder(Student * database, size_t size, char * to_find, int ** result, size_t * result_size, int find_flag) {
    (*result_size) = 0;
    size_t capacity = 2;
    (*result) = (int *)malloc(sizeof(int) * capacity);
    if (!result) return NO_MEMORY;
    for (int i = 0; i < size; i++) {
        if (strcmp((find_flag == 0) ? database[i].surname : (find_flag == 1) ? database[i].name : database[i].group, to_find) == 0) {
            (*result)[(*result_size)] = i;
            (*result_size)++;
            if ((*result_size) == capacity - 1) {
                capacity *= 2;
                int * tmp = NULL;
                if (!(tmp = (int *)realloc((*result), sizeof(int) * capacity))) {
                    free(*result);
                    return NO_MEMORY;
                }
                (*result) = tmp;
            }
        }
    }
    return OK;
}


int id_comp(const void * first, const void * second) {
    return ((Student *)first)->id - ((Student *)second)->id;
}

int surname_comp(const void * first, const void * second) {
    return strcmp(((Student *)first)->surname, ((Student *)second)->surname);
}

int name_comp(const void * first, const void * second) {
    return strcmp(((Student *)first)->name, ((Student *)second)->name);
}

int group_comp(const void * first, const void * second) {
    return strcmp(((Student *)first)->group, ((Student *)second)->group);
}

void print_menu() {
    printf("You may use this commands:\n\n");
    printf("find <flag> <value>\n");
    printf("This command finds a student with <value> in <flag> and prints it in file\nYou have this <flag>s:\n");
    printf("id - to find a student with <value> id\nname - to find a students with <value> name\nsurname - to find a students with <value> surname\ngroup - to find a students from <value> group\n\n");
    printf("If no such student have been found, command will be ignored\n");
    printf("sort <flag>\n");
    printf("This commands sorts database of students by <flag>\n You have this <flag>s\n");
    printf("id - to sort by id\nname - to sort by name\nsurname - to sort by surname\ngroup - to sort by group\n\n");
    printf("get_stat <id>\n");
    printf("Prints name, surname, group and average of exam marks of student which has <id>\n\n");
    printf("find_best\n");
    printf("Prints students that have average exam marks below that overall average exam marks\n\n");
    printf("print\n");
    printf("Prints current database condition\n\n");
    printf("exit\n");
    printf("Exits the programm\n\n");
    printf("Wrong commands will be ignored\n");
}