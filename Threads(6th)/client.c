#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define MAX_MESSAGE 1000

void Read(const char* fifo_name, const int my_id)
{
	int fd;
	if ((fd = open(fifo_name, O_RDONLY)) < 0) {
		printf("Can't open FIFO for reading\n");
		exit(-1);
	}

	char message[MAX_MESSAGE + 1] = {};

	while(1) {
		int size = read(fd, message, MAX_MESSAGE);
		if (size <= 0) {
			printf("Interlocutor leaved\n");
			close(fd);
			exit(0);
		}
		printf("[%d]: %s", my_id,  message);
		memset(message, '\0', MAX_MESSAGE);
	}
}
void Write(const char* fifo_name, const int my_id)
{

	int fd;
	if ((fd = open(fifo_name, O_WRONLY)) < 0) {
		printf("Can't open FIFO for writting\n");
		exit(-1);
	}

	char message[MAX_MESSAGE + 1] = {};

	while(1) {
		fgets(message, MAX_MESSAGE, stdin);
		write(fd, message, MAX_MESSAGE);
		memset(message, '\0', MAX_MESSAGE);
	}
}

int main(int argc, char** argv) {
	(void)umask(0);

	int f1 = mknod("0", S_IFIFO | 0666, 0);

	if (f1 < 0 && errno != EEXIST) {
		printf("Cant create 0 FIFO\n");
		exit(-1);
	}

	int f2 = mknod("1", S_IFIFO | 0666, 0);

	if (f2 < 0 && errno != EEXIST) {
		printf("Cant create 1 FIFO\n");
		exit(-1);
	}

	pid_t pid = fork();
	int id = atoi(argv[1]);
	
	char fifo_read[2] = {};
	char fifo_write[2] = {};

	if (id == 0) {
		fifo_read[0] = '1';
		fifo_write[0] = '0';
	}
	else {
		fifo_read[0] = '0';
		fifo_write[0] = '1';
	}

	if (pid < 0) {
		printf("Can't fork\n");
		exit(-1);
	}
	else if (pid == 0)
		Write(fifo_write, id);
	else
		Read(fifo_read, !id);
	return 0;
}
