#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "String.h"

typedef struct {
    String * city;
    String * street;
    int house_number;
    String * building;
    int apartament_number;
    String * index;
} Adress;

typedef struct {
    Adress adress;
    double weight;
    String * mail_id;
    String * creation;
    String * delivery;
} Mail;

typedef struct {
    Adress * adress;
    Mail * mails;
} Post;

int double_validation(const char * number);

int int_validation(const char * number);

int mail_id_validation(const char * mail_id);

int time_validation(const char * time);

int time_compare_validation(const char * time1, const char * time2);

int time_compare_sort(const void * first, const void * second);

void print_mail(Mail _mail);

int mail_compare(const void * first, const void * second);

int time_compare_to_local(const char * time_ptr);

void timesup_finder(Post * _post, size_t size);

void print_mails(Post *_post, size_t size);

void free_post(Post ** _post, size_t size);

void print_menu();

status_codes create_adress(Adress * new, char * new_city, char * new_street, char * new_house_number_string, char * new_building, char * new_apartament_number_string, char * new_index) {
    int new_house_number = atoi(new_house_number_string);
    int new_apartament_number = atoi(new_apartament_number_string);
    if (!strlen(new_city) || !strlen(new_street) || !int_validation(new_house_number_string) || new_house_number <= 0 || !int_validation(new_apartament_number_string) || new_apartament_number <= 0
    || strlen(new_index) != 6 || !int_validation(new_index)) return INVALID_PARAMETER;
    new->apartament_number = new_apartament_number;
    new->house_number = new_house_number;
    String * string_city = NULL;
    String * string_street = NULL;
    String * string_building = NULL;
    String * string_index = NULL;
    if (init_string(&string_city, new_city) == NO_MEMORY || init_string(&string_street, new_street) == NO_MEMORY || init_string(&string_building, new_building) == NO_MEMORY || init_string(&string_index, new_index) == NO_MEMORY) return NO_MEMORY;
    new->city = string_city;
    new->building = string_building;
    new->street = string_street;
    new->index = string_index;
    return OK;
}

status_codes create_mail(Mail * new, Adress new_adress, char * new_weight_string, char * new_mail_id, char * new_creation, char * new_delivery) {
    new->adress = new_adress;
    if (!double_validation(new_weight_string) || !mail_id_validation(new_mail_id) || !time_validation(new_creation) || !time_validation(new_delivery) || time_compare_validation(new_creation, new_delivery) > 0) return INVALID_PARAMETER;
    double new_weight = atof(new_weight_string);
    if (new_weight < 0) return INVALID_PARAMETER;
    new->weight = new_weight;
    String * string_mail_id = NULL; 
    String * string_new_creation = NULL;
    String * string_new_delivery = NULL;
    if (init_string(&string_mail_id, new_mail_id) == NO_MEMORY || init_string(&string_new_creation, new_creation) == NO_MEMORY || init_string(&string_new_delivery, new_delivery) == NO_MEMORY) {
        return INVALID_PARAMETER;
    }
    new->mail_id = string_mail_id;
    new->delivery = string_new_delivery;
    new->creation = string_new_creation;
    return OK;
}

status_codes init_post(Post ** _post, Adress * _adress, size_t * size, size_t * capacity) {
    (*size) = 0;
    (*capacity) = 2;
    (*_post) = (Post *)malloc(sizeof(Post));
    (*_post)->adress = _adress;
    (*_post)->mails = (Mail *)malloc(sizeof(Mail) * (*capacity));
    if (!(*_post)->mails) return NO_MEMORY;
    return OK;
}

status_codes find_mail(Post * _post, String * mail_id, size_t size) {
    for (size_t i = 0; i < size; i++) {
        int cmp_result = 0;
        if (compare_string(&cmp_result, _post->mails[i].mail_id, mail_id) == INVALID_PARAMETER) return INVALID_PARAMETER;
        if (!cmp_result) {
            print_mail(_post->mails[i]);
            return OK;
        }
    }
    return INVALID_PARAMETER;
}

