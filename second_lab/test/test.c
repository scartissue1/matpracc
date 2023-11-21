#include <stdio.h>

size_t _strlen(const char * string) {
    size_t size = 0;
    while (string[++size] != '\0');
    return size;
}

int main() {
    void * f = 10101010;
    void * arr = "1342";
    // scanf("  %d %s,   ", &f, buf);
    printf("%p %p %ld\n", f, arr, _strlen(f));
}