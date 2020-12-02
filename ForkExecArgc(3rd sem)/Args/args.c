#include <stdio.h>

int main(int argc, char* argv[], char* env[])
{
	printf("Arguments:\n");
	for (int i = 0; i < argc; i++)
		printf("[%d]: %s\n", i, argv[i]);
	printf("Enviroment:\n");
	for (int i = 0; env[i] != NULL; i++)
		printf("[%d]: %s\n", i, env[i]);
	return 0;
}
