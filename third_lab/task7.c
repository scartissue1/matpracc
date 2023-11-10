#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int time_compare(const char * time1, const char * time2);

int name_validation(const char * name);

int double_validation(const char * number);

int time_validation(const char * time);

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

typedef enum {
    added,
    changed,
    deleted
} actions;

typedef struct { 
    unsigned int id;
    char name[BUFSIZ];
    char surname[BUFSIZ];
    char second_name[BUFSIZ];
    char birth[BUFSIZ];
    char gender;
    double salary;
} Liver; // печень)

typedef struct Stack {
    Liver head;
    actions action;
    struct Stack * tail;
} Stack;

status_codes insertStack(Stack ** root, Liver value, actions _action) {
    Stack * tmp = (Stack *)malloc(sizeof(Stack));
    if (!tmp)
        return NO_MEMORY;
    tmp->head = value;
    tmp->action = _action;
    tmp->tail = (*root);
    (*root) = tmp;
    return OK;
}

status_codes popStack(Stack ** root, Liver * resultLiver, actions * resultActions) {
    if (!(*root))
        return INVALID_PARAMETER;
    Stack * tmp = (*root);
    (*root) = tmp->tail;
    (*resultLiver) = tmp->head;
    (*resultActions) = tmp->action;
    free(tmp);
    return OK;
}

void freeStack(Stack * root) {
    if (!root)
        return;
    free(root->tail);
    free(root);
}

typedef struct List {
    Liver data;
    struct List * next;
} List;

status_codes initList(List ** head, const Liver value) {
    List * tmp = (List *)malloc(sizeof(List));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->data = value;
    tmp->next = (*head);
    (*head) = tmp;
    return OK;
}

status_codes insertList(List * head, const Liver value) {
    while (head->next && time_compare(head->next->data.birth, value.birth) < 0) {
        head = head->next;
    }
    List * tmp = (List *)malloc(sizeof(List));
    if (!tmp) {
        return INVALID_PARAMETER;
    }
    tmp->data = value;
    if (head->next) 
        tmp->next = head->next;
    else
        tmp->next = NULL;
    head->next = tmp;
    return OK;
}

void freeList(List * head) {
    if (!head)
        return;
    freeList(head->next);
    free(head);
}

void printLiver(FILE * stream, Liver value) {
    fprintf(stream, "Name: %s\n", value.name);
    fprintf(stream, "Surname: %s\n", value.surname);
    (strcmp(value.second_name, "0") == 0) ? fprintf(stream, "Second name: \n") : fprintf(stream, "Second name: %s\n", value.second_name);
    fprintf(stream, "Brithday: %s\n", value.birth);
    fprintf(stream, "Gender: %c\n", value.gender);
    fprintf(stream, "Average month salary: %f\n", value.salary);
}

void printList(FILE * stream, List * root) {
    if (!root)
        return;
    printLiver(stream, root->data);
    printList(stream, root->next);
}

status_codes find_stringList(List * head, const char * value, int value_type) {
    status_codes result = INVALID_PARAMETER;
    while (head) {
        if ((value_type == 0) ? strcmp(head->data.name, value) == 0 : 
        (value_type == 1) ? strcmp(head->data.surname, value) == 0 :
        (value_type == 2) ? strcmp(head->data.second_name, value) == 0 :
        (value_type == 3) ? strcmp(head->data.birth, value) == 0 : 
        head->data.gender == value[0]) {
            printLiver(stdout, head->data);
            result = OK;
        }
        head = head->next;
    }
    return result;
}

status_codes find_doubleList(List * head, const double value) {
    double eps = 1e-10;
    status_codes result = INVALID_PARAMETER;
    while (head) {
        if (fabs(value - head->data.salary) < eps) {
            printLiver(stdout, head->data);
            result = OK;
        }
        head = head->next;
    }
    return result;
}

int cmpLiver(const Liver _1, const Liver _2) {
    double eps = 1e-10;
    if (strcmp(_1.name, _2.name)) return strcmp(_1.name, _2.name);
    if (strcmp(_1.surname, _2.surname)) return strcmp(_1.surname, _2.surname);
    if (strcmp(_1.second_name, _2.second_name)) return strcmp(_1.second_name, _2.second_name);
    if (time_compare(_1.birth, _2.birth)) return time_compare(_1.birth, _2.birth);
    if (_1.gender != _2.gender) return _1.gender - _2.gender;
    if (fabs(_1.salary - _2.salary) > eps) {
        if (_1.salary - _2.salary > eps)
            return 1;
        else
            return -1;
    }
    return 0;
}

