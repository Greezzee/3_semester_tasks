#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int fd[2];

	if (pipe(fd) < 0) {
		printf("Pipe not opened\n");
		exit(-1);
	}

	pid_t pid = fork();

	if (pid < 0) {
		printf("Process don't fork\n");
		exit(-1);
	}
	else if (pid == 0) {

		printf("Multiplicator starting. Waiting for data\n");
		int input[2];

		if (read(fd[0], input, sizeof(int) * 2) != sizeof(int) * 2) {
			printf("Incorrect pipe read\n");
			exit(-1);
		}
		printf("Multiplicator gets enought data\n");
		int out = input[0] * input[1];

		if (write(fd[1], &out, sizeof(int)) != sizeof(int)) {
			printf("Incorrect pipe write\n");
			exit(-1);
		}
		printf("Multiplicator exiting\n");
		close(fd[0]);
		close(fd[1]);
		exit(0);
	}
	else {
		int in[2];

		printf("Enter a and b:\n");
		scanf("%d %d", in, in + 1);

		if (write(fd[1], in, sizeof(int) * 2) != sizeof(int) * 2) {
			printf("Incorrect pipe write\n");
			exit(-1);
		}

		int out;
		int status;

		wait(&status);

		if (read(fd[0], &out, sizeof(int)) != sizeof(int)) {
			printf("Incorrect pipe read\n");
			exit(-1);
		}
		close(fd[0]);
		close(fd[1]);
		printf("a * b = %d\n", out);
	}
	return 0;

}
