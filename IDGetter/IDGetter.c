#include<stdio.h>
#include <unistd.h>
int main()
{
    printf("User ID: %d\n", getuid());
    printf("Group ID: %d\n", getgid());
    return 0;
}