status_codes delete_firstLiverList(List ** head, Stack ** root, actions action, unsigned int * result_id, int to_stack_flag) {
    if (to_stack_flag)
        if (insertStack(root, (*head)->data, action) == NO_MEMORY)
            return NO_MEMORY;
    (*result_id) = (*head)->data.id;
    List * tmp = (*head)->next;
    free(*head);
    (*head) = tmp;
    return OK;
}

status_codes delete_LiverList(List * head, Stack ** root, const Liver value, actions action, unsigned int * result_id, int to_stack_flag, int * action_count) {
    status_codes result = INVALID_PARAMETER;
    int deleted = 0;
    while (head->next) {
        if (cmpLiver(value, head->next->data) == 0) {
            List * tmp = head->next;
            (*result_id) = tmp->data.id;
            if (to_stack_flag)
                if (insertStack(root, tmp->data, action) == NO_MEMORY)
                    return NO_MEMORY;
            result = OK;
            head->next = tmp->next;
            free(tmp);
            (*action_count)++;
            deleted = 1;
        }
        if (!deleted) {
            if (head->next) 
                head = head->next;
            else
                return result;
        }
        deleted = 0;
    }
    return result;
}

status_codes change_LiverList(List ** head, Stack ** root, const Liver to_change, Liver changing, int * actions_count, int to_stack_flag) {
    status_codes result = INVALID_PARAMETER;
    List * start = (*head);
    int deleted = 0;
    while (start->next) {
        if (cmpLiver(to_change, start->next->data) == 0) {
            List * tmp = start->next;
            if (to_stack_flag)
                if (insertStack(root, tmp->data, changed) == NO_MEMORY)
                    return NO_MEMORY;
            result = OK;
            start->next = tmp->next;
            changing.id = tmp->data.id;
            free(tmp);
            if (!(*head) || time_compare((*head)->data.birth, changing.birth) > 0) {
                if (initList(head, changing) == NO_MEMORY)
                    return NO_MEMORY;
            }
            else {
                if (insertList((*head), changing) == NO_MEMORY)
                    return NO_MEMORY;
            }
            deleted = 1;
            (*actions_count)++;
        }
        if (!deleted) {
            if (start->next)
                start = start->next;
            else
                return result;
        }
        deleted = 0;
    }
    return result;
}

status_codes get_info(FILE * file, List ** root, int * index) {
    Liver citizen;
    (*index) = 0;
    char new_name[BUFSIZ];
    char new_surname[BUFSIZ];
    char new_second_name[BUFSIZ];
    char new_birth[BUFSIZ];
    char new_gender;
    char new_salary[BUFSIZ];
    while (1) {
        int second_name_flag = 1;
        if (fscanf(file, "%s", new_name) == EOF)
            break;
        if (!strlen(new_name) || !name_validation(new_name))
            return INVALID_PARAMETER;
        strcpy(citizen.name, new_name);
        fseek(file, 1, SEEK_CUR);
        if (fscanf(file, "%s", new_surname) == EOF) 
            break;
        if (!strlen(new_surname) || !name_validation(new_surname)) 
            return INVALID_PARAMETER;
        strcpy(citizen.surname, new_surname);
        fseek(file, 1, SEEK_CUR);
        if (fscanf(file, "%s", new_second_name) == EOF)
            break;
        if (!strlen(new_second_name)) 
            return INVALID_PARAMETER;
        if (isdigit(new_second_name[0])) {
            if (!time_validation(new_second_name))
                return INVALID_PARAMETER;
            strcpy(citizen.birth, new_second_name);
            strcpy(citizen.second_name, "0");
            second_name_flag = 0;
        }
        else {
            if (!name_validation(new_second_name))
                return INVALID_PARAMETER;
            strcpy(citizen.second_name, new_second_name);
        }
        if (second_name_flag) {
            fseek(file, 1, SEEK_CUR);
            if (fscanf(file, "%s", new_birth) == EOF)
                break;
            if (!strlen(new_birth) || !time_validation(new_birth))
                return INVALID_PARAMETER;
            strcpy(citizen.birth, new_birth);
        }
        fseek(file, 1, SEEK_CUR);
        if (fscanf(file, "%c", &new_gender) == EOF)
            break;
        if (new_gender != 'M' && new_gender != 'W')
            return INVALID_PARAMETER;
        citizen.gender = new_gender;
        if (fscanf(file, "%s", new_salary) == EOF)
            break;
        if (!strlen(new_salary) || !double_validation(new_salary))
            return INVALID_PARAMETER;
        citizen.salary = atof(new_salary);
        citizen.id = (*index);
        (*index)++;
        if (!(*root) || time_compare((*root)->data.birth, citizen.birth) > 0) {
            if (initList(root, citizen) == NO_MEMORY)
                return NO_MEMORY;
        }
        else {
            if (insertList((*root), citizen) == NO_MEMORY)
                return NO_MEMORY;
        }
        fseek(file, 1, SEEK_CUR);
    }
    return OK;
}

