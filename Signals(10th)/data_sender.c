#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 1000
int str_len;
char in[MESSAGE_SIZE], out[MESSAGE_SIZE];
int in_pos = 0, out_pos = 0;
int child_pid;

void set_bit(void* data, int pos, int bit)
{
    if(bit)
        ((char*)data)[pos / 8] |= 1 << (pos % 8);
    else
        ((char*)data)[pos / 8] &= 255 - (1 << (pos % 8));
}

int get_bit(void* data, int pos)
{
    return (((char*)data)[pos / 8] & (1 << (pos % 8))) >> (pos % 8);
}

void send_ready()
{
    kill(child_pid, SIGCHLD);
}

void print(int signal)
{
    printf("\n%s\n", in);
    exit(0);
}

void get0(int signal)
{
    set_bit(in, in_pos, 0);
    in_pos++;
    printf("0\n");
    if (in_pos >= str_len * 8)
        print(0);
    send_ready();
}

void get1(int signal)
{
    set_bit(in, in_pos, 1);
    in_pos++;
    printf("1\n");
    if (in_pos >= str_len * 8)
        print(0);
    send_ready();
}

void send_next(int signal)
{
    if (get_bit(out, out_pos))
        kill(getppid(), SIGUSR2);
    else
        kill(getppid(), SIGUSR1);
    out_pos++;
}

int main() {
    signal(SIGUSR1, get0);
    signal(SIGUSR2, get1);
    signal(SIGCHLD, send_next);
    scanf("%[^\n]", out);
    str_len = strlen(out);
    for (int i = 0; i < str_len * 8; i++)
        printf("%d", get_bit(out, i));
    printf("\n");
    int pid = fork();

    if (pid == 0) {
        while(1);
    }
    else {
        child_pid = pid;
        send_ready();
        while(1);
    }

    return 0;
}