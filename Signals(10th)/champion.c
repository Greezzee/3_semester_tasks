#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int child_pid;

void signal1(int s) {
    printf("RT\n");
    sleep(1);
    kill(child_pid, SIGUSR2);
}

void signal2(int s) {
    printf("Champion\n");
    sleep(1);
    kill(getppid(), SIGUSR1);
}

void run1() {
    signal1(0);
    while(1);
}

void run2() {
    while(1);
}

int main() {
    signal(SIGUSR1, signal1);
    signal(SIGUSR2, signal2);
    int pid = fork();
    if (pid == 0)
        run2();
    else {
        child_pid = pid;
        run1();
    }
    return 0;
}