#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<dirent.h>
#include<errno.h>

#define MAX_LENGTH 100
#define MAX_ARGS 50
#define MAX_ARG_LEN 20
#define MAX_PATH 260
extern char **environ;

void understand(char *input, char *cwd);
void whatisenviron();
char **parse(char *buffer);
void freeparse(char **parsed);
void print_args(char **args);
void external(char **args);
int isampersand(char **args);

int main(int argc, char **argv) {    
    // Constants
    const char *PROMPT = "%> "; // Define prompt
    char* CWD = (char*)malloc(MAX_PATH * sizeof(char)); // setup cwd
    char *BUFFER = (char *)malloc(MAX_LENGTH * sizeof(char)); // buffer temporarily stores user's input
    if (!BUFFER) {
        fputs("Failed to allocate buffer memory!", stdout);
    }
    while(!feof(stdin)) { // Keep running as long as you're not at the end of file
        getcwd(CWD, MAX_PATH);
        fputs(CWD, stdout);
        fputs(PROMPT, stdout); // Print prompt
        fgets(BUFFER, MAX_LENGTH, stdin); // Get input
        understand(BUFFER, CWD);
    }
    free(CWD);
    free(BUFFER);
    return 0;
}

// internal commands
void understand(char *input, char *cwd) {
    char **args = parse(input);
    if (*args != NULL) {
        if (strcmp(*args, "clr") == 0) {
            pid_t pid;
            switch(pid = fork()) {
            case -1:
                printf("Uh oh, something went wrong!");
            case 0:
                execlp("clear", "clear", NULL);
                printf("Uh oh, something went wrong!");
            default:
                int status;
                wait(&status);
            }
        } else if (strcmp(*args, "quit") == 0) {
            exit(0);
        } else if (strcmp(*args, "moo") == 0) {
            fputs("moo to you too!\n", stdout);
        } else if (strcmp(*args, "environ") == 0) {
            whatisenviron();
        } else if (strcmp(*args, "arglist") == 0) {
            print_args(args);
        } else if (strcmp(*args, "pwd") == 0) { // pwd implementation
            fputs(cwd, stdout);
            fputs("\n", stdout);
        } else if (strcmp(*args, "cd") == 0) { // cd implementation
            if (*(args + 1) != NULL) {
                char *relpathname = (char*)malloc(MAX_PATH * sizeof(char));
                if (relpathname == NULL) {
                    fputs("Failed to assign memory for cd operation!", stdout);
                    freeparse(args);
                    return;
                }   
                // sets relpathname
                strcpy(relpathname, cwd);
                strcat(relpathname, "/");
                strcat(relpathname, *(args + 1));

                DIR* dir = opendir(relpathname); // check to see if it exists (relative path);
                if (dir) { // it's a relative path
                    chdir(relpathname);
                    setenv("PWD", relpathname, 1);
                } else if (ENOENT == errno) { // Directory doesn't exist!
                    // check absolute path
                    dir = opendir(*(args + 1));
                    if (dir) {
                        // it's an absolute path
                        chdir(*(args + 1));
                        setenv("PWD", *(args + 1), 1);
                    } else if (ENOENT == errno) {
                        fputs("Directory doesn't exist!\n", stdout);
                    }
                } else if (EACCES == errno) { // It's not a directory at all!
                    fputs("Permission denied!\n", stdout);
                } else { // Something's gone wrong!
                    fputs("Operation failed\n", stdout);
                }
                closedir(dir);
                free(relpathname);
            } else {
                chdir(getenv("HOME"));
            }
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
        } else { // it's an external command
            external(args);
        }
    }
    freeparse(args);
    return;
}

void external(char **args) {
    pid_t pid; // hold pid of the child process
    int parallel = isampersand(args);
    // check if parralell or not 
    

    switch(pid = fork()) {
        case -1:
            printf("execution failed!");
        case 0:
            execvp(*args, args);
            printf("Child process %i failed to execute command: %s\n", getpid(), *args);
            exit(0);
        default:
            if (!parallel) {
                waitpid(pid, NULL, 0);
            }
            parallel = 0;
    }
}

int isampersand(char **args) {
    int i = 0;
    while (*(args + i + 1)) {
        i++;
    }
    if (strcmp(*(args + i), "&") == 0) {
        *(args + i) = NULL;
        free(*(args + i));
        return 1;
    }
    return 0;
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
