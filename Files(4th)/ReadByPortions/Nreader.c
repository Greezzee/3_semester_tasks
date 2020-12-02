#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
int main()
{
	int fd;
	
	char* filename = (char*)calloc(PATH_MAX, sizeof(char));
	size_t port = 0;

	printf("Enter file name: ");
	scanf("%s", filename);
	printf("Enter text protion (N): ");
	scanf("%u", &port);

	if ((fd = open(filename, O_RDONLY)) < 0) {
		printf("Can't open file [%s]\n", filename);
		exit(-1);
	}
	free(filename);

	char* text = (char*)calloc(port + 1, sizeof(char));

	int s = 0;
	while ((s = read(fd, text, port)) != 0) {
		printf("[%s]\n\n", text);
		memset(text, '\0', port + 1);

	}
	
	free(text);
	close(fd);

	return 0;
}
