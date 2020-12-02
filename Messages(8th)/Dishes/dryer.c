#include <sys/sem.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#define CHAR_VAL_COUNT 256

size_t GetFileSize(FILE* file)
{
    fseek(file, 0L, SEEK_END); 
    size_t res = ftell(file);
    fseek(file, 0, SEEK_SET);
    return res;
}

int GetSem(char* pathname) {
	key_t key;
	key = ftok(pathname, 0);
    int semid;
	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
		printf("Can't create semaphore\n");
		exit(-1);
	}
	return semid;
}

int GetMsgId(char* pathname) {
    key_t key = ftok(pathname, 0);
    int msgid;
    if ((msgid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    return msgid;
}

void InitSpeed(char* pathname, int* speed)
{
    for (int i = 0; i < CHAR_VAL_COUNT; i++)
        speed[i] = -1;

    FILE* file = fopen(pathname, "r");
    unsigned type = 0;
    printf("Dryer speed at elements:\n");
    while (fscanf(file, " [%c] ", &type) == 1 && fscanf(file, " %d ", speed + type) == 1)
        printf("[%c]: %d\n", type, speed[type]);
}

char GetFromTable(int semid, int msgid)
{
    struct message {
        long mtype;
        char data;
    } mes;

    if (msgrcv(msgid, (struct msgbuf *) &mes, sizeof(char), 1, 0) < 0) {
        perror("Error: unable to take from table\n");
        msgctl(msgid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    struct sembuf buf;
	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = 1;
	semop(semid, &buf, 1);

    return mes.data;
}

int main()
{
    int speed[CHAR_VAL_COUNT] = {};
    InitSpeed("dryer_speed.info", speed);
    int semid = GetSem("table.info");
    int msgid = GetMsgId("table.info");

    char data;
    int i = 0;

    while (1) {
        printf("%d:\nWaiting for something on table\n", i);
        data = GetFromTable(semid, msgid);
        if (speed[data] == -1) {
            printf("Incorrect element [%c]\n", data);
            msgctl(msgid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("[%c] taken from table. Working at [%c]\n", data, data);
        sleep(speed[data]);
        printf("Work at [%c] done.\n\n", data);
        i++;
    }
    return 0; 
}