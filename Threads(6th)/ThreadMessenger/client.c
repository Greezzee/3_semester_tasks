#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#define MAX_MESSAGE 1000

struct ThreadData
{
	char* fifo_name;
	int my_id;
};

void *Read(void* data)
{
	struct ThreadData* info = (struct ThreadData*)data;
	int fd;
	printf("Waiting\n");
	if ((fd = open(info->fifo_name, O_RDONLY)) < 0) {
		printf("Can't open FIFO for reading\n");
		exit(-1);
	}
	printf("Connected!\n");

	char message[MAX_MESSAGE + 1] = {};

	while(1) {
		int size = read(fd, message, MAX_MESSAGE);
		if (size <= 0) {
			printf("Interlocutor leaved\n");
			close(fd);
			return NULL;
		}
		printf("[%d]: %s", !info->my_id,  message);
		memset(message, '\0', MAX_MESSAGE);
	}
	return NULL;
}
void *Write(void* data)
{
	struct ThreadData* info = (struct ThreadData*)data;
	int fd;
	if ((fd = open(info->fifo_name, O_WRONLY)) < 0) {
		printf("Can't open FIFO for writting\n");
		exit(-1);
	}

	char message[MAX_MESSAGE + 1] = {};

	while(1) {
		fgets(message, MAX_MESSAGE, stdin);
		write(fd, message, MAX_MESSAGE);
		memset(message, '\0', MAX_MESSAGE);
	}
	return NULL;
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

	pthread_t read_id, write_id;

	struct ThreadData for_read, for_write;

	for_read.fifo_name = fifo_read;
	for_write.fifo_name = fifo_write;

	for_read.my_id = id;
	for_write.my_id = id;

	pthread_create(&read_id, NULL, Read, &for_read);
	pthread_create(&write_id, NULL, Write, &for_write);

	pthread_join(read_id, NULL);

	return 0;
}
