#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

extern int errno;

int main(int argc, char **argv) {
//    FILE *original_stdout = stdout;
//    if (!freopen("./test.txt", "w+", stdout)) { // return value
//        fprintf(stderr, "ERROR: %s\n", strerror(errno));
//    }

    pid_t pid;
    switch(pid = fork()) {
        case -1:
            printf("Error: %s\n", strerror(errno));
        case 0:
            printf("Child process %i running\n", getpid());
            freopen("./test.txt", "w+", stdout);
            execlp("ls", "ls", "-l", NULL);
            printf("uh oh");
        default:
            waitpid(pid, NULL, 0);
    }

    printf("Finished!\n");
}
