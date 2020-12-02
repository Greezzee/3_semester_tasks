#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int a[3] = {};

void* thread1(void* dummy)
{
	for (int i = 0; i < 10000000; i++) {
		a[0]++;
		a[2]++;
	}
	return NULL;
}

void* thread2(void* dummy)
{
	for (int i = 0; i < 10000000; i++) {
		a[1]++;
		a[2]++;
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
