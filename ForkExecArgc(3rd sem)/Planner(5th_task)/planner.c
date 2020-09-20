#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include "../../Split/SplitFunc.h"

size_t GetFileSize(FILE* file) {
	fseek(file, 0, SEEK_END);
	size_t out = ftell(file);
	fseek(file, 0, SEEK_SET);
	return out;
}

struct command_t {
	unsigned delay;
	unsigned words_count;	
	char** command;
};

int command_comp(const void* a, const void* b) {
	struct command_t* ca = (struct command_t*)a;
	struct command_t* cb = (struct command_t*)b;
	return ca->delay - cb->delay;
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Enter filename as argument:\n./planner [filename]\n");
		return -1;
	}

	FILE* plan_file = fopen(argv[1], "r");
	size_t file_size = GetFileSize(plan_file);
	char* plan = (char*)calloc(file_size + 1, sizeof(char));
	fread(plan, sizeof(char), file_size - 1, plan_file);
	char** command_str = (char**)calloc(file_size + 1, sizeof(char*));

	size_t command_count = Split(plan, "\n", command_str);
	struct command_t* command = (struct command_t*)calloc(command_count, sizeof(struct command_t));

	for (size_t i = 0; i < command_count; i++) {
		char* com_start = command_str[i];
		while(isdigit(command_str[i][0]))
			command_str[i]++;
		while(isspace(command_str[i][0])) {
			command_str[i][0] = '\0';
			command_str[i]++;
		}
		if (command_str[i][0] == '\0')
			continue;

		command[i].delay = atoi(com_start);
		command[i].command = (char**)calloc(file_size + 1, sizeof(char*));
		command[i].words_count = Split(command_str[i], " ", command[i].command);
	}

	qsort(command, command_count, sizeof(struct command_t), command_comp);

	for (size_t i = 1; i < command_count; i++)
		command[i].delay -= command[i - 1].delay;
	
	for (size_t i = 0; i < command_count; i++) {
		sleep(command[i].delay);
		pid_t pid = fork();
		if (pid == 0) {
			execvp(command[i].command[0], command[i].command);
				
			printf("\033[0;31m");
			printf("Incorrect command: ");
			printf("\033[0m");
			for (size_t j = 0; j < command[i].words_count; j++)
				printf("%s ", command[i].command[j]);
			printf("\n");
			exit(1);
		}
		else {
			int status = 0;
			wait(&status);
		}
	}

	return 0;
	
}
