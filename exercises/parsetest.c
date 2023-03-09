#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 100

int main() {
    char input[1000000];
    char *tokens[MAX_TOKENS];
    int count = 0;

    // Read input from user
    printf("Enter a line of text: ");
    fgets(input, sizeof(input), stdin);

    // Split the input into tokens
    char *token = strtok(input, " \n");
    while (token != NULL && count < MAX_TOKENS) {
        tokens[count] = malloc(strlen(token) + 1);
        if (tokens[count] == NULL) {
            fprintf(stderr, "Memory allocation error");
            exit(1);
        }
        strcpy(tokens[count], token);
        count++;
        token = strtok(NULL, " \n");
    }

    // Print the tokens
    for (int i = 0; i < count; i++) {
        printf("%s\n", tokens[i]);
    }

    // Free the memory allocated for the token array
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }

    return 0;
}

