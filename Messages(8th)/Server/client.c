#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int msqid;
    char pathname[] = "server.c";
    key_t  key;
    int i,len;

    struct in_mes_t
    {
       long mtype;
       long nums[3];
    } in_buf;

    struct out_mes_t
    {
       long mtype;
       long num;
    } out_buf;



    /* Create or attach message queue  */
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }

    size_t max_in_len = 3 * sizeof(long);
    size_t max_out_len = sizeof(long);

    scanf("%ld %ld", in_buf.nums + 1, in_buf.nums + 2);
    in_buf.nums[0] = getpid();
    in_buf.mtype = 1;

    printf("Sending data to server\n");

    if (msgsnd(msqid, (struct msgbuf *) &in_buf, max_in_len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    printf("Waiting for server\n");
    if (msgrcv(msqid, (struct msgbuf *) &out_buf, max_out_len, getpid(), 0) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }

    printf("Answer from server: %d\n", out_buf.num);
    return 0;
}