status_codes getLiver(Liver * value) {
    char name[BUFSIZ], surname[BUFSIZ], second_name[BUFSIZ], bithday[BUFSIZ], gender[BUFSIZ], salary[BUFSIZ];
    printf("Name: ");
    scanf("%s", name);
    if (!name_validation(name)) {
        printf("Name should contain only english letters\n");
        return INVALID_PARAMETER;
    }
    printf("Surname: ");
    scanf("%s", surname);
    if (!name_validation(surname)) {
        printf("Surname should contain only english letters\n");
        return INVALID_PARAMETER;
    }
    printf("Second name (leave 0 if there is no second_name): ");
    scanf("%s", second_name);
    if (strcmp(second_name, "0") && !name_validation(second_name)) {
        printf("Second name should contain only english letters\n");
        return INVALID_PARAMETER;
    }
    printf("Birthday (format is dd:MM:yy): ");
    scanf("%s", bithday);
    if (!time_validation(bithday)) {
        printf("Date is not in format\n");
        return INVALID_PARAMETER;
    }
    printf("Gender (only M or W): ");
    scanf("%s", gender);
    if (strcmp(gender, "M") && strcmp(gender, "W")) {
        printf("Gender should be M or W\n");
        return INVALID_PARAMETER;
    }
    printf("Salary (real+ number): ");
    scanf("%s", salary);
    if (!double_validation(salary)) {
        printf("Not real+ number\n");
        return INVALID_PARAMETER;
    }
    strcpy(value->name, name);
    strcpy(value->surname, surname);
    strcpy(value->second_name, second_name);
    strcpy(value->birth, bithday);
    value->gender = gender[0];
    value->salary = atof(salary);
    printf("\n");
    return OK;
}

void print_menu();

void find_to_changeList(List * head, const Liver value, Liver * changing) {
    while (head) {
        if (value.id == head->data.id) {
            (*changing) = head->data;
            break;
        }
        head = head->next;
    }
    
}

status_codes undo(List ** head, Stack ** root, int actions_count) {
    for (int i = 0; i < actions_count / 2; i++) {
        Liver value;
        actions action;
        popStack(root, &value, &action);
        switch (action) {
            case changed:
                Liver to_change;
                find_to_changeList((*head), value, &to_change);
                unsigned int no_use = 0;
                while (cmpLiver(to_change, (*head)->data) == 0) {
                    if (delete_firstLiverList(head, root, changed, &no_use, 0) == NO_MEMORY)
                        return NO_MEMORY;
                    actions_count++;
                
                    if (!(*head) || time_compare((*head)->data.birth, value.birth) > 0)
                        if (initList(head, value) == NO_MEMORY)
                            return NO_MEMORY;
                    else
                        if (insertList((*head), value) == NO_MEMORY)
                            return NO_MEMORY;
                }
            
                status_codes status = change_LiverList(head, root, value, to_change, &actions_count, 0);
                if (status == NO_MEMORY)
                    return NO_MEMORY;
                break;
            case deleted:
                if (!(*head) || time_compare((*head)->data.birth, value.birth) > 0) {
                    if (initList(head, value) == NO_MEMORY)
                        return NO_MEMORY;
                }
                else {
                    if (insertList((*head), value) == NO_MEMORY)
                        return NO_MEMORY;
                }
                break;
            case added:
                if (value.id == (*head)->data.id)
                    delete_firstLiverList(head, root, action, &no_use, 0);
                else
                    delete_LiverList((*head), root, value, action, &no_use, 0, &no_use);
        }
    }
}

