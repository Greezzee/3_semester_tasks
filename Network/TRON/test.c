#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define FIELD_X 100
#define FIELD_Y 50

struct termios oldChars, newChars;

void initTermios(int echo)
{
    fcntl(0, F_SETFL, O_NONBLOCK);
    tcgetattr(0, &oldChars);
    newChars = oldChars;
    newChars.c_lflag &= ~ICANON;
    newChars.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(0, TCSANOW, &newChars);
}

int main() {

    struct sockaddr_in addr, addr_to_recv;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(51707);
    inet_aton("localhost", &addr.sin_addr);
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(addr.sin_zero, 8);
    struct sockaddr test;
    int my_socket;

    struct addrinfo hints;
    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* for wildcard IP address */

    struct addrinfo *a;
    getaddrinfo(NULL, "51707", &hints, &a);
    struct addrinfo* p = a;
    for(;p != NULL; p = p->ai_next) {
        struct sockaddr_in* t = (struct sockaddr_in*)p->ai_addr;
        printf("[%s]\n", inet_ntoa(t->sin_addr));
    }

    if ((my_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Unable to create socket\n");
        exit(-1);
    }

    if (bind(my_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind\n");
        exit(-1);
    }

    printf("Binding success as %s:%hu, reciever started\nPress any key to start\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    getchar();

    initTermios(0);
    char text[(FIELD_Y + 2)][(FIELD_X + 3)];
    for (int i = 0; i < FIELD_Y + 2; i++)
        for (int j = 0; j < FIELD_X + 3; j++)
            text[i][j] = ' ';
    for (int i = 0; i < FIELD_X + 2; i++) {
        text[0][i] = '*';
        text[FIELD_Y + 1][i] = '*';
    }
    for (int i = 0; i < FIELD_Y + 2; i++) {
        text[i][0] = '*';
        text[i][FIELD_X + 1]= '*';
        text[i][FIELD_X + 2] = '\n';
    }
    printf("%s", text);
    char cmd = ' ';
    int x, y;
    x = y = 0;
    int direction = 0;
    for (int i = 0;; i++) {
        usleep(300000);
        char mes;
        socklen_t addr_size;
        recvfrom(my_socket, &mes, 1, 0, (struct sockaddr*)&addr_to_recv, &addr_size);
        //char buf = getchar();
        if (mes != -1) {
            switch (mes)
            {
            case 'd': direction = 0; break;
            case 's': direction = 1; break;
            case 'a': direction = 2; break;
            case 'w': direction = 3; break;
            default: break;
            }
        }
        text[y + 1][x + 1] = '.';
        switch (direction)
        {
            case 0:
                x += 2;
                if (x >= FIELD_X)
                    x -= FIELD_X;
                break;
            case 1:
                y++;
                if (y >= FIELD_Y)
                    y -= FIELD_Y;
                break;
            case 2:
                x -= 2;
                if (x < 0)
                    x += FIELD_X;
                break;
            case 3:
                y--;
                if (y < 0)
                    y += FIELD_Y;
                break;
        }

        text[y + 1][x + 1] = 'O';
        printf("\033[0d\033[2J");
        printf("%s", text);
    }
}