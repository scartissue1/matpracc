#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER,
    NO_FILE
} status_codes;

typedef struct {
    int x;
    int y;
    char transport_id[BUFSIZ];
    char arrive[BUFSIZ];
    char departure[BUFSIZ];
    char stop_status[BUFSIZ];
} Stop;

typedef struct stopList {
    Stop value;
    struct stopList * next;
} stopList;

typedef struct pathList {
    stopList * value;
    struct pathList * next; 
} pathList;

long long int time_compare(const char * time1, const char * time2) {
    int day1 = (time1[0] - '0') * 10 + (time1[1] - '0');
    int month1 = (time1[3] - '0') * 10 + (time1[4] - '0');
    int year1 = (time1[6] - '0') * 1000 + (time1[7] - '0') * 100 + (time1[8] - '0') * 10 + (time1[9] - '0');
    int hours1 = (time1[11] - '0') * 10 + (time1[12] - '0');
    int minutes1 = (time1[14] - '0') * 10 + (time1[15] - '0');
    int seconds1 = (time1[17] - '0') * 10 + (time1[18] - '0');

    int day2 = (time2[0] - '0') * 10 + (time2[1] - '0');
    int month2 = (time2[3] - '0') * 10 + (time2[4] - '0');
    int year2 = (time2[6] - '0') * 1000 + (time2[7] - '0') * 100 + (time2[8] - '0') * 10 + (time2[9] - '0');
    int hours2 = (time2[11] - '0') * 10 + (time2[12] - '0');
    int minutes2 = (time2[14] - '0') * 10 + (time2[15] - '0');
    int seconds2 = (time1[17] - '0') * 10 + (time2[18] - '0');
    
    long long int diff = (year1 - year2) * 3.156e7 + (month1 - month2) * 2.628e6 + (day1 - day2) * 86400 +
    (hours1 - hours2) * 3600 + (minutes1 - minutes2) * 60 + (seconds1 - seconds2); 
    return diff;
}

void free_stopList(stopList * root) {
    if (!root) {
        return;
    }
    free_stopList(root->next);
    free(root);
}

void free_pathList(pathList * root) {
    if (!root) {
        return;
    }
    free_pathList(root->next);
    free_stopList(root->value);
    free(root);
}

void print_stopList(stopList * root) {
    if (!root) {
        return;
    }
    printf("Coords: %d %d\n", root->value.x, root->value.y);
    printf("ID: %s\n", root->value.transport_id);
    printf("Arrive: %s\n", root->value.arrive);
    printf("Departure: %s\n", root->value.departure);
    printf("Stop status: %s\n", root->value.stop_status);
    print_stopList(root->next);
}

void print_pathList(pathList * root) {
    if (!root) {
        return;
    }
    print_stopList(root->value);
    printf("\n________\n");
    print_pathList(root->next);
}

status_codes create_stopList(stopList ** head, Stop _value) {
    stopList * tmp = (stopList *)malloc(sizeof(stopList));
    if (!tmp) return NO_MEMORY;
    tmp->value = _value;
    tmp->next = (*head);
    (*head) = tmp;
    return OK;
}

status_codes insert_stopList(stopList * head, Stop _value) {
    while (head->next && (time_compare(head->next->value.arrive, _value.arrive) < 0)) {
        head = head->next;
    }
    stopList * tmp = (stopList *)malloc(sizeof(stopList));
    if (!tmp) return NO_MEMORY;
    tmp->value = _value;
    if (head->next) tmp->next = head->next;
    else tmp->next = NULL;
    head->next = tmp;
    return OK;
}

