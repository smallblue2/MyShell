#include<stdio.h>
#include<fcntl.h>
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
extern int errno;

void understand(char *input, char *cwd);
void whatisenviron();
char **parse(char *buffer);
void freeparse(char **parsed);
void print_args(char **args);
void external(char **args, char *mode, char *output);
int isampersand(char **args);
void errhan();
char *inputredirection(char **args);
char *outputredirection(char **args, char *mode);

int main(int argc, char **argv) {    
    // Constants
    const char *PROMPT = "%> "; // Define prompt
    char* CWD = (char*)malloc(MAX_PATH * sizeof(char)); // setup cwd
    char *BUFFER = (char *)malloc(MAX_LENGTH * sizeof(char)); // buffer temporarily stores user's input
    if (!BUFFER) {
        fputs("ERROR: memory allocation failed", stderr);
        exit(-1);
    }
    while(!feof(stdin)) { // Keep running as long as you're not at the end of file
        if (getcwd(CWD, MAX_PATH) == NULL) {errhan();}
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
                errhan();
            case 0:
                if (execlp("clear", "clear", NULL)==-1) {
                    errhan();
                }
            default:
                waitpid(pid, NULL, 0);
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
            char *mode = (char*)malloc(3 * sizeof(char));
            char *output = outputredirection(args, mode);
            external(args, mode, output);
        }
    }
    freeparse(args);
    return;
}

char *inputredirection(char **args);


char *outputredirection(char **args, char *mode) {

    //printf("Searching.\n");
    int i = 0;
    while(*(args + i) != NULL) {
        if (strcmp(*(args + i), ">")==0) {
            strcpy(mode, "w+");
            break;
        }
        if (strcmp(*(args + i), ">>")==0) {
            strcpy(mode, "a+");
            break;
        }
        //fputs(*(args + i), stdout);
        //fputs("\n", stdout);
        i++;
    }
    //printf("Finished searching.\n");
    if (*(args + i) == NULL) return NULL;
    if (strcmp(*(args + i), ">") != 0 && strcmp(*(args + i), ">>") != 0) return NULL;
    if (*(args + i + 1) == NULL) return NULL;

    //fputs("output redirection detected.\n", stdout);

    char *pathname = (char*)malloc(MAX_PATH * sizeof(char));
    strcpy(pathname, *(args + i + 1));
    //FILE *exists = fopen(pathname, "r"); // check if the file exists
    //if (exists) {
    //    *(args + i) = NULL;
    //    *(args + i + 1) = NULL;
    //    return pathname;
    //}
    *(args + i) = NULL;
    *(args + i + 1) = NULL;
    return pathname;
    //fprintf(stderr, "can't find file\n");
    free(pathname);
    return NULL;
}

// for executing external commands
void external(char **args, char *mode, char *output) {
    pid_t pid; // hold pid of the child process
    int parallel = isampersand(args);
    // check if parralell or not 
    

    switch(pid = fork()) { // fork process, returns child pid
        case -1:
            printf("execution failed!"); // failed to fork
        case 0:
            if (output) {
                freopen(output, mode, stdout);
            }
            if (execvp(*args, args)==-1) { // runs command and checks if failed
                pid_t child = getpid(); // re-gets child pid
                printf("Child process %i failed to execute command: %s\n", child, *args); // alerts failure
                errhan();
                kill(child, SIGTERM); // kills child process
            }
        default:
            if (!parallel) { // wait for child process just created
                waitpid(pid, NULL, 0); // NULL is for return value, don't need it
            }
    }
    free(mode);
    free(output);
}

// checks if ampersand is final character
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

// displays environ
void whatisenviron() {
    char **traveler = environ;
    while(*traveler) {
        char *tmp = (char*)malloc(strlen(*traveler) + 2);
        if (tmp == NULL) {
            fputs("Failed to assign tmp in whatisenviron()", stderr); // don't need to exit
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

// parses input buffer
char **parse(char *buffer) {
    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));
    if (!args) {
        printf("FAILED: Memory assignment in parser");
        exit(-1);
    }
    
    char *arg = strtok(buffer, " \n");
    int arg_count = 0;
    while (arg != NULL && arg_count < MAX_ARGS) {
        *(args + arg_count) = (char*)malloc(strlen(arg) + 1);
        if (*(args + arg_count) == NULL) {
            fprintf(stderr, "FAILED: memory assigned to args");
            exit(-1);
        }
        strcpy(*(args + arg_count), arg);
        arg_count++;
        arg = strtok(NULL, " \n");
    }
    *(args + arg_count) = NULL;

    return args;
}

// frees parsed buffer
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

// displays args, mainly for debugging
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

void errhan() {
    fprintf(stderr, "ERROR: %s\n", strerror(errno));
}
