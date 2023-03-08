#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LENGTH 100
#define MAX_ARGS 50
#define MAX_ARG_LEN 20
extern char **environ;

void understand(char *input);
void whatisenviron();
char **parse(char *BUFFER);
void clearparse(char **freeme);
void printstrlist(char **list);

int main(int argc, char **argv) {
    
    // Constants
    const char *PROMPT = "% "; // Define prompt
    char *BUFFER = (char *)malloc(MAX_LENGTH * sizeof(char)); // buffer temporarily stores user's input

    while(!feof(stdin)) { // Keep running as long as you're not at the end of file
        fputs(PROMPT, stdout); // Print prompt
        fgets(BUFFER, MAX_LENGTH, stdin); // Get input
        understand(BUFFER);
    }
    free(BUFFER);
    return 0;
}

void understand(char *input) {
    if (strcmp(input, "moo\n") == 0) {
        fputs("Moo to you too!\n", stdout);
    }
    if (strcmp(input, "environ\n") == 0) {
        whatisenviron();
    }
    if (strncmp(input, "parsetest", 9) == 0) {
        char **test = parse(input);
        printstrlist(test);
        clearparse(test);
    }
    return;
}

void whatisenviron() {
    char **traveler = environ;
    while(*traveler) {
        char *tmp = (char*)malloc(strlen(*traveler) + 2);
        if (tmp == NULL) {
            fputs("Failed to assign tmp in whatisenviron()", stderr);
            return;
        }
        strcpy(tmp, *traveler);
        strcat(tmp, "\n");
        fputs(tmp, stdout);
        free(tmp);
        traveler++;
    }
    return;
}

// Returns a string array of all arguments.
char **parse(char *BUFFER) {
    char **args = (char**)malloc(MAX_ARGS * sizeof(char*)); // up to 50 args accepted
    for (int i = 0; i < MAX_ARGS; ++i) {
        *(args + i) = (char*)malloc(MAX_ARG_LEN * sizeof(char));
    }
    
    char *tmpbuffer = (char*)malloc(strlen(BUFFER) + 1);
    strcpy(tmpbuffer, BUFFER);

    char *tmp = strtok(tmpbuffer, " ");

    int counter = 0;
    while (tmp != NULL) {
        strcpy(*(args + counter), tmp);
        counter++;
        tmp = strtok(NULL, " ");
    }

    free(tmpbuffer);
    return args;
}

void clearparse(char **freeme) {
    for(int i = 0; i < MAX_ARGS; ++i) {
        free(*(freeme + i));
    }
    free(freeme);
    return;
}

void printstrlist(char **list) {
    int i = 0;
    while(strlen(*(list + i)) != 0) {
        fputs(*(list + i), stdout);
        fputs("\n", stdout);
        i++;
    }
    return;
}

void arg_length() {
}

void dir() {
    
}
