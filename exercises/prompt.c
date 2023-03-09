#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LENGTH 100
#define MAX_ARGS 50
#define MAX_ARG_LEN 20
extern char **environ;

void understand(char *input);
void whatisenviron();
char **parse(char *buffer);
void freeparse(char **parsed);

int main(int argc, char **argv) {
    
    // Constants
    const char *PROMPT = "% "; // Define prompt
    char *BUFFER = (char *)malloc(MAX_LENGTH * sizeof(char)); // buffer temporarily stores user's input
    if (!BUFFER) {
        fputs("Failed to allocate buffer memory!", stdout);
    }
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
        //printf("not implemented yet");
        char **args = parse(input);
        freeparse(args);
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

char **parse(char *buffer) {
    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));
    if (!args) {
        printf("FAILED: memory assigned to args");
        return NULL;
    }
    
    char *arg = strtok(buffer, " \n");
    int arg_count = 0;
    while (arg != NULL && arg_count < MAX_ARGS) {
        *(args + arg_count) = (char*)malloc(strlen(arg) + 1);
        if (*(args + arg_count) == NULL) {
            fprintf(stderr, "FAILED: memory assigned to args");
            exit(1);
        }
        strcpy(*(args + arg_count), arg);
        arg_count++;
        arg = strtok(NULL, " \n");
    }
    *(args + arg_count) = NULL;

    return args;
}

void freeparse(char **parsed) {
    int counter = 0;
    while(*(parsed + counter) != NULL) {
        free(*(parsed + counter));
        printf("cleared mem %i\n", counter);
        counter++;
    }
    free(parsed);
    return;
}


// Returns a string array of all arguments.
//char **parse(char *BUFFER) {
//    char **args = (char**)malloc((MAX_ARGS + 1) * sizeof(char*)); // up to 50 args accepted + 1 for NULL
//    if (!args) {
//        fputs("Failed to allocate memory for arg array", stdout);
//        return NULL; // Error handling here?
//    }
//    for (int i = 0; i < MAX_ARGS + 1; ++i) { // Setting 50 args + 1 for NULL
//        *(args + i) = (char*)malloc(MAX_ARG_LEN * sizeof(char));
//        if (!*(args + i)) {
//            fputs("Failed to allocate memory for individual arg", stdout);
//            return NULL; // Error handling here?
//        }
//    }
//    *(args + MAX_ARGS) = NULL;
//    
//    //char *tmpbuffer = (char*)malloc(strlen(BUFFER) + 1);
//    //strcpy(tmpbuffer, BUFFER);
//
//    char *tmp = strtok(BUFFER, " ");
//
//    int counter = 0;
//    while (tmp != NULL) {
//        strcpy(*(args + counter), tmp);
//        counter++;
//        tmp = strtok(NULL, " ");
//    }
//
//    return args;
//}
//
//void clearparse(char **freeme) {
//    for(int i = 0; i < MAX_ARGS + 1; ++i) {
//        free(*(freeme + i));
//    }
//    free(freeme);
//    return;
//}
//
//void printstrlist(char **list) {
//    int i = 0;
//    while(strlen(*(list + i)) != 0) {
//        fputs(*(list + i), stdout);
//        fputs("\n", stdout);
//        i++;
//    }
//    return;
//}
//
//int args_length(char **args) {
//    int counter = 0;
//    while (strlen(*(args + counter)) != 0) {
//        counter++;
//    }
//    return counter;
//}
//
//void dir() {
//    
//}