status_codes add_mail(Post ** _post, Mail _mail, size_t * size, size_t * capacity) {
    if (find_mail((*_post), _mail.mail_id, (*size)) == OK) return INVALID_PARAMETER;
    (*_post)->mails[(*size)] = _mail;
    (*size)++;
    if ((*size) >= (*capacity)) {
        (*capacity) *= 2;
        Mail * tmp = (Mail *)realloc((*_post)->mails, sizeof(Mail) * (*capacity));
        if (!tmp) {
            free((*_post)->mails);
            return NO_MEMORY;
        }
        (*_post)->mails = tmp;
    }
    return OK;
}

status_codes delete_mail(Post ** _post, String * mail_id, size_t size) {
    for (size_t i = 0; i < size; i++) {
        int cmp_result = 0;
        if (compare_string(&cmp_result, (*_post)->mails[i].mail_id, mail_id) == INVALID_PARAMETER) return INVALID_PARAMETER;
        if (!cmp_result) {
            free_string(&(*_post)->mails[i].mail_id);
            switch (init_string(&(*_post)->mails[i].mail_id, "0")) {
                case OK:
                    return OK;
                case NO_MEMORY:
                    return NO_MEMORY;
                case INVALID_PARAMETER:
                    return INVALID_PARAMETER;
            }
        }
    }
    return INVALID_PARAMETER;
}

status_codes communicate() {
    Post * _post = NULL;
    size_t post_size = 0, post_capacity = 2;
    printf("Write the address of the post to initialize it:\n");
    char city[BUFSIZ], street[BUFSIZ], house_number[BUFSIZ], building[BUFSIZ], apartament_number[BUFSIZ], index[BUFSIZ];
    printf("City: ");
    scanf("%s", city);
    printf("Street: ");
    scanf("%s", street);
    printf("House: ");
    scanf("%s", house_number);
    printf("Building: ");
    scanf("%s", building);
    printf("Apartament: ");
    scanf("%s", apartament_number);
    printf("Index (contains 6 numbers): ");
    scanf("%s", index);
    printf("Done!\n\n");
    Adress post_adress;
    if (create_adress(&post_adress, city, street, house_number, building, apartament_number, index) == INVALID_PARAMETER) return INVALID_PARAMETER;
    if (init_post(&_post, &post_adress, &post_size, &post_capacity) == NO_MEMORY) return NO_MEMORY;
    print_menu();
    while (1) {
        char command[BUFSIZ];
        printf("Waiting for command...\n");
        scanf("%s", command);
        if (strcmp(command, "add") == 0) {
            printf("Write the information about the mail\n");
            char city[BUFSIZ], street[BUFSIZ], house_number[BUFSIZ], building[BUFSIZ], apartament_number[BUFSIZ], index[BUFSIZ];
            char mail_id[BUFSIZ], creation_date[BUFSIZ], creation_time[BUFSIZ], delivery_date[BUFSIZ], delivery_time[BUFSIZ], weight[BUFSIZ];
            printf("City: ");
            scanf("%s", city);
            printf("Street: ");
            scanf("%s", street);
            printf("House: ");
            scanf("%s", house_number);
            printf("Building: ");
            scanf("%s", building);
            printf("Apartament: ");
            scanf("%s", apartament_number);
            printf("Index (contains 6 numbers): ");
            scanf("%s", index);
            Adress mail_adress;
            if (create_adress(&mail_adress, city, street, house_number, building, apartament_number, index) == INVALID_PARAMETER) {
                printf("Invalid parameter detected\n");
                continue;
            }
            
            printf("Weight (real+ number): ");
            scanf("%s", weight);
            printf("Mail ID (contains 14 numbers): ");
            scanf("%s", mail_id);
            printf("Creation time (dd:MM:yyyy hh:mm:ss): ");
            scanf("%s", creation_date);
            scanf("%s", creation_time);
            printf("Delivery time (dd:MM:yyyy hh:mm:ss): ");
            scanf("%s", delivery_date);
            scanf("%s", delivery_time);
            char * creation = strcat(creation_date, " ");
            creation = strcat(creation, creation_time);
            char * delivery = strcat(delivery_date, " ");
            delivery = strcat(delivery, delivery_time);
            Mail new_mail;
            if (create_mail(&new_mail, mail_adress, weight, mail_id, creation, delivery) == INVALID_PARAMETER) {
                printf("Invalid parameter detected\n");
                continue;
            }
            status_codes tmp = add_mail(&_post, new_mail, &post_size, &post_capacity);
            if (tmp == NO_MEMORY) return NO_MEMORY;
            else if (tmp == INVALID_PARAMETER) {
                printf("This mail id already exists\n");
                continue;
            }

        }
        else if (strcmp(command, "delete") == 0) {
            char mail_id[BUFSIZ];
            scanf("%s", mail_id);
            String * string_mail_id = NULL;
            status_codes tmp = init_string(&string_mail_id, mail_id);
            if (tmp == NO_MEMORY) return NO_MEMORY;
            else if (tmp == INVALID_PARAMETER) {
                printf("Invalid parameter detected\n");
                continue;
            }
            if (delete_mail(&_post, string_mail_id, post_size) == INVALID_PARAMETER) {
                printf("No such mail id has been found\n");
                free_string(&string_mail_id);
                continue;
            }
            free_string(&string_mail_id);

        }
        else if (strcmp(command, "find") == 0) {
            char mail_id[BUFSIZ];
            scanf("%s", mail_id);
            String * string_mail_id = NULL;
            status_codes tmp = init_string(&string_mail_id, mail_id);
            if (tmp == NO_MEMORY) return NO_MEMORY;
            else if (tmp == INVALID_PARAMETER) {
                printf("Invalid parameter detected\n");
                continue;
            }
            if (find_mail(_post, string_mail_id, post_size) == INVALID_PARAMETER) {
                printf("No such mail id has been found\n");
                free_string(&string_mail_id);
                continue;
            }
            free_string(&string_mail_id);
        }
        else if (strcmp(command, "times_up") == 0) {
            qsort(_post->mails, post_size, sizeof(_post->mails[0]), time_compare_sort);
            timesup_finder(_post, post_size);
        }
        else if (strcmp(command, "sort") == 0) qsort(_post->mails, post_size, sizeof(_post->mails[0]), mail_compare);
        else if (strcmp(command, "exit") == 0) break;
        else if (strcmp(command, "print_all") == 0) print_mails(_post, post_size);
        else printf("Wrong command\n");
    }
    free_post(&_post, post_size);
    return OK;
}

