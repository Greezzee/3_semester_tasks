#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/msg.h>

void InitSem(int semid, int init_val)
{
    union semun {
		int val;
		struct semid_ds *buf;
		ushort * array;
	} argument;
   argument.val = init_val;
   semctl(semid, 0, SETVAL, argument);
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

void RemoveMsg(int msgid) {
    msgctl(msgid, IPC_RMID, (struct msqid_ds *) NULL);
}

int RecreateMsg(char* pathname) {
    RemoveMsg(GetMsgId(pathname));
    return GetMsgId(pathname);
}

int main() {
    size_t table_size = 0;
    printf("Enter table size: ");
    scanf("%lu", &table_size);
    int semid = GetSem("table.info");
    InitSem(semid, table_size);
    RecreateMsg("table.info");
    printf("Table size setted\n");
    return 0;
}