status_codes communicate(List ** root, int index) {
    print_menu();
    int actions_count = 0;
    Stack * actionsStack = NULL;
    while (1) {
        char command[BUFSIZ];
        scanf("%s", command);
        if (strcmp(command, "find") == 0) {
            char flag[BUFSIZ];
            scanf("%s", flag);
            if (strcmp(flag, "name") == 0) {
                char name[BUFSIZ];
                scanf("%s", name);
                if (!name_validation(name)) {
                    printf("Name should contain only english letters\n");
                    continue;
                }
                if (find_stringList((*root), name, 0) == INVALID_PARAMETER)
                    printf("No such name has been found\n");
            }
            else if (strcmp(flag, "surname") == 0) {
                char surname[BUFSIZ];
                scanf("%s", surname);
                if (!name_validation(surname)) {
                    printf("Surname should contain only english letters\n");
                    continue;
                }
                if (find_stringList((*root), surname, 1) == INVALID_PARAMETER)
                    printf("No such surname has been found\n");
            }
            else if (strcmp(flag, "second_name") == 0) {
                char second_name[BUFSIZ];
                scanf("%s", second_name);
                if (strcmp(second_name, "0") && !name_validation(second_name)) {
                    printf("Second name should contain only english letters\n");
                    continue;
                }
                if (find_stringList((*root), second_name, 2) == INVALID_PARAMETER)
                    printf("No such second name has been found\n");
            }
            else if (strcmp(flag, "birthday") == 0) {
                char birthday[BUFSIZ];
                scanf("%s", birthday);
                if (!time_validation(birthday)) {
                    printf("Birthday should be in dd:MM:yyyy format\n");
                    continue;
                }
                if (find_stringList((*root), birthday, 3) == INVALID_PARAMETER)
                    printf("No such birthday has been found\n");
            }
            else if (strcmp(flag, "gender") == 0) {
                char gender[BUFSIZ];
                scanf("%s", gender);
                if (strcmp(gender, "M") && strcmp(gender, "W")) {
                    printf("Gender can be only M and W\n");
                    continue;
                }
                if (find_stringList((*root), gender, 4) == INVALID_PARAMETER)
                    printf("No such gender has been found\n");
            }
            else if (strcmp(flag, "salary") == 0) {
                char salary[BUFSIZ];
                scanf("%s", salary);
                if (!double_validation(salary)) {
                    printf("Salary should be real+ number\n");
                    continue;
                }
                if (find_doubleList((*root), atof(salary)) == INVALID_PARAMETER)
                    printf("No such salary has been found\n");
            }
            else
                printf("Wrong flag\n");
        }
        else if (strcmp(command, "change") == 0) {
            if (!(*root)) {
                printf("Citizens database is empty\n");
                continue;
            }
            printf("Type the information about the citizen that you want to change\n");
            Liver to_change;
            if (getLiver(&to_change) == INVALID_PARAMETER)
                continue;
            printf("Type the information about changing citizen\n");
            Liver changing;
            if (getLiver(&changing) == INVALID_PARAMETER)
                continue;
            unsigned int id = 0;
            int in_first = 0;
            while (cmpLiver(to_change, (*root)->data) == 0) {
                in_first = 1;
                if (delete_firstLiverList(root, &actionsStack, changed, &id, 1) == NO_MEMORY)
                    return NO_MEMORY;
                actions_count++;
                changing.id = id;
                if (!(*root) || time_compare((*root)->data.birth, changing.birth) > 0) {
                    if (initList(root, changing) == NO_MEMORY)
                        return NO_MEMORY;
                }
                else {
                    if (insertList((*root), changing) == NO_MEMORY)
                        return NO_MEMORY;
                }
            }
            
            status_codes status = change_LiverList(root, &actionsStack, to_change, changing, &actions_count, 1);
            if (status == NO_MEMORY)
                return NO_MEMORY;
            else if (status == INVALID_PARAMETER && !in_first) {
                printf("No such citizen has been found\n");
                continue;
            }
            printf("Done\n");
        }
        else if (strcmp(command, "add") == 0) {
            printf("Type the information about the citizen you want to add\n");
            Liver to_add;
            if (getLiver(&to_add) == INVALID_PARAMETER)
                continue;
            to_add.id = index;
            index++;
            if (!(*root) || time_compare((*root)->data.birth, to_add.birth) > 0) {
                if (initList(root, to_add) == NO_MEMORY)
                    return NO_MEMORY;
            }
            else {
                if (insertList((*root), to_add) == NO_MEMORY)
                    return NO_MEMORY;
            }
            if (insertStack(&actionsStack, to_add, added) == NO_MEMORY)
                return NO_MEMORY;
            actions_count++;

        }
        else if (strcmp(command, "delete") == 0) {
            printf("Type the information about the citizen you want to delete\n");
            Liver to_delete;
            unsigned int id = 0;
            if (getLiver(&to_delete) == INVALID_PARAMETER)
                continue;
            int in_root_flag = cmpLiver(to_delete, (*root)->data) == 0;
            if (in_root_flag) {
                if (delete_firstLiverList(root, &actionsStack, deleted, &id, 1) == NO_MEMORY)
                    return NO_MEMORY;
            }
            else {
                status_codes status = delete_LiverList((*root), &actionsStack, to_delete, deleted, &id, 1, &actions_count);
                if (status == NO_MEMORY)
                    return NO_MEMORY;
                else if (status == INVALID_PARAMETER && !in_root_flag) {
                    printf("No such citizen has been found\n");
                    continue;
                }
            }
        }
        else if (strcmp(command, "print") == 0) {
            char path[BUFSIZ];
            scanf("%s", path);
            FILE * out = fopen(path, "w");
            if (!out) {
                printf("No such file\n");
                continue;
            }
            printList(out, (*root));
            fclose(out);

        }
        else if (strcmp(command, "undo") == 0) {
            undo(root, &actionsStack, actions_count);
            actions_count = 0;
        }
        else if (strcmp(command, "exit") == 0)
            break;
    }
    free(actionsStack);
    return OK;
}