status_codes copy_stopList(stopList ** dest, stopList * src, int iter) {
    if (!src) {
        return OK;
    }
    if (!iter) {
        if (create_stopList(dest, src->value) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    else {
        if (insert_stopList((*dest), src->value) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    copy_stopList(dest, src->next, iter + 1);
    return OK;
}

status_codes copy_pathList(pathList ** dest, pathList * src) {
    if (!src) {
        return OK;
    }
    if (copy_stopList(&(*dest)->value, src->value, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    copy_pathList(dest, src->next);
    return OK;
}

status_codes create_pathList(pathList ** head, Stop _stop) {
    pathList * tmp = (pathList *)malloc(sizeof(pathList));
    if (!tmp) return NO_MEMORY;
    switch (create_stopList(&tmp->value, _stop)) {
        case OK: break;
        case NO_MEMORY: return NO_MEMORY;
    }
    tmp->next = (*head);
    (*head) = tmp;
    return OK;
}


status_codes insert_pathList(pathList * head, Stop _stop) {
    while (1) {
        if (strcmp(head->value->value.transport_id, _stop.transport_id) == 0) {
            if (time_compare(head->value->value.arrive, _stop.arrive) > 0) {
                return create_stopList(&(head->value), _stop);
            }
            return insert_stopList(head->value, _stop);
        }
        if (!head->next) {
            break;
        }
        head = head->next;
    }
    if (strcmp(head->value->value.transport_id, _stop.transport_id) == 0) {
        return insert_stopList(head->value, _stop);
    }
    pathList * tmp = (pathList *)malloc(sizeof(pathList));
    if (!tmp) {
        return NO_MEMORY;
    }
    if (create_stopList(&(tmp->value), _stop) == NO_MEMORY) {
        return NO_MEMORY;
    }
    tmp->next = NULL;
    head->next = tmp;
    return OK;
    
}

status_codes get_info(pathList ** head, int array_size, char * array[]) {
    for (int i = 1; i < array_size; i++) {
        FILE * file = fopen(array[i], "r");
        if (!file) return NO_FILE;
        Stop _stop;
        fscanf(file, "%d%d", &_stop.x, &_stop.y);
        char new_id[BUFSIZ], new_arrive_date[BUFSIZ], new_arrive_time[BUFSIZ], new_departure_date[BUFSIZ], new_departure_time[BUFSIZ], stop_status[BUFSIZ];
        while (fscanf(file, "%s%s%s%s%s%s", new_id, new_arrive_date, new_arrive_time, new_departure_date, new_departure_time, stop_status) != EOF) {
            strcpy(_stop.transport_id, new_id);
            char * new_arrive = strcat(new_arrive_date, " ");
            new_arrive = strcat(new_arrive, new_arrive_time);
            char * new_departure = strcat(new_departure_date, " ");
            new_departure = strcat(new_departure, new_departure_time);
            strcpy(_stop.arrive, new_arrive);
            strcpy(_stop.departure, new_departure);
            strcpy(_stop.stop_status, stop_status);
            if (!(*head)) {
                switch (create_pathList(head, _stop)) {
                    case OK: break;
                    case NO_MEMORY: 
                        fclose(file);
                        free_pathList(*head);
                        return NO_MEMORY;
                }
            }
            else {
                switch (insert_pathList((*head), _stop)) {
                    case OK:
                        break;
                    case NO_MEMORY:
                        fclose(file);
                        free_pathList(*head);
                        return NO_MEMORY;
                }

            }
        }
        fclose(file);
    }
    return OK;
}

status_codes find_longest_way(pathList * path, char * max_result, char * min_result, double max, double min) {
    if (!path) {
        return OK;
    }
    double summ = 0;
    double eps = 1e-10;
    stopList * copy = NULL;
    if (copy_stopList(&copy, path->value, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    while (copy->next) {
        double current = sqrt((copy->value.x - copy->next->value.x) * (copy->value.x - copy->next->value.x) + 
        (copy->value.y - copy->next->value.y) * (copy->value.y - copy->next->value.y));
        summ += current;
        stopList * tmp = copy->next;
        free(copy);
        copy = tmp;
    }
    if (summ - max > eps) {
        max = summ;
        strcpy(max_result, copy->value.transport_id);
    }
    if (summ - min < eps || min + 1 < eps) {
        min = summ;
        strcpy(min_result, copy->value.transport_id);
    }
    free_stopList(copy);
    find_longest_way(path->next, max_result, min_result, max, min);
    return OK;
}

status_codes find_most_routes(pathList * path, char * most_result, char * min_result, int max, int min) {
    if (!path)
        return OK;
    int current = 0;
    stopList * copy = NULL;
    if (copy_stopList(&copy, path->value, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    while (copy->next) {
        if (strcmp(copy->value.stop_status, "start") == 0) {
            current++;
        }
        stopList * tmp = copy->next;
        free(copy);
        copy = tmp;
    }
    if (strcmp(copy->value.stop_status, "start") == 0) {
        current++;
    }
    if (current >= max) {
        max = current;
        strcpy(most_result, copy->value.transport_id);
    }
    if (current < min || min == -1) {
        min = current;
        strcpy(min_result, copy->value.transport_id);
    }
    free_stopList(copy);
    find_most_routes(path->next, most_result, min_result, max, min);
    return OK;
}

status_codes find_most_route_length(pathList * path, char * max_result, char * min_result, double max, double min) {
    if (!path) {
        return OK;
    }
    double summ = 0;
    double eps = 1e-10;
    double current = 0;
    stopList * copy = NULL;
    if (copy_stopList(&copy, path->value, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    while (copy->next) {
        if (strcmp(copy->value.stop_status, "end") == 0) {
            current = 0;
            if (summ - max > eps) {
                max = summ;
                strcpy(max_result, copy->value.transport_id);
            }
            if (current < min || min == -1) {
                min = summ;
                strcpy(min_result, copy->value.transport_id);
            }
            summ = 0;
        }
        else {
            current = sqrt((copy->value.x - copy->next->value.x) * (copy->value.x - copy->next->value.x) + 
                    (copy->value.y - copy->next->value.y) * (copy->value.y - copy->next->value.y));
            summ += current;
        }
        stopList * tmp = copy->next;
        free(copy);
        copy = tmp;
    }
    free_stopList(copy);
    find_most_route_length(path->next, max_result, min_result, max, min);
    return OK;
}

status_codes find_most_sleeptime(pathList * path, char * max_result, char * min_result, int max, int min) {
    if (!path) {
        return OK;
    }
    stopList * copy = NULL;
    if (copy_stopList(&copy, path->value, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    while (copy->next) {
        int diff = time_compare(copy->value.departure, copy->value.arrive);
        if (diff > max) {
            max = diff;
            strcpy(max_result, copy->value.transport_id);
        }
        if (diff < min || min == -1) {
            min = diff;
            strcpy(min_result, copy->value.transport_id);
        }
        stopList * tmp = copy->next;
        free(copy);
        copy = tmp;
    }
    int diff = time_compare(copy->value.departure, copy->value.arrive);
    if (diff > max) {
        max = diff;
        strcpy(max_result, copy->value.transport_id);
    }
    if (diff < min || min == -1) {
        min = diff;
        strcpy(min_result, copy->value.transport_id);
    }
    free_stopList(copy);
    find_most_sleeptime(path->next, max_result, min_result, max, min);
    return OK;
}

status_codes find_longest_ovr_sleeptime(pathList * path, char * max_result, char * min_result, int min, int max) {
    if (!path) {
        return OK;
    }
    stopList * copy = NULL;
    if (copy_stopList(&copy, path->value, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    int summ = 0;
    while (copy->next) {
        int diff = time_compare(copy->value.departure, copy->value.arrive);
        summ += diff;
        stopList * tmp = copy->next;
        free(copy);
        copy = tmp;
    }
    int diff = time_compare(copy->value.departure, copy->value.arrive);
    summ += diff;
    if (summ > max) {
        max = summ;
        strcpy(max_result, copy->value.transport_id);
    }
    if (summ < min || min == -1) {
        min = summ;
        strcpy(min_result, copy->value.transport_id);
    }
    free_stopList(copy);
    find_longest_ovr_sleeptime(path->next, max_result, min_result, min, max);
    return OK;
}

void print_menu();

status_codes communicate(pathList * path) {
    print_menu();
    while (1) {
        char command[BUFSIZ];
        scanf("%s", command);
        if (strcmp(command, "info") == 0) {
            print_pathList(path);
        }
        else if (strcmp(command, "routes") == 0) {
            char max[128];
            char min[128];
            if (find_most_routes(path, max, min, -1, -1) == NO_MEMORY) {
                return NO_MEMORY;
            }
            printf("Most routes: %s\nMin routes %s\n", max, min);
        }
        else if (strcmp(command, "ovr_length") == 0) {
            char max[128];
            char min[128];
            if (find_longest_way(path, max, min, -1.0, -1.0) == NO_MEMORY) {
                return NO_MEMORY;
            }
            printf("Longest way: %s\nShortest way: %s\n", max, min);
        }
        else if (strcmp(command, "route_length") == 0) {
            char max[128];
            char min[128];
            if (find_most_route_length(path, max, min, -1.0, -1.0) == NO_MEMORY) {
                return NO_MEMORY;
            }
            printf("Longest way in one route: %s\nShortest way in one route: %s\n", max, min);
        }
        else if (strcmp(command, "sleeptime") == 0) {
            char max[128];
            char min[128];
            if (find_most_sleeptime(path, max, min, -1, -1) == NO_MEMORY) {
                return NO_MEMORY;
            }
            printf("Most sleeptime in one stop: %s\nLeast sleeptime in one stop: %s\n", max, min);
        }
        else if (strcmp(command, "ovr_sleeptime") == 0) {
            char max[128];
            char min[128];
            if (find_longest_ovr_sleeptime(path, max, min, -1, -1) == NO_MEMORY) {
                return NO_MEMORY;
            }
            printf("Most overall sleeptime: %s\nLeast overall sleeprime: %s\n", max, min);
        }
        else if (strcmp(command, "print_menu") == 0) {
            print_menu();
        }
        else if (strcmp(command, "exit") == 0) {
            return OK;
        }
        else {
            printf("No such command\n");
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        printf("Wrong flag\n");
        return -1;
    }
    pathList * paths = NULL;
    switch (get_info(&paths, argc, argv)) {
        case OK: 
            break;
        case NO_MEMORY:
            free_pathList(paths);
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            printf("Invalid parameter\n");
            return -1;
        case NO_FILE:
            printf("No such file\n");
            return -1;

    }
    if (communicate(paths) == NO_MEMORY) {
        free_pathList(paths);
        printf("No memory\n");
        return -1;
    }
    free_pathList(paths);
    return 0;
}

void print_menu() {
    printf("You have this commands:\n");
    printf("info - to print info about all the stops of transportIDs\n");
    printf("routes - find transport_IDs that made the most/least routes\n");
    printf("ovr_length - find most/least length that transportID made for the all time\n");
    printf("route_length - find most/least length that transportID made in one route\n");
    printf("sleeptime - find most/least time that transportID slept on one stop\n");
    printf("ovr_sleeptime - find most/least time that transportID slept on all stops\n");
    printf("print_menu - print this menu again\n");
    printf("exit - to leave the programm\n");
}
