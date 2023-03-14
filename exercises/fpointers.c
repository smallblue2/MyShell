#include<stdio.h>

int add(int x ,int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

int main(int argc, char **argv) {
    int (*operation)(int, int);
    int x = 10, y = 5;

    operation = &add;
    printf("Added: %i\n", operation(x, y));
    operation = &subtract;
    printf("Subtracted: %i\n", operation(x, y));

    return 0;
}
