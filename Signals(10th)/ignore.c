#include <stdio.h>
#include <signal.h>

void test(int input) {
    printf("Nope :>\n");
}

int main()
{
    signal(SIGINT, &test);

    while(1);
    return 0;
}