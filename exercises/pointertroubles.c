#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv) {
    int *n = (int *)malloc(sizeof(int));
    n = NULL;
    printf("%i\n", *n);
    free(n);
}
