#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char **argv) {
    char *path = (char*)malloc(1024*sizeof(char));
    char *filename = (char*)malloc(1024*sizeof(char));
    realpath("./", path);
    strcat(path, "/path");
    printf("The file path is: %s\n", path);
}
