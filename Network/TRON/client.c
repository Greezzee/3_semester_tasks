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
#include <pthread.h>

#define DATA_SIZE 16
#define IP_LEN 17
#define FIELD_X 20
#define FIELD_Y 20

enum package_t {
    ERROR,
    INCORRECT_MES,
    CONNECTION_REQ,
    CONNECTION_ACCEPT,
    CONNECTION_DENY,

    GAME_START,

    GAME_TURN_REQ,
    GAME_TURN_DATA,

    OK,

    DISCONNECT,

    GAME_OVER
};

struct network_package {
    enum package_t type;
    char data[DATA_SIZE];
};

struct sockaddr_in InitClient(char ip[IP_LEN]) {
    struct sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = INADDR_ANY;
    inet_aton(ip, &client.sin_addr);
    return client;
}

struct sockaddr_in InitServer(char IP[IP_LEN], unsigned host) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(host);
    inet_aton(IP, &server.sin_addr);
    return server;
}   

int CreateSocket(struct sockaddr_in* addr) {
    int my_socket;
    if ((my_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Unable to create socket\n");
        exit(-1);
    }

    if (bind(my_socket, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
        perror("Unable to bind\n");
        exit(-1);
    }
    return my_socket;
}

void RecievePackageFromServer(int my_socket, struct sockaddr_in* server, struct network_package* package) {
    while (1) {
        struct sockaddr_in buf;
        socklen_t buf_len = sizeof(struct sockaddr);
        recvfrom(my_socket, package, sizeof(struct network_package), 0, (struct sockaddr*)&buf, &buf_len);
        //printf("Recieved\n");
        if (buf.sin_addr.s_addr == server->sin_addr.s_addr && buf.sin_port == server->sin_port)
            return;
        //printf("Wrong server\n");
    }
}

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

void* UpdateInput(void* buffer)
{
    char* input = (char*)buffer;
    while(1) {
        char buf = getchar();
        if (buf != -1)
            *input = buf;
    }
    return NULL;
}

int main() {
    struct sockaddr_in server = {}, client = {};

    char server_ip[IP_LEN] = {}, client_ip[IP_LEN] = {};
    unsigned server_port;
    printf("Enter your IP: ");
    scanf("%s", client_ip);
    printf("Enter IP to connect: ");
    scanf("%s", server_ip);
    printf("Enter port: ");
    scanf("%u", &server_port);

    client = InitClient(client_ip);
    server = InitServer(server_ip, server_port);
    int client_socket = CreateSocket(&client);

    struct network_package pack, server_pack;
    pack.type = CONNECTION_REQ;
    sendto(client_socket, &pack, sizeof(pack), 0, (struct sockaddr*)&server, sizeof(server));
    RecievePackageFromServer(client_socket, &server, &pack);
    if (pack.type != CONNECTION_ACCEPT) {
        printf("Server error\n");
        exit(-1);
    }

    char your_id = pack.data[0];
    printf("Connected to server waiting for players. You are [%hhd]\n", your_id);
    char input_buf = -1;
    do {
        RecievePackageFromServer(client_socket, &server, &pack);
    } while (pack.type != GAME_START);
    printf("Game started!\n");
    initTermios(0);
    pthread_t input_id;
    pthread_create(&input_id, NULL, UpdateInput, &input_buf);


    char drawable_gamefield[(FIELD_Y + 2) * (FIELD_X * 2 + 3)];

    for (int i = 0; i < (FIELD_Y + 2) * (FIELD_X * 2 + 3); i++)
        drawable_gamefield[i] = ' ';
    for (int i = 0; i < FIELD_Y + 2; i++) {
        drawable_gamefield[i * (FIELD_X * 2 + 3)] = '*';
        drawable_gamefield[i * (FIELD_X * 2 + 3) + FIELD_X * 2 + 1] = '*';
        drawable_gamefield[i * (FIELD_X * 2 + 3) + FIELD_X * 2 + 2] = '\n';
    }
    for (int i = 0; i < FIELD_X * 2 + 2; i++) {
        drawable_gamefield[i] = '*';
        drawable_gamefield[(FIELD_Y + 1) * (FIELD_X * 2 + 3) + i] = '*';
    }


    while(1) {
        do {
            RecievePackageFromServer(client_socket, &server, &pack);
        } while (pack.type != GAME_TURN_REQ);
        printf("OK\n");
        pack.type = GAME_TURN_DATA;
        pack.data[0] = your_id - 1;
        pack.data[1] = input_buf;
        input_buf = -1;
        sendto(client_socket, &pack, sizeof(pack), 0, (struct sockaddr*)&server, sizeof(server));
        do {
            RecievePackageFromServer(client_socket, &server, &server_pack);
        } while (server_pack.type != GAME_TURN_DATA);

        pack.type = OK;
        sendto(client_socket, &pack, sizeof(pack), 0, (struct sockaddr*)&server, sizeof(server));

        for (int i = 0; i < (FIELD_Y + 2) * (FIELD_X * 2 + 3); i++)
            if (drawable_gamefield[i] != ' ' && drawable_gamefield[i] != '*' && drawable_gamefield[i] != '\n')
                drawable_gamefield[i] = '.';

        for(int i = 0; server_pack.data[i] != -1; i += 2) {
            drawable_gamefield[(server_pack.data[i + 1] + 1) * (FIELD_X * 2 + 3) + server_pack.data[i] * 2 + 1] = '1' + i / 2;
        }

        printf("\033[0d\033[2J");
        printf("%s", drawable_gamefield);
    }
}