#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sem.h>

int msqid, semid;

struct in_mes_t
{
   long mtype;
   long nums[3];
};

struct out_mes_t
{
   long mtype;
   long num;
}; 

int GetSem(char* pathname, int init_val) {
	key_t key;
	key = ftok(pathname, 0);
	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
		printf("Can't create semaphore\n");
		exit(-1);
	}
	//printf("%d\n", semid);
	//exit(0);
	//semctl(semid, 1, SETVAL, 1);

   union semun {
		int val;
		struct semid_ds *buf;
		ushort * array;
	} argument;

   argument.val = init_val;
   semctl(semid, 0, SETVAL, argument);

   //printf("Sem: %d\n", semctl(semid, 0, GETVAL, 0));

	return semid;
}

void* Answer(void* data)
{
   sleep(1);
   struct out_mes_t out_buf;
   size_t max_out_len = sizeof(long);

   struct in_mes_t* in_buf = (struct in_mes_t*)data;

   out_buf.mtype = in_buf->nums[0];
   out_buf.num = in_buf->nums[1] * in_buf->nums[2];

   if (msgsnd(msqid, (struct msgbuf *) &out_buf, max_out_len, 0) < 0) {
      perror("Error: ");
      printf("mtype: %ld, num: %ld\n", out_buf.mtype, out_buf.num);
      printf("mtype: %ld, num0: %ld, num1: %ld, num2: %ld\n", in_buf->mtype, in_buf->nums[0], in_buf->nums[1], in_buf->nums[2]);
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
   }
   free(data);

   struct sembuf buf;

	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = 1;
	semop(semid, &buf, 1);
   //printf("Thread Sem: %d\n", semctl(semid, 0, GETVAL, 0));
   return NULL;
}

int main()
{
   char pathname[] = "server.c";
   key_t  key;
   int i,len;

   int threads_limit;
   printf("Enter treads limit: ");
   scanf("%d", &threads_limit);
   GetSem("server.c", threads_limit);


   /* Create or attach message queue  */
    
   key = ftok(pathname, 0);
    
   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid\n");
      exit(-1);
   }

   size_t max_in_len = 3 * sizeof(long);

   while(1)
   {
      struct in_mes_t* in_buf = (struct in_mes_t*)calloc(sizeof(struct in_mes_t), 1);
      if (msgrcv(msqid, (struct msgbuf *) in_buf, max_in_len, 1, 0) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
      }

      struct sembuf buf;

	   buf.sem_flg = 0;
	   buf.sem_num = 0;
	   buf.sem_op = -1;
      //printf("Sem: %d\n", semctl(semid, 0, GETVAL, 0));
	   semop(semid, &buf, 1);

      printf("Input: mtype: %ld, num0: %ld, num1: %ld, num2: %ld\n", in_buf->mtype, in_buf->nums[0], in_buf->nums[1], in_buf->nums[2]);
      //printf("Sem: %d\n", semctl(semid, 0, GETVAL, 0));
      pthread_t thid;
	   pthread_create(&thid, NULL, Answer, in_buf);

   }
   return 0;
}