#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Child process is ending. Child process became Zombie.\n");
        exit(0); 
    } else {
        printf("Parent process is sleeping.\n"); 
        sleep(10);
        printf("Parent woke up.\n");
        int status;
        pid_t finished_pid = wait(&status);
        printf("Calling \'wait\'. Zombie process is killed.\n");
        printf("Ending program..\n");
    }

    return 0;
}



