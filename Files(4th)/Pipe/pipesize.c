#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	size_t pipe_size = 0;
	int fd[2];
	flock_t lock;
	lock.l_type = F_UNLCK;
	fcntl(fd[1], F_SETLK, &lock);
	if (pipe(fd) < 0) {
		printf("Can't open pipe\n");
		exit(-1);
	}
	char c[1];
	
	while(write(fd[1], c, 1) == 1) {
		pipe_size++;
		printf("Pipe size: %d\n", pipe_size);
	}

	close(fd[0]);
	close(fd[1]);

	printf("Pipe size: %u\n", pipe_size);
	return 0;	
}
