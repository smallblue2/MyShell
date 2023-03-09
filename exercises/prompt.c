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
void print_args(char **args);

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

// internal commands
void understand(char *input) {
    char **args = parse(input);
    if (*args != NULL) {
        if (strcmp(*args, "clr") == 0) {
            system("clear");
        } else if (strcmp(*args, "quit") == 0) {
            exit(0);
        } else if (strcmp(*args, "moo") == 0) {
            fputs("moo to you too!\n", stdout);
        } else if (strcmp(*args, "environ") == 0) {
            whatisenviron();
        } else if (strcmp(*args, "arglist") == 0) {
            print_args(args);
        } else if (strcmp(*args, "dir") == 0) {
            if (*(args + 1) != NULL) {
                int cmdlen = strlen("ls -al ") + strlen(*(args + 1)) + 1;
                char *command = (char*)malloc(cmdlen * sizeof(char));
                strcpy(command, "ls -al ");
                strcat(command, *(args + 1));
                *(command + cmdlen) = '\0';
                system(command);
                free(command);
                // show the specific directory
            } else {
                system("ls -al");
            }
        } else {
            system(*args);
        }
    }
    freeparse(args);
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
        //printf("cleared mem %i\n", counter);
        counter++;
    }
    free(parsed);
    return;
}

void print_args(char **args) {
    fputs("====== args ======\n", stdout);
    int counter = 0;
    while (*(args + counter) != NULL) {
        fputs(*(args + counter), stdout);
        fputs("\n", stdout);
        counter++;
    }
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
