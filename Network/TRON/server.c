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
#include <time.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

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

struct point_t {
    char x, y;
    char rotation;
};

const struct point_t start_pos[4] = {{1, 1, 0}, {FIELD_X - 2, 1, 2}, {1, FIELD_Y - 2, 0}, {FIELD_X - 2, FIELD_Y - 2, 2}};

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

long get_time()
{
    long            us; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    us = (spec.tv_nsec / 1000) + s * 1000000; // Convert nanoseconds to milliseconds

    return us;
}

int main() {
    printf("Enter number of player: ");
    unsigned player_count = 0;
    scanf("%u", &player_count);
    if (player_count < 1) {
        printf("At least 1 player allowed\n");
        exit(-1);
    }
    if (player_count >= 5) {
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
        if (players_connected == player_count) {
            printf("Everyone connected\n");
            break;
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
                out_pack.data[0] = players_connected;
                sendto(server_socket, &out_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, buf_client_size);
            }
        }
    }
    printf("All player connected. Starting in 5 seconds\n");
    sleep(5);

    struct network_package starter;
    starter.type = GAME_START;

    for (int i = 0; i < player_count; i++)
        sendto(server_socket, &starter, sizeof(struct network_package), 0, (struct sockaddr*)(clients + i), sizeof(struct sockaddr));
    int* client_package = (int*)calloc(player_count, sizeof(int));
    char* client_commands = (char*)calloc(player_count, sizeof(char));
    struct point_t* positions = (struct point_t*)calloc(player_count, sizeof(struct point_t));

    for (int i = 0; i < player_count; i++)
        positions[i] = start_pos[i];
    char gamefield[FIELD_Y][FIELD_X] = {};
    for (int i = 0; i < FIELD_Y; i++)
        for(int j = 0; j < FIELD_X; j++)
            gamefield[i][j] = ' ';
    char drawable_field[FIELD_Y * (FIELD_X * 2 + 1)];
    for(int i = 0; i < FIELD_Y * (FIELD_X * 2 + 1); i++)
        drawable_field[i] = ' ';
    while (1) {
        long start = get_time();
        struct sockaddr_in buf_client;
        socklen_t buf_client_size = sizeof(struct sockaddr);
        struct network_package in_pack = {}, out_pack = {};
        out_pack.type = GAME_TURN_REQ;
        for (int i = 0; i < player_count; i++)
            client_package[i] = 0;
        for (int i = 0; i < player_count; i++)
            sendto(server_socket, &out_pack, sizeof(struct network_package), 0, (struct sockaddr*)(clients + i), sizeof(struct sockaddr));
        while (1) {
            recvfrom(server_socket, &in_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, &buf_client_size);
            if (in_pack.type != GAME_TURN_DATA)
                continue;
            client_package[in_pack.data[0]] = 1;
            client_commands[in_pack.data[0]] = in_pack.data[1];
            int flag = 1;
            for (int i = 0; i < player_count; i++)
                if (!client_package[i])
                    flag = 0;
            if (flag)
                break;
        }
        for(int i = 0; i < player_count; i++) {
            switch (client_commands[i])
            {
            case 'a': positions[i].rotation = 2; break;
            case 's': positions[i].rotation = 3; break;
            case 'd': positions[i].rotation = 0; break;
            case 'w': positions[i].rotation = 1; break;
            }
            gamefield[positions[i].y][positions[i].x] = '.';
            switch (positions[i].rotation)
            {
            case 0:
                positions[i].x++;
                if (positions[i].x >= FIELD_X)
                    positions[i].x -= FIELD_X;
                break;
            case 1:
                positions[i].y--;
                if (positions[i].y < 0)
                    positions[i].y += FIELD_Y;
                break;
            case 2:
                positions[i].x--;
                if (positions[i].x < 0)
                    positions[i].x += FIELD_X;
                break;
            case 3:
                positions[i].y++;
                if (positions[i].y >= FIELD_Y)
                    positions[i].y -= FIELD_Y;
                break;
            }
            gamefield[positions[i].y][positions[i].x] = '1' + i;
        }
        printf("\033[0d\033[2J");
        for(int i = 0; i < 50; i++)
            printf("*");
        printf("\n");
        for (int i = 0; i < FIELD_Y; i++) {
            for(int j = 0; j < FIELD_X; j++)
                drawable_field[i * (FIELD_X * 2 + 1) + j * 2] = gamefield[i][j];
            drawable_field[i * (FIELD_X * 2 + 1) + FIELD_X * 2] = '\n';
        }
        printf("%s", drawable_field);
        long end = get_time();

        out_pack.type = GAME_TURN_DATA;
        for (int i = 0; i < DATA_SIZE; i++)
            out_pack.data[i] = -1;
        for (int i = 0; i < player_count; i++) {
            out_pack.data[2 * i] = positions[i].x;
            out_pack.data[2 * i + 1] = positions[i].y;
        }
        for (int i = 0; i < player_count; i++)
            sendto(server_socket, &out_pack, sizeof(struct network_package), 0, (struct sockaddr*)(clients + i), sizeof(struct sockaddr));

        for (int i = 0; i < player_count; i++)
            client_package[i] = 0;
        while (1) {
            recvfrom(server_socket, &in_pack, sizeof(struct network_package), 0, (struct sockaddr*)&buf_client, &buf_client_size);
            if (in_pack.type != OK)
                continue;
            client_package[in_pack.data[0]] = 1;
            int flag = 1;
            for (int i = 0; i < player_count; i++)
                if (!client_package[i])
                    flag = 0;
            if (flag)
                break;
        }

        usleep(300000 - (end - start));
    }
}