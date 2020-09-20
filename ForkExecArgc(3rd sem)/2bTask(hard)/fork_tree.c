#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

struct Process {
	int id;
	int parent_id;
};


int main() {
	int proc_count = 0;
	scanf("%d", &proc_count);

	struct Process* proc = (struct Process*)calloc(proc_count, sizeof(struct Process));


	for (int i = 0; i < proc_count; i++) {
		scanf("%d %d", &proc[i].id, &proc[i].parent_id);
	}
	
	int status = 0;
	int my_id = -1;
	int parent_id = -1;
	int child_proc_count = 0;

	printf("Base process created, id = -1\n");

	for (int i = 0; i < proc_count; i++) {
		if(proc[i].parent_id == my_id) {
			pid_t pid = fork();
			if (pid == -1) {
				printf("Error\n)");
				exit(1);
			}
			else if(pid == 0) {
				parent_id = my_id;
				my_id = proc[i].id;
				i = 0;
				child_proc_count = 0;
				printf("Child process created (id = %d). Parent id = %d\n", my_id, parent_id);
			}
			else
				child_proc_count++;
		}
	}

	for (int j = 0; j < child_proc_count; j++)
		wait(&status);

	printf("Process id = %d exited. Parent id = %d\n", my_id, parent_id);

	return 0;
}
