#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    OK,
    NO_MEMORY
} status_codes;

typedef struct Node {
    char data;
    struct Node * parent;
    struct Node * brother;
    struct Node * son;
} Node;

status_codes add_son(Node * ptr, char value) {
    Node * _son = (Node *)malloc(sizeof(Node));
    if (!_son) return NO_MEMORY;
    _son->data = value;
    _son->parent = ptr;
    _son->brother = NULL;
    _son->son = NULL;
    ptr->son = _son;
    return OK;
}

status_codes add_brother(Node * ptr, char value) {
    Node * _brother = (Node *)malloc(sizeof(Node));
    if (!_brother) return NO_MEMORY;
    _brother->data = value;
    _brother->parent = ptr->parent;
    _brother->brother = NULL;
    _brother->son = NULL;
    ptr->brother = _brother;
    return OK;
}

status_codes create_tree(Node ** root, char * string) {
    int length = strlen(string);
    (*root) = (Node *)malloc(sizeof(Node));
    if (!(*root)) return NO_MEMORY;
    (*root)->data = string[0];
    (*root)->parent = NULL;
    (*root)->brother = NULL;
    (*root)->son = NULL;
    Node * current = (*root);
    for (int i = 1; i < length; i++) {
        char symbol = string[i];
        switch (symbol) {
            case '(':
                while (!isalpha(string[i])) i++;
                if (add_son(current, string[i]) == NO_MEMORY) return NO_MEMORY;
                current = current->son;
                break;
            case ',':
                while (!isalpha(string[i])) i++;
                if (add_brother(current, string[i]) == NO_MEMORY) return NO_MEMORY;
                current = current->brother;
                break;
            case ')':
                current = current->parent;
                break;
            default: break;
        }
    }
    return OK;
}

void print_tree(FILE * stream, Node * current, int level) {
    if (!current) return;
    print_tree(stream, current->son, level + 1);
    for (int i = 0; i < level; i++) fprintf(stream, "\t");
    fprintf(stream, "%c", current->data);
    fprintf(stream, "\n");
    print_tree(stream, current->brother, level);
}

void free_tree(Node * current) {
    if (!current) return;
    free_tree(current->son);
    free_tree(current->brother);
    free(current);
}

status_codes get_expression(FILE * in, FILE * out) {
    char * expression = NULL;
    Node * root = NULL;
    int read_status;
    while ((read_status = getline(&expression, &(size_t){0}, in)) != -1) {
        if (create_tree(&root, expression) == NO_MEMORY) {
            free_tree(root);
            return NO_MEMORY;
        }
        print_tree(out, root, 0);
        fprintf(out, "___________\n");
        free_tree(root);
        free(expression);
        expression = NULL;
    }
    if (expression) (free(expression));
    return OK;
}

int main(int argc, char * argv[]) {
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
    if (get_expression(in, out) == NO_MEMORY) {
        printf("No memory\n");
        fclose(in);
        fclose(out);
        return -1;
    }
    fclose(in);
    fclose(out);
    return 0;

}