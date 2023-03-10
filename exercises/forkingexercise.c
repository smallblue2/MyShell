#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char **argv) {
    pid_t pid = getpid();
    printf("Process ID before fork: %i\n", pid);

    

    switch(fork()) {
        case -1:
            fprintf(stderr, "%s: %s", strerror(errno), "fork");
        case 0:
            pid = getpid();
            printf("Process ID in child after fork %i\n", pid);
            execlp("/usr/bin/echo", "echo", "hello", NULL);
    }

    int status = -1;
    wait(&status);
    pid = getpid();
    printf("Process ID after fork: %i\n", pid);

    return 0;
}
