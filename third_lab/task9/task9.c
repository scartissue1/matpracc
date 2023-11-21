#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER
} status_codes;

typedef struct Node {
    char * data;
    int count;
    size_t size;
    struct Node * left;
    struct Node * right;
} Node;

typedef struct Stack {
    int count;
    char * word;
    struct Stack * next;
} Stack;

status_codes insertStack(Stack ** root, const int count, const char * word) {
    Stack * tmp = (Stack *)malloc(sizeof(Stack));
    if (!tmp)
        return NO_MEMORY;
    size_t word_size = strlen(word);
    tmp->word = (char *)malloc(sizeof(char) * word_size);
    memcpy(tmp->word, word, word_size);
    tmp->word[word_size] = '\0';
    tmp->count = count;
    tmp->next = (*root);
    (*root) = tmp;
    return OK;
}

status_codes popStack(Stack ** root, int * count, char ** word) {
    if (!(*root))
        return INVALID_PARAMETER;
    Stack * tmp = (*root);
    (*root) = tmp->next;
    (*count) = tmp->count;
    size_t word_size = strlen(tmp->word);
    (*word) = (char *)malloc(sizeof(char) * word_size);
    memcpy((*word), tmp->word, word_size);
    (*word)[word_size] = '\0';
    free(tmp->word);
    free(tmp);
    return OK;
}

status_codes addNode(Node ** ptr, const char * word) {
    if (!(*ptr)) {
        (*ptr) = (Node *)malloc(sizeof(Node));
        if (!(*ptr))
            return NO_MEMORY;
        size_t word_size = strlen(word);
        (*ptr)->size = word_size;
        (*ptr)->data = (char *)malloc(sizeof(char) * word_size);
        if (!(*ptr)->data)
            return NO_MEMORY;
        memcpy((*ptr)->data, word, word_size);
        (*ptr)->data[word_size] = '\0';
        (*ptr)->count = 1;
        (*ptr)->left = NULL;
        (*ptr)->right = NULL;
        return OK;
    }
    int cmp = strcmp((*ptr)->data, word);
    if (cmp == 0)
        (*ptr)->count++;
    else if (cmp > 0)
        return addNode(&(*ptr)->left, word);
    else
        return addNode(&(*ptr)->right, word);
}

void freeNode(Node * ptr) {
    if (!ptr)
        return;
    freeNode(ptr->left);
    freeNode(ptr->right);
    free(ptr->data);
    free(ptr);
}

void printNode(Node * ptr, FILE * stream, int count) {
    if (!ptr)
        return;
    for (int i = 0; i < count; i++) {
        fputc('\t', stream);
    }
    fprintf(stream, "%d %s\n", ptr->count, ptr->data);
    printNode(ptr->left, stream, count + 1);
    printNode(ptr->right, stream, count + 1);
}

status_codes get_from_file(FILE * stream, Node ** ptr) {
    char * string = NULL;
    (*ptr) = NULL;
    while (getline(&string, &(size_t){0}, stream) != -1) {
        int count = 0;
        char word[BUFSIZ];
        sscanf(string, "%d%s", &count, word);
        for (int i = 0; i < count; i++) {
            if (addNode(ptr, word) == NO_MEMORY)
                return NO_MEMORY;
        }
        free(string);
        string = NULL;
    } 
    return OK;
}

int depthTree(Node * ptr) {
    if (!ptr)
        return 0;
    int left = depthTree(ptr->left);
    int right = depthTree(ptr->right);
    return (left > right) ? left + 1 : right + 1;
} 

void find_word_count(Node * ptr, int * result, const char * word, int found_flag) {
    if (!ptr)
        return;
    if (strcmp(ptr->data, word) == 0) {
        (*result) = ptr->count;
        found_flag = 1;
        return;
    }
    if (!found_flag)
        find_word_count(ptr->left, result, word, found_flag);
        find_word_count(ptr->right, result, word, found_flag);

}

void print_max_size(Node * ptr) {
    Node * ptr2 = ptr;
    while (ptr->right)
        ptr = ptr->right;
    printf("String with max size is: %s\n", ptr->data);
    while (ptr2->left)
        ptr2 = ptr2->left;
    printf("String with min size is: %s\n", ptr2->data);
}

status_codes addsortNode(Node ** ptr, const char * word, const int count, int sort_flag) {
    if (!(*ptr)) {
        (*ptr) = (Node *)malloc(sizeof(Node));
        if (!(*ptr))
            return NO_MEMORY;
        size_t word_size = strlen(word);
        (*ptr)->size = word_size;
        (*ptr)->data = (char *)malloc(sizeof(char) * word_size);
        if (!(*ptr)->data)
            return NO_MEMORY;
        memcpy((*ptr)->data, word, word_size);
        (*ptr)->data[word_size] = '\0';
        (*ptr)->count = count;
        (*ptr)->left = NULL;
        (*ptr)->right = NULL;
        return OK;
    }
    if ((sort_flag == 0) ? count > (*ptr)->count : strlen(word) > (*ptr)->size)
        return addsortNode(&((*ptr)->right), word, count, sort_flag);
    else
        return addsortNode(&((*ptr)->left), word, count, sort_flag);
}

void most_frequent_print(Node * ptr, int current_count, int count) {
    if (!ptr || (current_count == count - 1))
        return;
    most_frequent_print(ptr->right, current_count, count);
    printf("%d %s\n", ptr->count, ptr->data);
    most_frequent_print(ptr->left, current_count + 1, count);
}