int main(int argc, char * argv[]) {
    switch (communicate()) {
        case OK: break;
        case NO_MEMORY:
            printf("No_memory\n");
            break;
    }
    return 0;
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
        if (e_count > 1 || _count > 1 || dot_count > 1 || (!isdigit(number[index]) && (number[index] != 'e' && number[index] != '.' && (index != 0 && number[index] != '-')))) return 0;
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

int mail_id_validation(const char * mail_id) {
    if (strlen(mail_id) != 14) return 0;
    for (int i = 0; i < 14; i++) {
        if (!isdigit(mail_id[i])) return 0;
    }
    return 1;
}

int time_validation(const char * time) {
    if (strlen(time) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (!isdigit(time[i]) && i != 2 && i != 5 && i != 10 && i != 13 && i != 16) return 0;
    }
    int day = (time[0] - '0') * 10 + (time[1] - '0');
    int month = (time[3] - '0') * 10 + (time[4] - '0');
    int year = (time[6] - '0') * 1000 + (time[7] - '0') * 100 + (time[8] - '0') * 10 + (time[9] - '0');

    int hours = (time[11] - '0') * 10 + (time[12] - '0');
    int minutes = (time[14] - '0') * 10 + (time[15] - '0');
    int seconds = (time[17] - '0') * 10 + (time[18] - '0');
    if (!(0 <= hours && hours <= 23) || !(0 <= minutes && minutes <= 60) || !(0 <= seconds && seconds <= 60)) return 0;
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

int time_compare_validation(const char * time1, const char * time2) {
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

    if (year1 != year2) return year1 - year2;
    if (month1 != month2) return month1 - month2;
    if (day1 != day2) return day1 - day2;
    if (hours1 != hours2) return hours1 - hours2;
    if (minutes1 != minutes2) return minutes1 - minutes2;
    return seconds1 - seconds2;
}

int time_compare_sort(const void * first, const void * second) {
    const char * time1 = ((Mail *)first)->creation->string;
    const char * time2 = ((Mail *)second)->creation->string;
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

    if (year1 != year2) return year1 - year2;
    if (month1 != month2) return month1 - month2;
    if (day1 != day2) return day1 - day2;
    if (hours1 != hours2) return hours1 - hours2;
    if (minutes1 != minutes2) return minutes1 - minutes2;
    return seconds1 - seconds2;
}

void print_mail(Mail _mail) {
    printf("Adress:\n\tCity: %s\n\tStreet: %s\n\tHouse: %d\n\tBuilding: %s\n\tApartament: %d\n\tIndex: %s\n", _mail.adress.city->string, _mail.adress.street->string,
        _mail.adress.house_number, _mail.adress.building->string, _mail.adress.apartament_number, _mail.adress.index->string);
    printf("Weight: %f\nID: %s\nCreation time: %s\nDelivery time: %s\n", _mail.weight, _mail.mail_id->string, _mail.creation->string, _mail.delivery->string);
}

int mail_compare(const void * first, const void * second) {
    int cmp_result = 0;
    compare_string(&cmp_result, ((Mail *)first)->adress.index, ((Mail *)second)->adress.index);
    if (cmp_result) return -cmp_result;
    compare_string(&cmp_result, ((Mail *)first)->mail_id, ((Mail *)second)->mail_id);
    return -cmp_result;
}

int time_compare_to_local(const char * time_ptr) {
    time_t now;
    time(&now);
    struct tm * local = localtime(&now);
    int current_hours = local->tm_hour;
    int current_minutes = local->tm_min;
    int current_seconds = local->tm_sec;
    int current_day = local->tm_mday;
    int current_month = local->tm_mon + 1;
    int current_year = local->tm_year + 1900;

    int day = (time_ptr[0] - '0') * 10 + (time_ptr[1] - '0');
    int month = (time_ptr[3] - '0') * 10 + (time_ptr[4] - '0');
    int year = (time_ptr[6] - '0') * 1000 + (time_ptr[7] - '0') * 100 + (time_ptr[8] - '0') * 10 + (time_ptr[9] - '0');

    int hours = (time_ptr[11] - '0') * 10 + (time_ptr[12] - '0');
    int minutes = (time_ptr[14] - '0') * 10 + (time_ptr[15] - '0');
    int seconds = (time_ptr[17] - '0') * 10 + (time_ptr[18] - '0');

    if (current_year != year) return current_year - year;
    if (current_month != month) return current_month - month;
    if (current_day != day) return current_day - day;
    if (current_hours != hours) return current_hours - hours;
    if (current_minutes != minutes) return current_minutes - minutes;
    return current_seconds - seconds;
}

void timesup_finder(Post * _post, size_t size) {
    for (int i = 0; i < size; i++) {
        if (time_compare_to_local(_post->mails[i].delivery->string) > 0) {
            print_mail(_post->mails[i]);
        }
    }
}

void print_mails(Post *_post, size_t size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(_post->mails[i].mail_id->string, "0")) print_mail(_post->mails[i]);
    }
}

void free_post(Post ** _post, size_t size) {
    for (int i = 0; i < size; i++) {
        free_string(&(*_post)->mails[i].adress.city);
        free_string(&(*_post)->mails[i].adress.street);
        free_string(&(*_post)->mails[i].adress.building);
        free_string(&(*_post)->mails[i].adress.index);
        free_string(&(*_post)->mails[i].mail_id);
        free_string(&(*_post)->mails[i].creation);
        free_string(&(*_post)->mails[i].delivery);
    }
    free((*_post)->mails);
    free(*_post);
}

void print_menu() {
    printf("You can use this commands:\n");
    printf("add - to add the mail to the post office\n");
    printf("delete <value> - to delete the mail with <value> id from the post office\n");
    printf("sort - to sort mails in post office\n");
    printf("find - to find a mail in the post office and get information\n");
    printf("times_up - to find mails, thats delivery time is up\n");
    printf("print_all - print all mails\n");
    printf("exit - to exit the programm");
}