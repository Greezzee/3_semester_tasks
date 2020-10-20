#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int a[3] = {};

int GetSem(char* pathname) {
	int semid;
	key_t key;
	key = ftok(pathname, 0);
	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
		printf("Can't create semaphore\n");
		exit(-1);
	}
	return semid;
}

void* thread1(void* dummy)
{
	struct sembuf buf;
	int semid = GetSem("race.c");

	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = 0;

	buf.sem_op = 1;
	semop(semid, &buf, 1);

	printf("%d\n", semctl(semid, 1, GETVAL));

	for (int i = 0; i < 10000000; i++) {
		buf.sem_op = 1;
		semop(semid, &buf, 1);
		a[0]++;
		a[2]++;
		buf.sem_op = 0;
		semop(semid, &buf, 1);
	}
	return NULL;
}

void* thread2(void* dummy)
{
	struct sembuf buf;
	int semid = GetSem("race.c");

	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = 0;

	for (int i = 0; i < 10000000; i++) {
		//buf.sem_op = 1;
		//semop(semid, &buf, 1);
		a[1]++;
		a[2]++;
		buf.sem_op = 0;
		semop(semid, &buf, 1);
	}
	return NULL;
}

int main() {
	pthread_t thid1, thid2;
	pthread_create(&thid1, NULL, thread1, NULL);
	pthread_create(&thid2, NULL, thread2, NULL);

	pthread_join(thid1, NULL);
	pthread_join(thid2, NULL);

	printf("a[0] = %d\na[1] = %d\na[2] = %d\na[0] + a[1] = %d\n", a[0], a[1], a[2], a[0] + a[1]);
	return 0;
}
