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

#define DATA_SIZE 16
#define IP_LEN 17

enum package_t {
    ERROR,
    INCORRECT_MES,
    CONNECTION_REQ,
    CONNECTION_ACCEPT,
    CONNECTION_DENY,

    GAME_START,

    GAME_TURN_REQ,
    GAME_TURN_DATA,

    DISCONNECT
};

struct network_package {
    enum package_t type;
    char data[DATA_SIZE];
};

struct sockaddr_in InitServer(char ip[IP_LEN], int port) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_aton(ip, &server.sin_addr);
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

int main() {
    printf("Enter number of player: ");
    unsigned player_count = 0;
    scanf("%u", &player_count);
    if (player_count < 2) {
        printf("At least 2 player allowed\n");
        exit(-1);
    }
    if (player_count >= 10) {
        printf("Too many players\n");
        exit(-1);
    }

    struct sockaddr_in server;
    struct sockaddr_in* clients = (struct sockaddr_in*)calloc(player_count, sizeof(struct sockaddr_in));
    int* clients_connection = (int*)calloc(player_count, sizeof(int));
    int players_connected = 0;

    printf("Enter server ip: ");
    char server_ip[IP_LEN];
    scanf("%s", server_ip);

    printf("Enter server port: ");
    unsigned port = 0;
    scanf("%u", &port);

    server = InitServer(server_ip, port);
    int server_socket =  CreateSocket(&server);

    while(1) {
        printf("\033[0d\033[2J");
        printf("Server created at %s:%u\n. Waiting for players:\n", inet_ntoa(server.sin_addr), port);
        for (int i = 0; i < player_count; i++) {
            printf("[%d]: ", i + 1);
            if (clients_connection[i])
                printf("%s:%hu connected\n", inet_ntoa(clients[i].sin_addr), ntohs(clients[i].sin_port));
            else
                printf("Not connected\n");
        }

        struct sockaddr_in buf_client;
        socklen_t buf_client_size = sizeof(struct sockaddr);
        struct network_package in_pack = {}, out_pack = {};

        int out = recvfrom(server_socket, &in_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, &buf_client_size);
        if (buf_client.sin_port == 0)
            continue;
        printf("%d\n", out);
        if (in_pack.type != CONNECTION_REQ) {
            out_pack.type = INCORRECT_MES;
            sendto(server_socket, &out_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, buf_client_size);
        }
        else {
            int success_flag = 1;
            for (int i = 0; i < players_connected; i++) {
                if (clients[i].sin_addr.s_addr == buf_client.sin_addr.s_addr && clients[i].sin_port == buf_client.sin_port) {
                    out_pack.type = CONNECTION_DENY;
                    sendto(server_socket, &out_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, buf_client_size);
                    success_flag = 0;
                }
            }
            if (success_flag) {
                clients[players_connected] = buf_client;
                clients_connection[players_connected] = 1;
                players_connected++;

                out_pack.type = CONNECTION_ACCEPT;
                sendto(server_socket, &out_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, buf_client_size);

                if (players_connected == player_count)
                    break;
            }
        }
    }
    printf("All player connected. Starting in 3 seconds\n");
    sleep(3);

    struct network_package starter;
    starter.type = GAME_START;

    for (int i = 0; i < player_count; i++)
        sendto(server_socket, &starter, sizeof(struct network_package), 0, (struct sockaddr*)(clients + i), sizeof(struct sockaddr));

    while (1) {
        

        sleep(1);
    }
}