#include<stdio.h>
#include<stdlib.h>

#define MAX_LENGTH 100

void *assign(int size, int type);

int main(int argc, char**argv) {
    char *buffer = (char *)assign(MAX_LENGTH, sizeof(char));
    if (buffer == NULL) {return 1;}
    fgets(buffer, MAX_LENGTH, stdin);
    fputs(buffer, stdout);
    free(buffer);
}

void *assign(int size, int type) {
    void *tmp = malloc(size * type);
    if (tmp == NULL) {
        return NULL;
    }
    return tmp;
}