int main(int argc, char * argv[]) {
    List * root = NULL;
    FILE * in = fopen(argv[1], "r");
    if (!in) {
        printf("No file\n");
        return -1;
    }
    int index = 0;
    switch (get_info(in, &root, &index)) {
        case OK:
            break;
        case NO_MEMORY:
            freeList(root);
            fclose(in);
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            freeList(root);
            fclose(in);
            printf("Invalid parameter\n");
            return -1;
    }
    communicate(&root, index);
    freeList(root);
    fclose(in);
    return 0;
}

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
    int e_count = 0, dot_count = 0;
    int fl = 0;
    while (index < size) {
        if (number[index] == 'e') e_count++;
        if (number[index] == '.') dot_count++;
        if (e_count > 1 || dot_count > 1 || (!isdigit(number[index]) && (number[index] != 'e' || number[index] != '.'))) return 0;
        if (isdigit(number[index])) fl = 1;
        index++;
    }
    return (1 && fl);
}

int time_validation(const char * time) { // dd:MM:yyyy
    if (strlen(time) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(time[i]) && i != 2 && i != 5) return 0;
    }
    int day = (time[0] - '0') * 10 + (time[1] - '0');
    int month = (time[3] - '0') * 10 + (time[4] - '0');
    int year = (time[6] - '0') * 1000 + (time[7] - '0') * 100 + (time[8] - '0') * 10 + (time[9] - '0');
    if (day <= 0) return 0;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
    month == 10 || month == 12) {
        if (day > 31) return 0;
    }
    else if (month == 2) {
        if (year % 4 == 0) {
            if (day > 29) return 0;
        }
        else {
            if (day > 28) return 0;
        }
    }
    else {
        if (day > 30) return 0;
    }
    return 1;
}

int time_compare(const char * time1, const char * time2) {
    int day1 = (time1[0] - '0') * 10 + (time1[1] - '0');
    int month1 = (time1[3] - '0') * 10 + (time1[4] - '0');
    int year1 = (time1[6] - '0') * 1000 + (time1[7] - '0') * 100 + (time1[8] - '0') * 10 + (time1[9] - '0');

    int day2 = (time2[0] - '0') * 10 + (time2[1] - '0');
    int month2 = (time2[3] - '0') * 10 + (time2[4] - '0');
    int year2 = (time2[6] - '0') * 1000 + (time2[7] - '0') * 100 + (time2[8] - '0') * 10 + (time2[9] - '0');

    if (year1 != year2) return year1 - year2;
    if (month1 != month2) return month1 - month2;
    return day1 - day2;
}

void print_menu() {
    printf("You may use this commands\n");
    printf("find <flag> <value> - finds a citizen which has <value> in <flag>\n");
    printf("You have this flags\nname - find by name (contains only english letters)\nsurname - find by surname (contains only english letters)\nsecond_name - find by second_name, if there is no second_name type 0 (contains only english letters)\nbirthday - find by birthday (format of birthday is dd:MM:yyyy)\ngender - find by gender (only M or W)\nsalary - find by salary (real+ number)\n");
    printf("add - to add the citizen to the database\n");
    printf("delete - to delete the citizen from the database\n");
    printf("change - to change the information about the citizen in the database\n");
    printf("print <file_path> - print current database to the file with <file_path> path\n");
    printf("undo - to undo last opeartions\n");
    printf("exit - to exit\n\n");
}
