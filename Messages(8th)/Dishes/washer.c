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
    printf("Washer speed at elements:\n");
    while (fscanf(file, " [%c] ", &type) == 1 && fscanf(file, " %d ", speed + type) == 1)
        printf("[%c]: %d\n", type, speed[type]);
}

void PutOnTable(char elem, int semid, int msgid)
{
    struct message {
        long mtype;
        char data;
    } mes;

    struct sembuf buf;

	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = -1;
	semop(semid, &buf, 1);

    mes.mtype = 1;
    mes.data = elem;

    if (msgsnd(msgid, (struct msgbuf *) &mes, sizeof(char), 0) < 0) {
        perror("Error: unable to put on table\n");
        msgctl(msgid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
}

int main()
{
    int speed[CHAR_VAL_COUNT] = {};
    InitSpeed("washer_speed.info", speed);
    int semid = GetSem("table.info");
    int msgid = GetMsgId("table.info");

    FILE* data_file = fopen("data.info", "r");
    size_t data_size = GetFileSize(data_file);
    char* data = (char*)calloc(data_size + 1, sizeof(char));

    fread(data, sizeof(char), data_size, data_file);
    printf("[%s]\n", data);

    for (size_t i = 0; i < data_size; ++i) {
        if (speed[data[i]] != -1) {
            printf("%d:\nWorking at [%c]\n", i, data[i]);
            sleep(speed[data[i]]);
            printf("Work at [%c] done. Waiting for table\n", data[i]);
            PutOnTable(data[i], semid, msgid);
            printf("[%c] put on table\n\n", data[i]);
        }
    }

    free(data);
    printf("Washer done\n");
    return 0; 
}