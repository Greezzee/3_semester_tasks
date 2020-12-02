#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main() {
	printf("Parend: PID: %d. PPID: %d\n", getpid(), getppid());
	for (int i = 0; i < 5; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			printf("Child %d: PID: %d. PPID: %d\n", i + 1, getpid(), getppid());
			exit(i);
		}
		int status = 0;
		wait(&status);
		printf("Child %d exit with status %d\n", i + 1, status);
	}
	sleep(1);
	return 1;
}