status_codes sortTree(Node ** sort, Node * ptr, int sort_flag) {
    if (!ptr)
        return OK;
    if (sortTree(sort, ptr->left, sort_flag) == NO_MEMORY)
        return NO_MEMORY;
    if (sortTree(sort, ptr->right, sort_flag) == NO_MEMORY)
        return NO_MEMORY;
    if (addsortNode(sort, ptr->data, ptr->count, sort_flag) == NO_MEMORY)
        return NO_MEMORY;
    return OK;
}

status_codes getTree(FILE * file, const int splitters_count, char ** splitters, Node ** ptr) {
    for (int i = 2; i < splitters_count; i++) {
        if (strlen(splitters[i]) > 1)
            return INVALID_PARAMETER;
    }
    size_t size = 0;
    size_t capacity = 2;
    char symbol = fgetc(file);
    int splitter_flag = 0;
    char * string = (char *)malloc(sizeof(char) * capacity);
    if (!string)
        return NO_MEMORY;
    char current_splitter;
    while (symbol != EOF) {
        for (int i = 2; i < splitters_count; i++) {
            if (symbol == splitters[i][0]) {
                splitter_flag = 1;
                current_splitter = splitters[i][0];
                while (current_splitter == symbol && symbol != EOF)
                    symbol = fgetc(file);
            }
        }
        if (symbol == EOF)
            return OK;
        if (strlen(string) && splitter_flag) {
            fseek(file, -1, SEEK_CUR);
            string[size] = '\0';
            if (addNode(ptr, string) == NO_MEMORY)
                return NO_MEMORY;
            free(string);
            string = NULL;
            size = 0;
            capacity = 2;
            string = (char *)malloc(sizeof(char) * capacity);
            if (!string) {
                fclose(file);
                freeNode(*ptr);
                return NO_MEMORY;
            }
            symbol = fgetc(file);
            splitter_flag = 0;
            continue;
        }
        string[size] = symbol;
        size++;
        if (size >= capacity) {
            capacity *= 2;
            char * tmp = NULL;
            if (!(tmp = (char *)realloc(string, sizeof(char) * capacity))) {
                free(string);
                freeNode((*ptr));
                fclose(file);
                return NO_MEMORY;
            }
            string = tmp;
        }
        symbol = fgetc(file);
    }
    if (strlen(string)) {
        string[size] = '\0';
        if (addNode(ptr, string) == NO_MEMORY)
            return NO_MEMORY;
    }
    free(string);
    return OK;
}

int int_validation(const char * number);

void print_menu() {
    printf("You have this commands\n");
    printf("word_count <value> - find how manu times <value> in text\n");
    printf("most_frequent <value> - <value> most frequent number in text\n");
    printf("min_max_size - print minimum size and maximum size of word in text\n");
    printf("depth - find max depth of BST\n");
    printf("print <file_path> - print tree to <file_path> and get it from there to stdout\n");
    printf("exit - to exit\n");

}

status_codes communicate(Node * root) {
    print_menu();
    while (1) {
        char command[BUFSIZ];
        scanf("%s", command);
        if (strcmp(command, "word_count") == 0) {
            char word[BUFSIZ];
            scanf("%s", word);
            int count = 0;
            find_word_count(root, &count, word, 0);
            printf("Word %s contains %d time(s) in text\n", word, count);
        }
        else if (strcmp(command, "most_frequent") == 0) {
            char number[BUFSIZ];
            scanf("%s", number);
            if (!int_validation(number)) {
                printf("Invalid parameter\n");
                continue;
            }
            Node * sort = NULL;
            if (sortTree(&sort, root, 0) == NO_MEMORY)
                return NO_MEMORY;
            most_frequent_print(sort, 0, atoi(number));
            freeNode(sort);
        }
        else if (strcmp(command, "min_max_size") == 0) {
            Node * sort = NULL;
            if (sortTree(&sort, root, 1) == NO_MEMORY)
                return NO_MEMORY;
            print_max_size(sort);
            freeNode(sort);
        }
        else if (strcmp(command, "depth") == 0) {
            printf("%d\n", depthTree(root));
        }
        else if (strcmp(command, "print") == 0) {
            char path[BUFSIZ];
            scanf("%s\n", path);
            FILE * file = fopen(path, "w");
            if (!file) {
                printf("No such file\n");
                continue;
            }
            printNode(root, file, 0);
            fclose(file);
            Node * new_root = NULL;
            FILE * file1 = fopen(path, "r");
            if (!file1) {
                return INVALID_PARAMETER;
            }
            get_from_file(file1, &new_root);
            printNode(new_root, stdout, 0);
            fclose(file1);
            freeNode(new_root);
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
        else
            printf("Wrong command\n");
    }
    return OK;
}

int main(int argc, char * argv[]) {
    if (argc < 3) {
        printf("Wrong flag\n");
        return -1;
    }
    FILE * in = fopen(argv[1], "r");
    if (!in) {
        printf("No such file\n");
        return -1;
    }
    Node * root = NULL;
    switch (getTree(in, argc, argv, &root)) {
        case OK:
            break;
        case INVALID_PARAMETER:
            printf("Splitter contain only one symbol\n");
            fclose(in);
            return -1;
        case NO_MEMORY:
            printf("No memory\n");
            return -1;
    }
    if (communicate(root) == NO_MEMORY) {
        freeNode(root);
        fclose(in);
        printf("No memory\n");
        return -1;
    }
    //printNode(root);
    freeNode(root);
    fclose(in);
    return 0;
}

int int_validation(const char * number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(number[i])) return 0;
    }
    return 1;
}